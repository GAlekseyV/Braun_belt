#include "test_runner.h"

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <unordered_set>

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
    auto low_it = by_karma_idx.lower_bound(high);
    auto high_it = by_karma_idx.upper_bound(low);
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
  map<string, list<Record>::iterator> by_id_idx;
  map<int, list<Record>::iterator> by_timestamp_idx;
  multimap<string, list<Record>::iterator, greater<>> by_user_idx;
  multimap<int, list<Record>::iterator, greater<>> by_karma_idx;
};

bool Database::Put(const Record &record)
{
  if (by_id_idx.count(record.id) == 0) {
    auto it = database.insert(database.begin(), record);
    by_id_idx[record.id] = it;
    by_timestamp_idx[record.timestamp] = it;
    by_user_idx.insert({ record.user, it });
    by_karma_idx.insert({ record.karma, it });
    return true;
  }
  return false;
}

const Record *Database::GetById(const string &id) const
{
  if (by_id_idx.count(id) > 0) {
    return &(*by_id_idx.at(id));
  }
  return nullptr;
}

bool Database::Erase(const string &id)
{
  auto it = by_id_idx.find(id);
  if (it == by_id_idx.end()) {
    return false;
  }

  auto record_it = it->second;
  const Record &record = *record_it;

  // Remove from by_id_idx
  by_id_idx.erase(it);

  // Remove from by_timestamp_idx
  auto ts_range = by_timestamp_idx.equal_range(record.timestamp);
  for (auto ts_it = ts_range.first; ts_it != ts_range.second; ++ts_it) {
    if (ts_it->second == record_it) {
      by_timestamp_idx.erase(ts_it);
      break;
    }
  }

  // Remove from by_user_idx
  auto user_range = by_user_idx.equal_range(record.user);
  for (auto user_it = user_range.first; user_it != user_range.second; ++user_it) {
    if (user_it->second == record_it) {
      by_user_idx.erase(user_it);
      break;
    }
  }

  // Remove from by_karma_idx
  auto karma_range = by_karma_idx.equal_range(record.karma);
  for (auto karma_it = karma_range.first; karma_it != karma_range.second; ++karma_it) {
    if (karma_it->second == record_it) {
      by_karma_idx.erase(karma_it);
      break;
    }
  }

  // Remove from database
  database.erase(record_it);

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
