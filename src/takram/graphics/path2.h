//
//  takram/graphics/path2.h
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
#ifndef TAKRAM_GRAPHICS_PATH2_H_
#define TAKRAM_GRAPHICS_PATH2_H_

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <list>
#include <type_traits>
#include <utility>

#include "takram/graphics/command.h"
#include "takram/graphics/conic.h"
#include "takram/graphics/path_direction.h"
#include "takram/math/promotion.h"
#include "takram/math/rectangle.h"
#include "takram/math/vector.h"

namespace takram {
namespace graphics {

extern void *enabler;

template <class T, int D>
class Path;

template <class T>
using Path2 = Path<T, 2>;

template <class T>
class Path<T, 2> final {
 public:
  using Type = T;
  using Command = Command2<T>;
  using Iterator = typename std::list<Command2<T>>::iterator;
  using ConstIterator = typename std::list<Command2<T>>::const_iterator;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  static constexpr const int dimensions = 2;

 private:
  // Provided for reverse()
  struct Holder {
    explicit Holder(const Vec2<T>& vector) : vector(vector) {}
    explicit Holder(const math::Promote<T>& scalar) : scalar(scalar) {}
    operator const Vec2<T>&() const { return vector; }
    operator const math::Promote<T>&() const { return scalar; }
    union {
      Vec2<T> vector;
      math::Promote<T> scalar;
    };
  };

 public:
  Path();
  explicit Path(const std::list<Command2<T>>& commands);

  // Copy semantics
  Path(const Path&) = default;
  Path& operator=(const Path&) = default;

  // Mutators
  void set(const std::list<Command2<T>>& commands);
  void reset();

  // Comparison
  bool operator==(const Path& other) const;
  bool operator!=(const Path& other) const;

  // Attributes
  bool empty() const { return commands_.empty(); }
  bool closed() const;
  std::size_t size() const { return commands_.size(); }
  Rect2<T> bounds() const;

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

  // Commands
  const std::list<Command2<T>>& commands() const { return commands_; }
  std::list<Command2<T>>& commands() { return commands_; }

  // Direction
  PathDirection direction() const;
  Path& reverse();
  Path reversed() const;

  // Conversion
  bool convertConicsToQuadratics();
  bool convertConicsToQuadratics(math::Promote<T> tolerance);

  // Element access
  Command2<T>& operator[](int index) { return at(index); }
  const Command2<T>& operator[](int index) const { return at(index); }
  Command2<T>& at(int index);
  const Command2<T>& at(int index) const;
  Command2<T>& front() { return commands_.front(); }
  const Command2<T>& front() const { return commands_.front(); }
  Command2<T>& back() { return commands_.back(); }
  const Command2<T>& back() const { return commands_.back(); }

  // Iterator
  Iterator begin() { return std::begin(commands_); }
  ConstIterator begin() const { return std::begin(commands_); }
  Iterator end() { return std::end(commands_); }
  ConstIterator end() const { return std::end(commands_); }
  ReverseIterator rbegin() { return ReverseIterator(begin()); }
  ConstReverseIterator rbegin() const { return ConstReverseIterator(begin()); }
  ReverseIterator rend() { return ReverseIterator(end()); }
  ConstReverseIterator rend() const { return ConstReverseIterator(end()); }

 private:
  template <
    class Method, class... Args,
    std::enable_if_t<std::is_member_pointer<Method>::value> *& = enabler
  >
  bool convertConicsToQuadratics(Method method, Args&&... args);

