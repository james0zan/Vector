#include "fixed_vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_SIZE 10

void TestAccessors() {
  FixedVector<int> v(MAX_SIZE);

  // Test front()
  v.push_back(0);
  assert(v.front() == 0);

  // Test back()
  for (int i = 1; i < MAX_SIZE; i++) {
    v.push_back(i);
    assert(v.back() == i);
  }

  // Test at() and []
  for (int i = 0; i < MAX_SIZE; i++) {
    v[i] = i;
    assert(v.at(i) == i);

    v.at(i) = i + 1;
    assert(v[i] == i + 1);
  }

  // Test range errors
  try {
    v.at(-1);
    assert(false && "Should be out of range");
  }
  catch (...) {
  }
  try {
    v[MAX_SIZE];
    assert(false && "Should be out of range");
  }
  catch (...) {
  }
}

void TestIterators() {
  FixedVector<int> v(MAX_SIZE);
  for (int i = 0; i < MAX_SIZE; i++) {
    v.push_back(i);
  }

  // begin() and end()
  int value = 0;
  for (FixedVector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    assert((*it) == value);
    ++value;
  }

  // rbegin() and rend()
  int value_2 = MAX_SIZE - 1;
  for (FixedVector<int>::reverse_iterator it = v.rbegin(); it != v.rend();
       ++it) {
    assert((*it) == value_2);
    --value_2;
  }
}

void TestModifiers() {
  FixedVector<int> v(MAX_SIZE);

  // Test range errors
  try {
    v.pop_back();
    assert(false && "Should be out of range");
  }
  catch (...) {
  }

  for (int i = 0; i < MAX_SIZE; i++) {
    v.push_back(i);
  }

  // Test range errors
  size_t old_size = v.size();
  try {
    v.push_back(MAX_SIZE);
    assert(false && "Should be out of range");
  }
  catch (...) {
    assert(v.size() == old_size); // Check the strong exception-safety gusrantee
  }
}

void TestCopy() {
  FixedVector<int> v(MAX_SIZE);
  for (int i = 0; i < MAX_SIZE; i++) {
    v.push_back(i);
  }

  FixedVector<int> v2 = v;
  assert(v2.size() == v.size());
  for (int i = 0; i < MAX_SIZE; i++) {
    assert(v2[i] == i);
  }

  FixedVector<int> v3(MAX_SIZE);
  v3 = v;
  assert(v3.size() == v.size());
  for (int i = 0; i < MAX_SIZE; i++) {
    assert(v3[i] == i);
  }
}

void TestOther() {
  FixedVector<int> v(MAX_SIZE);
  assert(v.empty());
  assert(v.capacity() == MAX_SIZE);

  for (int i = 0; i < MAX_SIZE; i++) {
    v.push_back(i);

    assert(!v.empty());
    assert(v.size() == i + 1);
    assert(v.capacity() == MAX_SIZE);
  }

  v.clear();
  assert(v.empty());
  assert(v.capacity() == MAX_SIZE);
}

int main() {
  TestAccessors();
  TestIterators();
  TestModifiers();
  TestCopy();
  TestOther();
}
