#include "nfib.h"

#include <gtest/gtest.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

namespace {

template <typename IntType, size_t N>
IntType fib_correct(size_t n) {
  if (n == 0) {
    return 0;
  }

  if (n <= N) {
    return 1;
  }

  std::array<IntType, N> base;
  base.fill(1);

  for (size_t i = 0; i < n - N - 1; ++i) {
    IntType sum = 0;
    for (size_t j = 0; j < N; ++j) {
      sum += base[j];
      if (j != N - 1) {
        base[j] = base[j + 1];
      }
    }
    base[N - 1] = sum;
  }
  return base[N - 1];
}

} // namespace

TEST(correctness, 2test) {
  for (size_t j = 2; j < 93; ++j) {
    EXPECT_EQ((fib<size_t, 2>(j)), (fib_correct<size_t, 2>(j)));
  }
}

TEST(correctness, 7_trielement) {
  size_t n = 7;
  EXPECT_EQ((fib<size_t, 3>(n)), (fib_correct<size_t, 3>(n)));
}

TEST(correctness, 35_trielement) {
  size_t n = 35;
  EXPECT_EQ((fib<size_t, 3>(n)), (fib_correct<size_t, 3>(n)));
}

TEST(correctness, 90_trielement) {
  size_t n = 90;
  EXPECT_EQ((fib<size_t, 3>(n)), (fib_correct<size_t, 3>(n)));
}

TEST(correctness, 10_4element) {
  size_t n = 10;
  EXPECT_EQ((fib<size_t, 4>(n)), (fib_correct<size_t, 4>(n)));
}

TEST(correctness, 5_10element) {
  size_t n = 5;
  EXPECT_EQ((fib<size_t, 10>(n)), (fib_correct<size_t, 10>(n)));
}

TEST(correctness, 7test) {
    for (size_t j = 6; j < 30; ++j)
    {
    EXPECT_EQ((fib<size_t, 7>(j)), (fib_correct<size_t, 7>(j)));
    }
}

TEST(correctness, bigtest1) {
  size_t j = 10000;
  EXPECT_EQ((fib<cpp_int, 10>(j)), (fib_correct<cpp_int, 10>(j)));
  //std::cout << fib<cpp_int, 10>(j) << "\n";
}

TEST(correctness, bigtest2) {
  size_t j = 10000;
  EXPECT_EQ((fib<cpp_int, 10>(j)), (fib_correct<cpp_int, 10>(j)));
  //std::cout << fib<cpp_int, 10>(j) << "\n";
}

TEST(correctness, bigtest4) {
  size_t j = 100000;
  EXPECT_EQ((fib<cpp_int, 2>(j)), (fib_correct<cpp_int, 2>(j)));
  //std::cout << fib<cpp_int, 2>(j) << "\n";
}

TEST(correctness, bigtest5) {
  //size_t j = 1000000;
  //std::cout << fib<cpp_int, 2>(j) << "\n";
}

TEST(correctness, bigtest6) {
  size_t j = 1000;
  EXPECT_EQ((fib<cpp_int, 100>(j)), (fib_correct<cpp_int, 100>(j)));
  //std::cout << fib<cpp_int, 100>(j) << "\n";
}

TEST(correctness, bigtest_matrix) {
  for (size_t j = 0; j < 1000; ++j) {
    //std::cout << j << "\n";
    EXPECT_EQ((fib<cpp_int, 30>(j)), (fib_correct<cpp_int, 30>(j)));
  }
}

TEST(correctness, bigtest_iterative) {
  for (size_t j = 0; j < 1000; ++j) {
    //std::cout << j << "\n";
    EXPECT_EQ((fib_iterative<cpp_int, 30>(j)), (fib_correct<cpp_int, 30>(j)));
  }
}

TEST(correctness, bigtest_matrix_2) {
  for (size_t j = 0; j < 1000000; j += 200000) {
    // std::cout << j << "\n";
    fib<cpp_int, 2>(j);
  }
}

//TEST(correctness, bigtest_iterative_2) {
//  for (size_t j = 0; j < 1000000; j += 200000) {
//    // std::cout << j << "\n";
//    fib_iterative<cpp_int, 2>(j);
//  }
//}

TEST(correctness, very_big) {
  size_t j = 1000000000;
  fib<cpp_int, 2>(j);
}
