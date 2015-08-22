//
//  takram/graphics/shape2.h
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#pragma once
#ifndef TAKRAM_GRAPHICS_SHAPE2_H_
#define TAKRAM_GRAPHICS_SHAPE2_H_

#include <cstddef>
#include <list>
#include <iterator>

#include "takram/algorithm/leaf_iterator_iterator.h"
#include "takram/graphics/path.h"
#include "takram/math/promotion.h"
#include "takram/math/vector.h"

namespace takram {
namespace graphics {

template <class T, int D>
class Shape;

template <class T>
using Shape2 = Shape<T, 2>;

template <class T>
class Shape<T, 2> final {
 public:
  using Type = T;
  using Iterator = LeafIteratorIterator<
      typename std::list<Path2<T>>::iterator,
      typename Path2<T>::Iterator>;
  using ConstIterator = LeafIteratorIterator<
      typename std::list<Path2<T>>::const_iterator,
      typename Path2<T>::ConstIterator>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  static constexpr const int dimensions = 2;

 public:
  Shape() = default;
  explicit Shape(const std::list<Path2<T>>& paths);

  // Copy semantics
  Shape(const Shape&) = default;
  Shape& operator=(const Shape&) = default;

  // Mutators
  void set(const std::list<Path2<T>>& paths);
  void reset();

  // Comparison
  bool operator==(const Shape& other) const;
  bool operator!=(const Shape& other) const;

  // Attributes
  bool empty() const { return paths_.empty(); }
  std::size_t size() const { return paths_.size(); }

  // Adding commands
  void close();
  void moveTo(T x, T y);
  void moveTo(const Vec2<T>& point);
  void lineTo(T x, T y);
  void lineTo(const Vec2<T>& point);
  void quadraticTo(T cx, T cy, T x, T y);
  void quadraticTo(const Vec2<T>& control, const Vec2<T>& point);
  void conicTo(T cx, T cy, T x, T y, math::Promote<T> weight);
  void conicTo(const Vec2<T>& control,
               const Vec2<T>& point,
               math::Promote<T> weight);
  void cubicTo(T cx1, T cy1, T cx2, T cy2, T x, T y);
  void cubicTo(const Vec2<T>& control1,
               const Vec2<T>& control2,
               const Vec2<T>& point);

  // Paths
  const std::list<Path2<T>>& paths() const { return paths_; }
  std::list<Path2<T>>& paths() { return paths_; }

  // Conversion
  bool convertConicsToQuadratics();
  bool convertConicsToQuadratics(math::Promote<T> tolerance);

  // Element access
  Path2<T>& operator[](int index) { return at(index); }
  const Path2<T>& operator[](int index) const { return at(index); }
  Path2<T>& at(int index);
  const Path2<T>& at(int index) const;
  Path2<T>& front() { return paths_.front(); }
  const Path2<T>& front() const { return paths_.front(); }
  Path2<T>& back() { return paths_.back(); }
  const Path2<T>& back() const { return paths_.back(); }

  // Iterator
  Iterator begin();
  ConstIterator begin() const;
  Iterator end();
  ConstIterator end() const;
  ReverseIterator rbegin() { return ReverseIterator(begin()); }
  ConstReverseIterator rbegin() const { return ConstReverseIterator(begin()); }
  ReverseIterator rend() { return ReverseIterator(end()); }
  ConstReverseIterator rend() const { return ConstReverseIterator(end()); }