 private:
  std::list<Command2<T>> commands_;
  mutable PathDirection direction_;
};

using Path2i = Path2<int>;
using Path2f = Path2<float>;
using Path2d = Path2<double>;

#pragma mark -

template <class T>
inline Path2<T>::Path() : direction_(PathDirection::UNDEFINED) {}

template <class T>
inline Path2<T>::Path(const std::list<Command2<T>>& commands)
    : commands_(commands),
      direction_(PathDirection::UNDEFINED) {}

#pragma mark Mutators

template <class T>
inline void Path2<T>::set(const std::list<Command2<T>>& commands) {
  commands_ = commands;
}

template <class T>
inline void Path2<T>::reset() {
  commands_.clear();
}

#pragma mark Comparison

template <class T>
inline bool Path2<T>::operator==(const Path& other) const {
  return commands_ == other.commands_;
}

template <class T>
inline bool Path2<T>::operator!=(const Path& other) const {
  return !operator==(other);
}

#pragma mark Attributes

template <class T>
inline bool Path2<T>::closed() const {
  return !commands_.empty() && commands_.back().type() == CommandType::CLOSE;
}

template <class T>
inline Rect2<T> Path2<T>::bounds() const {
  T min_x = std::numeric_limits<T>::max();
  T min_y = std::numeric_limits<T>::max();
  T max_x = std::numeric_limits<T>::lowest();
  T max_y = std::numeric_limits<T>::lowest();
  for (const auto& command : *this) {
    switch (command.type()) {
      case CommandType::CUBIC:
        if (command.control2().x < min_x) min_x = command.control2().x;
        if (command.control2().y < min_y) min_y = command.control2().y;
        if (command.control2().x > max_x) max_x = command.control2().x;
        if (command.control2().y > max_y) max_y = command.control2().y;
        // Pass through
      case CommandType::CONIC:
      case CommandType::QUADRATIC:
        if (command.control().x < min_x) min_x = command.control().x;
        if (command.control().y < min_y) min_y = command.control().y;
        if (command.control().x > max_x) max_x = command.control().x;
        if (command.control().y > max_y) max_y = command.control().y;
        // Pass through
      case CommandType::LINE:
      case CommandType::MOVE:
        if (command.point().x < min_x) min_x = command.point().x;
        if (command.point().y < min_y) min_y = command.point().y;
        if (command.point().x > max_x) max_x = command.point().x;
        if (command.point().y > max_y) max_y = command.point().y;
        break;
      case CommandType::CLOSE:
        break;
      default:
        assert(false);
        break;
    }
  }
  if (min_x == std::numeric_limits<T>::max()) min_x = T();
  if (min_y == std::numeric_limits<T>::max()) min_y = T();
  if (max_x == std::numeric_limits<T>::lowest()) max_x = T();
  if (max_y == std::numeric_limits<T>::lowest()) max_y = T();
  return Rect2<T>(Vec2<T>(min_x, min_y), Vec2<T>(max_x, max_y));
}

#pragma mark Adding commands

template <class T>
inline void Path2<T>::close() {
  if (!closed()) {
    commands_.emplace_back(CommandType::CLOSE);
  }
}

template <class T>
inline void Path2<T>::moveTo(T x, T y) {
  moveTo(Vec2<T>(x, y));
}

template <class T>
inline void Path2<T>::moveTo(const Vec2<T>& point) {
  commands_.clear();
  commands_.emplace_back(CommandType::MOVE, point);
}

template <class T>
inline void Path2<T>::lineTo(T x, T y) {
  lineTo(Vec2<T>(x, y));
}

template <class T>
inline void Path2<T>::lineTo(const Vec2<T>& point) {
  if (commands_.empty()) {
    moveTo(point);
  } else {
    commands_.emplace_back(CommandType::LINE, point);
    if (point == commands_.front().point()) {
      close();
    }
  }
}

template <class T>
inline void Path2<T>::quadraticTo(T cx, T cy, T x, T y) {
  quadraticTo(Vec2<T>(cx, cy), Vec2<T>(x, y));
}

template <class T>
inline void Path2<T>::quadraticTo(const Vec2<T>& control,
                                  const Vec2<T>& point) {
  if (commands_.empty()) {
    moveTo(point);
  } else {
    commands_.emplace_back(CommandType::QUADRATIC, control, point);
    if (point == commands_.front().point()) {
      close();
    }
  }
}

template <class T>
inline void Path2<T>::conicTo(T cx, T cy, T x, T y, math::Promote<T> weight) {
  conicTo(Vec2<T>(cx, cy), Vec2<T>(x, y), weight);
}

template <class T>
inline void Path2<T>::conicTo(const Vec2<T>& control,
                              const Vec2<T>& point,
                              math::Promote<T> weight) {
  if (commands_.empty()) {
    moveTo(point);
  } else {
    commands_.emplace_back(CommandType::CONIC, control, point, weight);
    if (point == commands_.front().point()) {
      close();
    }
  }
}

template <class T>
inline void Path2<T>::cubicTo(T cx1, T cy1, T cx2, T cy2, T x, T y) {
  cubicTo(Vec2<T>(cx1, cy1), Vec2<T>(cx2, cy2), Vec2<T>(x, y));
}

template <class T>
inline void Path2<T>::cubicTo(const Vec2<T>& control1,
                              const Vec2<T>& control2,
                              const Vec2<T>& point) {
  if (commands_.empty()) {
    moveTo(point);
  } else {
    commands_.emplace_back(CommandType::CUBIC, control1, control2, point);
    if (point == commands_.front().point()) {
      close();
    }
  }
}

#pragma mark Direction

template <class T>
inline PathDirection Path2<T>::direction() const {
  if (commands_.size() < 3) {
    return PathDirection::UNDEFINED;
  }
  T sum{};
  auto first = std::begin(commands_);
  auto second = std::next(first);
  for (; second != std::end(commands_); ++first, ++second) {
    switch (second->type()) {
      case CommandType::LINE:
      case CommandType::QUADRATIC:
      case CommandType::CONIC:
      case CommandType::CUBIC:
        sum += first->point().cross(second->point());
        break;
      case CommandType::CLOSE:
        sum += second->point().cross(commands_.front().point());
        break;
      default:
        assert(false);
        break;
    }
  }
  if (!sum) {
    return PathDirection::UNDEFINED;
  }
  return sum < 0 ? PathDirection::COUNTER_CLOCKWISE : PathDirection::CLOCKWISE;
}

template <class T>
inline Path2<T>& Path2<T>::reverse() {
  if (commands_.empty()) {
    return *this;
  }
  std::list<Holder> holders;
  for (auto& command : commands_) {
    switch (command.type()) {
      case CommandType::MOVE:
      case CommandType::LINE:
        holders.emplace_back(command.point());
        break;
      case CommandType::QUADRATIC:
        holders.emplace_back(command.control());
        holders.emplace_back(command.point());
        break;
      case CommandType::CONIC:
        holders.emplace_back(command.control());
        holders.emplace_back(command.weight());
        holders.emplace_back(command.point());
        break;
      case CommandType::CUBIC:
        holders.emplace_back(command.control1());
        holders.emplace_back(command.control2());
        holders.emplace_back(command.point());
        break;
      case CommandType::CLOSE:
        break;
      default:
        assert(false);
        break;
    }
  }
  if (commands_.back().type() == CommandType::CLOSE) {
    std::reverse(std::next(std::begin(commands_)),
                 std::prev(std::end(commands_)));
  } else {
    std::reverse(std::next(std::begin(commands_)),
                 std::end(commands_));
  }
  std::reverse(std::begin(holders), std::end(holders));
  auto itr = std::begin(holders);
  for (auto& command : commands_) {
    switch (command.type()) {
      case CommandType::MOVE:
      case CommandType::LINE:
        command.point() = *(itr++);
        break;
      case CommandType::QUADRATIC:
        command.control() = *(itr++);
        command.point() = *(itr++);
        break;
      case CommandType::CONIC:
        command.weight() = *(itr++);
        command.control() = *(itr++);
        command.point() = *(itr++);
        break;
      case CommandType::CUBIC:
        command.control1() = *(itr++);
        command.control2() = *(itr++);
        command.point() = *(itr++);
        break;
      case CommandType::CLOSE:
        break;
      default:
        assert(false);
        break;
    }
  }
  assert(itr == std::end(holders));
  return *this;
}

template <class T>
inline Path2<T> Path2<T>::reversed() const {
  return std::move(Path(*this).reverse());
}

#pragma mark Conversion

template <class T>
inline bool Path2<T>::convertConicsToQuadratics() {
  using Method = std::vector<Vec2<T>> (Conic2<T>::*)(void) const;
  return convertConicsToQuadratics(static_cast<Method>(&Conic2<T>::quadratics));
}

template <class T>
inline bool Path2<T>::convertConicsToQuadratics(math::Promote<T> tolerance) {
  using Method = std::vector<Vec2<T>> (Conic2<T>::*)(math::Promote<T>) const;
  return convertConicsToQuadratics(static_cast<Method>(&Conic2<T>::quadratics),
                                   tolerance);
}

template <class T>
template <
  class Method, class... Args,
  std::enable_if_t<std::is_member_pointer<Method>::value> *&
>
inline bool Path2<T>::convertConicsToQuadratics(Method method, Args&&... args) {
  assert(method);
  bool changed{};
  const auto end = std::end(commands_);
  auto current = std::begin(commands_);
  auto previous = current++;
  while (current != end) {
    if (current->type() != CommandType::CONIC) {
      previous = current++;
      continue;
    }
    const Conic2<T> conic(previous->point(),
                          current->control(),
                          current->point(),
                          current->weight());
    const auto points = (conic.*method)(args...);
    current = commands_.erase(current);
    for (auto itr = std::begin(points); itr != std::end(points); ++itr) {
      current = commands_.emplace(current, CommandType::QUADRATIC,
                                  *itr, *(++itr));
      previous = current++;
    }
    changed = true;
  }
  return changed;
}

#pragma mark Element access

template <class T>
inline Command2<T>& Path2<T>::at(int index) {
  auto itr = std::begin(commands_);
  std::advance(itr, index);
  return *itr;
}

template <class T>
inline const Command2<T>& Path2<T>::at(int index) const {
  auto itr = std::begin(commands_);
  std::advance(itr, index);
  return *itr;
}

}  // namespace graphics

namespace gfx = graphics;

using graphics::Path;
using graphics::Path2;
using graphics::Path2i;
using graphics::Path2f;
using graphics::Path2d;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_PATH2_H_
