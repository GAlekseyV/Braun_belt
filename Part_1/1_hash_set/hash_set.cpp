#include "test_runner.h"
#include <algorithm>
#include <cstddef>
#include <forward_list>
#include <iterator>
#include <vector>

using namespace std;

template<typename Type, typename Hasher>
class HashSet
{
public:
  using BucketList = forward_list<Type>;

  explicit HashSet(
    size_t num_buckets,
    const Hasher &hasher = {}) : _hasher(hasher)
  {
    _buckets.resize(num_buckets);
  }

  void Add(const Type &value)
  {
    if (!Has(value)) {
      _buckets[_hash(value)].push_front(value);
    }
  }

  [[nodiscard]] bool Has(const Type &value) const
  {
    const auto &bucket = _buckets[_hash(value)];
    return std::find(bucket.begin(), bucket.end(), value) != bucket.end();
  }

  void Erase(const Type &value)
  {
    if (Has(value)) {
      _buckets[_hash(value)].remove(value);
    }
  }

  const BucketList &GetBucket(const Type &value) const
  {
    return _buckets[_hash(value)];
  }

private:
  size_t _hash(const Type &value) const
  {
    auto h = _hasher(value);
    auto bsz = _buckets.size();
    return _hasher(value) % _buckets.size();
  }
  vector<BucketList> _buckets;
  Hasher _hasher;
};

struct IntHasher
{
  size_t operator()(int value) const
  {
    // Это реальная хеш-функция из libc++, libstdc++.
    // Чтобы она работала хорошо, std::unordered_map
    // использует простые числа для числа бакетов
    return value;
  }
};

struct TestValue
{
  int value;

  bool operator==(TestValue other) const
  {
    return value / 2 == other.value / 2;
  }
};

struct TestValueHasher
{
  size_t operator()(TestValue value) const
  {
    return value.value / 2;
  }
};

void TestSmoke()
{
  HashSet<int, IntHasher> hash_set(2);
  hash_set.Add(3);
  hash_set.Add(4);

  ASSERT(hash_set.Has(3));
  ASSERT(hash_set.Has(4));
  ASSERT(!hash_set.Has(5));

  hash_set.Erase(3);

  ASSERT(!hash_set.Has(3));
  ASSERT(hash_set.Has(4));
  ASSERT(!hash_set.Has(5));

  hash_set.Add(3);
  hash_set.Add(5);

  ASSERT(hash_set.Has(3));
  ASSERT(hash_set.Has(4));
  ASSERT(hash_set.Has(5));
}

void TestEmpty()
{
  HashSet<int, IntHasher> hash_set(10);
  for (int value = 0; value < 10000; ++value) {
    ASSERT(!hash_set.Has(value));
  }
}

void TestIdempotency()
{
  HashSet<int, IntHasher> hash_set(10);
  hash_set.Add(5);
  ASSERT(hash_set.Has(5));
  hash_set.Add(5);
  ASSERT(hash_set.Has(5));
  hash_set.Erase(5);
  ASSERT(!hash_set.Has(5));
  hash_set.Erase(5);
  ASSERT(!hash_set.Has(5));
}

void TestEquivalence()
{
  HashSet<TestValue, TestValueHasher> hash_set(10);
  hash_set.Add(TestValue{ 2 });
  hash_set.Add(TestValue{ 3 });

  ASSERT(hash_set.Has(TestValue{ 2 }));
  ASSERT(hash_set.Has(TestValue{ 3 }));

  const auto &bucket = hash_set.GetBucket(TestValue{ 2 });
  const auto &three_bucket = hash_set.GetBucket(TestValue{ 3 });
  ASSERT_EQUAL(&bucket, &three_bucket);

  ASSERT_EQUAL(1, distance(begin(bucket), end(bucket)));
  ASSERT_EQUAL(2, bucket.front().value);
}

int main()
{
  TestRunner trunner;
  RUN_TEST(trunner, TestSmoke);
  RUN_TEST(trunner, TestEmpty);
  RUN_TEST(trunner, TestIdempotency);
  RUN_TEST(trunner, TestEquivalence);
  return 0;
}
