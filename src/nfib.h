#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <iostream>

template <typename T, size_t SIDE>
class SquareMatrix {
private:
  template <typename N>
  class BasicColumnIterator {
  public:
    using value_type = T;
    using reference = N&;
    using pointer = N*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    using ConstColumnIterator = BasicColumnIterator<const N>;

    reference operator[](size_t off) {
      return *(*this + off);
    }

    reference operator[](size_t off) const {
      return *(*this + off);
    }

    reference operator*() const {
      return _data[_offset];
    }

    pointer operator->() const {
      return _data + _offset;
    }

    operator ConstColumnIterator() const {
      return ConstColumnIterator(_data, _cols, _offset);
    }

    BasicColumnIterator& operator++() {
      _data += _cols;
      return *this;
    }

    BasicColumnIterator operator++(int) {
      BasicColumnIterator result = *this;
      ++(*this);
      return result;
    }

    BasicColumnIterator& operator--() {
      _data -= _cols;
      return *this;
    }

    BasicColumnIterator operator--(int) {
      BasicColumnIterator result = *this;
      --(*this);
      return result;
    }

    BasicColumnIterator& operator=(const BasicColumnIterator& other) = default;

    friend BasicColumnIterator operator+(const BasicColumnIterator& left, difference_type right) {
      return {left._data + (right * static_cast<difference_type>(left._cols)), left._cols, left._offset};
    }

    friend BasicColumnIterator operator-(const BasicColumnIterator& left, difference_type right) {
      return left + (-right);
    }

    friend BasicColumnIterator operator+(difference_type left, const BasicColumnIterator& right) {
      return right + left;
    }

    friend BasicColumnIterator operator-(difference_type left, const BasicColumnIterator& right) {
      return right - left;
    }

    friend difference_type operator+(const BasicColumnIterator& left, const BasicColumnIterator& right) {
      return (left._data + right._data) / left._cols;
    }

    friend difference_type operator-(const BasicColumnIterator& left, const BasicColumnIterator& right) {
      return (left._data - right._data) / static_cast<difference_type>(left._cols);
    }

    BasicColumnIterator& operator+=(difference_type other) {
      *this = *this + other;
      return *this;
    }

    BasicColumnIterator& operator-=(difference_type other) {
      *this = *this - other;
      return *this;
    }

    friend bool operator==(const BasicColumnIterator& lhs, const BasicColumnIterator& rhs) = default;

    friend auto operator<=>(const BasicColumnIterator& lhs, const BasicColumnIterator& rhs) = default;

    BasicColumnIterator() = default;

    BasicColumnIterator(const BasicColumnIterator&) = default;

  private:
    BasicColumnIterator(pointer data, std::size_t cols, std::size_t col)
        : _data(data)
        , _cols(cols)
        , _offset(col) {}

    pointer _data;
    std::size_t _cols;
    std::size_t _offset;

    friend class SquareMatrix;
  };

public:
  using ValueType = T;

  using Reference = T&;
  using ConstReference = const T&;

  using Pointer = T*;
  using ConstPointer = const T*;

  using Iterator = T*;
  using ConstIterator = const T*;

  using RowIterator = T*;
  using ConstRowIterator = const T*;

  using ColIterator = BasicColumnIterator<T>;
  using ConstColIterator = BasicColumnIterator<const T>;

public:
  SquareMatrix()
      : _data(new T[SIDE * SIDE]{}) {
    for (Iterator it = begin(); it < end(); ++it) {
      *it = 0;
    }
  }

  SquareMatrix(const SquareMatrix& other)
      : SquareMatrix() {
    std::copy(other.begin(), other.end(), begin());
  }

  SquareMatrix& operator=(const SquareMatrix& other) {
    if (this != &other) { 
      SquareMatrix tmp = other;
      swap(*this, tmp);
    }
    return *this;
  }

  friend void swap(SquareMatrix& lhs, SquareMatrix& rhs) 
  {
    std::swap(lhs._data, rhs._data);
  }

  ~SquareMatrix() {
    delete[] _data;
  }

  // Iterators

  Iterator begin() {
    return _data;
  }

  ConstIterator begin() const {
    return _data;
  }

  Iterator end() {
    return begin() + size();
  }

  ConstIterator end() const {
    return begin() + size();
  }

  RowIterator row_begin(size_t row) {
    return begin() + row * side();
  }

  ConstRowIterator row_begin(size_t row) const {
    return begin() + row * side();
  }

  RowIterator row_end(size_t row) {
    return row_begin(row) + side();
  }

  ConstRowIterator row_end(size_t row) const {
    return row_begin(row) + side();
  }

  ColIterator col_begin(std::size_t col) {
    return {begin(), side(), col};
  }

  ColIterator col_end(std::size_t col) {
    return {end(), side(), col};
  }

  ConstColIterator col_begin(std::size_t col) const {
    return {begin(), side(), col};
  }

  ConstColIterator col_end(std::size_t col) const {
    return {end(), side(), col};
  }

  // Size

  size_t side() const {
    return SIDE;
  }

  size_t size() const {
    return side() * side();
  }

  bool empty() const {
    return size() == 0;
  }

  // Elements access

  Reference operator()(size_t row, size_t col) {
    return _data[row * SIDE + col];
  }

  ConstReference operator()(size_t row, size_t col) const {
    return _data[row * SIDE + col];
  }

  Pointer data() {
    return _data;
  }

  ConstPointer data() const {
    return _data;
  }

  // mul

  SquareMatrix& operator*=(const SquareMatrix& other) {
    SquareMatrix m = *this * other;
    swap(*this, m);
    return *this;
  }

  friend SquareMatrix operator*(const SquareMatrix& left, const SquareMatrix& right) {
    SquareMatrix m = SquareMatrix();
    for (std::size_t row = 0; row < SIDE; ++row) {
      for (std::size_t col = 0; col < SIDE; ++col) {
        m._data[(row * SIDE) + col] =
            std::inner_product(right.col_begin(col), right.col_end(col), left.row_begin(row), T());
      }
    }
    return m;
  }

  friend std::array<T, SIDE> operator*(const std::array<T, SIDE>& left, const SquareMatrix& right) {
    std::array<T, SIDE> m;
    for (std::size_t col = 0; col < SIDE; ++col) {
      m[col] = std::inner_product(right.col_begin(col), right.col_end(col), left.begin(), T());
    }
    return m;
  }

  SquareMatrix genIdentity() {
    SquareMatrix id;
    for (size_t i = 0; i < side(); ++i) {
      id(i, i) = 1;
    }
    return id;
  }

  SquareMatrix pow(size_t power) {
    SquareMatrix result = genIdentity();
    SquareMatrix base = *this;
    while (power > 0) {
      if (power & 1) {
        result *= base;
      }
      base *= base;
      power = power / 2;
    }
    return result;
  }

private:
  T* _data;
};

// Generalization of the Binet formula
template <size_t N>
SquareMatrix<size_t, N> genBase() {
  SquareMatrix<size_t, N> base;
  for (size_t i = 0; i < N; ++i) {
    base(i, 0) = 1;
  }
  for (size_t i = 1; i < N; ++i) {
    base(i - 1, i) = 1;
  }
  return base;
}

template <size_t N>
size_t fib(size_t n) {
	if (n == 0) {
		return 0;
	}
    if (n <= N) {
      return 1;
    }
    SquareMatrix matrix = genBase<N>();
    std::array<size_t, N> first{};
    first.fill(1);
    SquareMatrix powered = matrix.pow(n - N);
    first = first * powered;
    return first[0];
}
