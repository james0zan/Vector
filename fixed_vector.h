#ifndef _FIXED_VECTOR_H_
#define _FIXED_VECTOR_H_

#include <algorithm>
#include <memory>
#include <stdexcept>

template <class T, class A = std::allocator<T> > class FixedVector {

public:
  typedef T &reference;
  typedef const T &const_reference;
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef unsigned long size_type;

  class reverse_iterator {
  public:
    reverse_iterator(T *p) noexcept : pos(p) {}
    reverse_iterator() noexcept : pos(0) {}

    T &operator*() { return *pos; }
    T *operator->() noexcept { return pos; }

    reverse_iterator operator++() noexcept {
      --pos;
      return *this;
    }
    reverse_iterator operator++(int) noexcept {
      --pos;
      return *this;
    }
    reverse_iterator operator--() noexcept {
      ++pos;
      return *this;
    }
    reverse_iterator operator--(int) noexcept {
      ++pos;
      return *this;
    }

    bool operator!=(const reverse_iterator &rhs) noexcept {
      return this->pos != rhs.pos;
    }

  private:
    T *pos;
  };
  typedef const reverse_iterator const_reverse_iterator;

  explicit FixedVector(size_type n) {
    space = v = alloc.allocate(n);
    last = v + n;
  }
  ~FixedVector() noexcept { alloc.deallocate(v, last - v); }

  FixedVector(const FixedVector &a) { // Copy constructor
    v = alloc.allocate(a.capacity());
    try {
      std::uninitialized_copy(a.begin(), a.end(), v);
    }
    catch (...) {
      alloc.deallocate(v, a.capacity()); // free memory
      throw;                             // re-throw
    }
    space = v + a.size();
    last = v + a.capacity();
  }

  FixedVector &operator=(const FixedVector &a) {
    size_type cnt = std::min(capacity(), a.size());

    T *new_v = alloc.allocate(capacity());
    try {
      std::uninitialized_copy(a.begin(), a.begin() + cnt,
                              new_v); // copy elements
    }
    catch (...) {
      alloc.deallocate(new_v, capacity()); // free memory
      throw;                               // re-throw
    }

    alloc.deallocate(v, last - v);
    v = new_v;
    space = new_v + cnt;
    last = new_v + capacity();

    return *this;
  }

  // Capacity
  size_type size() const noexcept { return space - v; }
  bool empty() const noexcept { return space == v; }
  size_type capacity() const noexcept { return last - v; }

  // Element access

  reference at(size_type pos) {
    if (pos >= size() || pos < 0) {
      throw std::out_of_range("Out of Range error");
    }

    return v[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size() || pos < 0) {
      throw std::out_of_range("Out of Range error");
    }

    return v[pos];
  }

  reference operator[](size_type pos) { return at(pos); }
  const_reference operator[](size_type pos) const { return at(pos); }
  reference front() { return at(0); }
  const_reference front() const { return at(0); }
  reference back() { return at(size() - 1); }
  const_reference back() const { return at(size() - 1); }

  // Iterators
  iterator begin() noexcept { return iterator(v); }
  const_iterator begin() const noexcept { return iterator(v); }
  iterator end() noexcept { return iterator(space); }
  const_iterator end() const noexcept { return iterator(space); }
  reverse_iterator rbegin() noexcept { return reverse_iterator(space - 1); }
  const_reverse_iterator rbegin() const noexcept {
    return reverse_iterator(space - 1);
  }
  reverse_iterator rend() noexcept { return reverse_iterator(v - 1); }
  const_reverse_iterator rend() const noexcept {
    return reverse_iterator(v - 1);
  }

  // Modifiers

  void push_back(const T &value) { // strong exception-safety
    if (space == last) {
      throw std::length_error("Reach the maximum size");
    }
    new (space) T(value); // place a copy of x in *space
    ++space;
  }

  void pop_back() {
    if (empty()) {
      throw std::length_error("It is empty");
    }

    space->~T();
    --space;
  }

  void clear() noexcept {
    while (!empty()) {
      pop_back(); // nothrow since !empty()
    }
  }

private:
  T *v;     // start of allocation
  T *space; // end of elements, start of space allocated for possible expansion
  T *last;  // end of allocated space
  A alloc;  // allocator
};

#endif // _FIXED_VECTOR_H_