 private:
  std::list<Path2<T>> paths_;
};

using Shape2i = Shape2<int>;
using Shape2f = Shape2<float>;
using Shape2d = Shape2<double>;

#pragma mark -

template <class T>
inline Shape2<T>::Shape(const std::list<Path2<T>>& paths) : paths_(paths) {}

#pragma mark Mutators

template <class T>
inline void Shape2<T>::set(const std::list<Path2<T>>& paths) {
  paths_ = paths;
}

template <class T>
inline void Shape2<T>::reset() {
  paths_.clear();
}

#pragma mark Comparison

template <class T>
inline bool Shape2<T>::operator==(const Shape& other) const {
  return paths_ == other.paths_;
}

template <class T>
inline bool Shape2<T>::operator!=(const Shape& other) const {
  return !operator==(other);
}

#pragma mark Adding commands

template <class T>
inline void Shape2<T>::close() {
  if (!paths_.empty()) {
    paths_.back().close();
  }
}

template <class T>
inline void Shape2<T>::moveTo(T x, T y) {
  paths_.emplace_back();
  paths_.back().moveTo(x, y);
}

template <class T>
inline void Shape2<T>::moveTo(const Vec2<T>& point) {
  paths_.emplace_back();
  paths_.back().moveTo(point);
}

template <class T>
inline void Shape2<T>::lineTo(T x, T y) {
  if (!paths_.empty()) {
    paths_.back().lineTo(x, y);
  }
}

template <class T>
inline void Shape2<T>::lineTo(const Vec2<T>& point) {
  if (!paths_.empty()) {
    paths_.back().lineTo(point);
  }
}

template <class T>
inline void Shape2<T>::quadraticTo(T cx, T cy, T x, T y) {
  if (!paths_.empty()) {
    paths_.back().quadraticTo(cx, cy, x, y);
  }
}

template <class T>
inline void Shape2<T>::quadraticTo(const Vec2<T>& control,
                                   const Vec2<T>& point) {
  if (!paths_.empty()) {
    paths_.back().quadraticTo(control, point);
  }
}

template <class T>
inline void Shape2<T>::conicTo(T cx, T cy, T x, T y, math::Promote<T> weight) {
  if (!paths_.empty()) {
    paths_.back().conicTo(cx, cy, x, y, weight);
  }
}

template <class T>
inline void Shape2<T>::conicTo(const Vec2<T>& control,
                               const Vec2<T>& point,
                               math::Promote<T> weight) {
  if (!paths_.empty()) {
    paths_.back().conicTo(control, point, weight);
  }
}

template <class T>
inline void Shape2<T>::cubicTo(T cx1, T cy1, T cx2, T cy2, T x, T y) {
  if (!paths_.empty()) {
    paths_.back().cubicTo(cx1, cy1, cx2, cy2, x, y);
  }
}

template <class T>
inline void Shape2<T>::cubicTo(const Vec2<T>& control1,
                               const Vec2<T>& control2,
                               const Vec2<T>& point) {
  if (!paths_.empty()) {
    paths_.back().cubicTo(control1, control2, point);
  }
}

#pragma mark Conversion

template <class T>
inline bool Shape2<T>::convertConicsToQuadratics() {
  bool changed{};
  for (auto& path : paths_) {
    changed |= path.convertConicsToQuadratics();
  }
  return changed;
}

template <class T>
inline bool Shape2<T>::convertConicsToQuadratics(math::Promote<T> tolerance) {
  bool changed{};
  for (auto& path : paths_) {
    changed |= path.convertConicsToQuadratics(tolerance);
  }
  return changed;
}

#pragma mark Element access

template <class T>
inline Path2<T>& Shape2<T>::at(int index) {
  auto itr = std::begin(paths_);
  std::advance(itr, index);
  return *itr;
}

template <class T>
inline const Path2<T>& Shape2<T>::at(int index) const {
  auto itr = std::begin(paths_);
  std::advance(itr, index);
  return *itr;
}

#pragma mark Iterator

template <class T>
inline typename Shape2<T>::Iterator Shape2<T>::begin() {
  return Iterator(std::begin(paths_), std::end(paths_));
}

template <class T>
inline typename Shape2<T>::ConstIterator Shape2<T>::begin() const {
  return ConstIterator(std::begin(paths_), std::end(paths_));
}

template <class T>
inline typename Shape2<T>::Iterator Shape2<T>::end() {
  return Iterator(std::end(paths_), std::end(paths_));
}

template <class T>
inline typename Shape2<T>::ConstIterator Shape2<T>::end() const {
  return ConstIterator(std::end(paths_), std::end(paths_));
}

}  // namespace graphics

namespace gfx = graphics;

using graphics::Shape;
using graphics::Shape2;
using graphics::Shape2i;
using graphics::Shape2f;
using graphics::Shape2d;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_SHAPE2_H_
