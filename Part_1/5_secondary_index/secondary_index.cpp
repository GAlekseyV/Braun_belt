#include "test_runner.h"

#include <list>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>

using namespace std;

struct Record
{
  string id;
  string title;
  string user;
  int timestamp;
  int karma;

  bool operator==(const Record &other) const
  {
    return id == other.id
           && title == other.title
           && user == other.user
           && timestamp == other.timestamp
           && karma == other.karma;
  }

  bool operator<(const Record &other) const
  {
    return tie(id, title, user, timestamp, karma)
           < tie(other.id, other.title, other.user, other.timestamp, other.karma);
  }
};

// Реализуйте этот класс
class Database
{
public:
  bool Put(const Record &record);
  const Record *GetById(const string &id) const;
  bool Erase(const string &id);

  template<typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const
  {
    auto low_it = by_timestamp_idx.lower_bound(low);
    auto high_it = by_timestamp_idx.upper_bound(high);
    for (auto it = low_it; it != high_it; ++it) {
      if (!callback(*(it->second))) {
        break;
      };
    }
  }

  template<typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const
  {
    auto low_it = by_karma_idx.lower_bound(low);
    auto high_it = by_karma_idx.upper_bound(high);
    for (auto it = low_it; it != high_it; ++it) {
      if (!callback(*(it->second))) {
        break;
      };
    }
  }

  template<typename Callback>
  void AllByUser(const string &user, Callback callback) const
  {
    const auto range = by_user_idx.equal_range(user);
    for (auto item = range.first; item != range.second; item++) {
      if (!callback(*(item->second))) {
        break;
      };
    }
  }

private:
  list<Record> database;
  using ListIt = list<Record>::iterator;
  using TsIt = multimap<int, ListIt>::iterator;
  using KrIt = multimap<int, ListIt>::iterator;
  using UserIt = unordered_multimap<string, ListIt>::iterator;

  struct Idx
  {
    ListIt record;
    TsIt ts;
    KrIt kr;
    UserIt user;
  };

  unordered_map<string, Idx> by_id_idx;
  multimap<int, ListIt> by_timestamp_idx;
  unordered_multimap<string, ListIt> by_user_idx;
  multimap<int, ListIt> by_karma_idx;
};

bool Database::Put(const Record &record)
{
  if (by_id_idx.count(record.id) == 0) {
    auto it = database.insert(database.begin(), record);
    auto ts_it = by_timestamp_idx.insert({ record.timestamp, it });
    auto user_it = by_user_idx.insert({ record.user, it });
    auto kr_it = by_karma_idx.insert({ record.karma, it });
    by_id_idx[record.id] = Idx{ it, ts_it, kr_it, user_it };
    return true;
  }
  return false;
}

const Record *Database::GetById(const string &id) const
{
  auto it = by_id_idx.find(id);
  if (it != by_id_idx.end()) {
    return &(*(it->second.record));
  }
  return nullptr;
}

bool Database::Erase(const string &id)
{
  auto it = by_id_idx.find(id);
  if (it == by_id_idx.end()) {
    return false;
  }

  Idx idx = it->second;
  // Erase index entries using stored iterators (O(1))
  by_timestamp_idx.erase(idx.ts);
  by_user_idx.erase(idx.user);
  by_karma_idx.erase(idx.kr);

  // Erase record from main storage
  database.erase(idx.record);

  // Erase id index
  by_id_idx.erase(it);

  return true;
}


void TestRangeBoundaries()
{
  const int good_karma = 1000;
  const int bad_karma = -10;
  const int timestamp = 1536107260;

  Database db;
  db.Put({ "id1", "Hello there", "master", timestamp, good_karma });
  db.Put({ "id2", "O>>-<", "general2", timestamp + 2, bad_karma });

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record &) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);

  int timestamp_cnt = 0;
  db.RangeByTimestamp(timestamp, timestamp + 1, [&timestamp_cnt](const Record &) {
    ++timestamp_cnt;
    return true;
  });
  ASSERT_EQUAL(1, timestamp_cnt);
}

void TestSameUser()
{
  Database db;
  db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
  db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

  int count = 0;
  db.AllByUser("master", [&count](const Record &) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement()
{
  const string final_body = "Feeling sad";

  Database db;
  db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
  db.Erase("id");
  db.Put({ "id", final_body, "not-master", 1536107260, -10 });

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main()
{
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
