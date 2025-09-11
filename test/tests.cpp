#include "nfib.h"

#include <gtest/gtest.h>

namespace {

template <size_t N>
size_t fib_correct(size_t n) {
  if (n <= N) {
    return 1;
  }
  std::vector<size_t> base(N, 1);
  for (size_t i = 0; i < n - N; ++i) {
    size_t sum = 0;
    for (size_t j = 1; j <= N; ++j) {
      sum += base[base.size() - j];
    }
    base.push_back(sum);
  }
  return base[base.size() - 1];
}

} // namespace

TEST(correctness, 11_element) {
  size_t n = 11;
  size_t answer = 89;
  EXPECT_EQ(fib<2>(n), answer);
}

TEST(correctness, 93_element) {
  size_t n = 93;
  size_t answer = 12200160415121876738ULL;
  EXPECT_EQ(fib<2>(n), answer);
}

TEST(correctness, 5_trielement) {
  size_t n = 5;
  size_t answer = 5;
  EXPECT_EQ(fib<3>(n), answer);
}

TEST(correctness, 6_trielement) {
  size_t n = 6;
  size_t answer = 9;
  EXPECT_EQ(fib<3>(n), answer);
}

TEST(correctness, 7_trielement) {
  size_t n = 7;
  EXPECT_EQ(fib<3>(n), fib_correct<3>(n));
}

TEST(correctness, 8_trielement) {
  size_t n = 8;
  size_t answer = 31;
  EXPECT_EQ(fib<3>(n), answer);
}

TEST(correctness, 9_trielement) {
  size_t n = 9;
  size_t answer = 57;
  EXPECT_EQ(fib<3>(n), answer);
}

TEST(correctness, 10_trielement) {
  size_t n = 10;
  size_t answer = 105;
  EXPECT_EQ(fib<3>(n), answer);
}

TEST(correctness, 11_trielement) {
  size_t n = 11;
  size_t answer = 193;
  EXPECT_EQ(fib<3>(n), answer);
}

TEST(correctness, 20_trielement) {
  size_t n = 20;
  size_t answer = 46499;
  EXPECT_EQ(fib<3>(n), answer);
}

TEST(correctness, 35_trielement) {
  size_t n = 35;
  EXPECT_EQ(fib<3>(n), fib_correct<3>(n));
}

TEST(correctness, 90_trielement) {
  size_t n = 90;
  EXPECT_EQ(fib<3>(n), fib_correct<3>(n));
}

TEST(correctness, 10_4element) {
  size_t n = 10;
  EXPECT_EQ(fib<4>(n), fib_correct<4>(n));
}

TEST(correctness, 5_10element) {
  size_t n = 5;
  EXPECT_EQ(fib<10>(n), fib_correct<10>(n));
}

TEST(correctness, bigtest) {
    for (size_t j = 7; j < 30; ++j) {
      EXPECT_EQ(fib<7>(j), fib_correct<7>(j));
    }
}
