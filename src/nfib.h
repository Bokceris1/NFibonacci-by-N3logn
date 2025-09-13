#pragma once

#include <boost/multiprecision/cpp_int.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>

using namespace boost::multiprecision;

template <typename T, size_t SIDE>
class SquareMatrix {
public:
  using ValueType = T;

  using Reference = T&;
  using ConstReference = const T&;

  using Pointer = T*;
  using ConstPointer = const T*;

  SquareMatrix() {
    for (size_t i = 0; i < SIDE * SIDE; ++i) {
      _data[i] = 0;
    }
  }

  Reference operator[](size_t row, size_t col) {
    return _data[row * SIDE + col];
  }

  ConstReference operator[](size_t row, size_t col) const {
    return _data[row * SIDE + col];
  }

  // mul

  friend SquareMatrix operator*(const SquareMatrix& left, const SquareMatrix& right) {
    SquareMatrix m;
    for (std::size_t i = 0; i < SIDE; ++i) {
      for (std::size_t k = 0; k < SIDE; ++k) {
        T a_ik = left[i, k];
        for (std::size_t j = 0; j < SIDE; ++j) {
          m[i, j] += a_ik * right[k, j];
        }
      }
    }
    return m;
  }

  SquareMatrix genIdentity() {
    SquareMatrix id;
    for (size_t i = 0; i < SIDE; ++i) {
      id[i, i] = 1;
    }
    return id;
  }

  SquareMatrix pow(size_t power) {
    SquareMatrix result = genIdentity();

    SquareMatrix base = *this;

    while (power > 0) {
      if (power & 1) {
        result = result * base;
      }
      base = base * base;
      power = power >>= 1;
    }

    return result;
  }

private:
  std::array<T, SIDE * SIDE> _data;
};

template <typename IntType, size_t N>
SquareMatrix<IntType, N> genBase() {
  SquareMatrix<IntType, N> base;
  for (size_t i = 0; i < N; ++i) {
    base[0, i] = 1;
  }
  for (size_t i = 1; i < N; ++i) {
    base[i, i - 1] = 1;
  }
  return base;
}

template <typename IntType, size_t N>
IntType fib_iterative(size_t n) {
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

template <typename IntType>
IntType fib_impl(size_t n) {
  IntType a = 0, b = 1;
  IntType bit = 1;

  while (bit <= n / 2) {
    bit <<= 1;
  }

  while (bit > 0) {
    IntType c = a * (2 * b - a);
    IntType d = a * a + b * b;
    if (n & bit) {
      a = d;
      b = c + d;
    } else {
      a = c;
      b = d;
    }
    bit >>= 1;
  }
  return a;
}

template <typename IntType, size_t N>
IntType fib(size_t n) {
  if (n == 0) {
    return 0;
  }

  if (N == 2) {
    return fib_impl<IntType>(n - 1);
  }

  if (n <= N) {
    return 1;
  }

  if (pow(N, 3) * log2(n) > N * n) {
    return fib_iterative<IntType, N>(n);
  }

  SquareMatrix matrix = genBase<IntType, N>();

  SquareMatrix powered = matrix.pow(n - N - 1);

  IntType sum = 0;
  for (size_t i = 0; i < N; ++i) {
    sum += powered[0, i];
  }

  return sum;
}
