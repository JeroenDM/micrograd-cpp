#pragma once

#include <functional>
#include <memory>
#include <string>
#include <type_traits>

namespace micrograd {

template <typename T>
concept Number = std::is_arithmetic_v<T>;

class _Value;
using ValueRef = std::shared_ptr<_Value>;

std::shared_ptr<_Value> Value(Number auto x) {
  return std::make_shared<_Value>(static_cast<double>(x));
}

class _Value {
public:
  double data;
  double grad;
  std::vector<ValueRef> children;
  char op;
  std::string label;

  _Value(double x)
      : data{x}, grad{0.0}, children{}, op{'0'}, label{std::to_string(x)} {}
};

// Fundamental operators
ValueRef operator+(ValueRef x, ValueRef y) {
  auto out = std::make_shared<_Value>(x->data + y->data);
  out->children = {x, y};
  out->op = '+';
  return out;
}

ValueRef operator*(ValueRef x, ValueRef y) {
  auto out = std::make_shared<_Value>(x->data * y->data);
  out->children = {x, y};
  out->op = '*';
  return out;
}

ValueRef relu(ValueRef x) {
  auto out = Value(x->data < 0 ? 0.0 : x->data);
  out->children = {x};
  out->op = 'r';
  return out;
}

// Derived operators
ValueRef operator+(Number auto x, ValueRef y) { return Value(x) + y; }
ValueRef operator+(ValueRef x, Number auto y) { return x + Value(y); }
ValueRef operator*(Number auto x, ValueRef y) { return Value(x) * y; }
ValueRef operator*(ValueRef x, Number auto y) { return x * Value(y); }

// utils
using Id = std::uintptr_t;
Id id(ValueRef x) { return reinterpret_cast<std::uintptr_t>(x.get()); }

} // namespace micrograd
