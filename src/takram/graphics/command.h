//
//  takram/graphics/command.h
//
//  MIT License
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
#ifndef TAKRAM_GRAPHICS_COMMAND_H_
#define TAKRAM_GRAPHICS_COMMAND_H_

#include <ostream>

#include "takram/graphics/command_type.h"
#include "takram/math/vector.h"

namespace takram {
namespace graphics {

template <class T, int D>
class Command final {
 public:
  using Type = T;
  using Point = Vec2<T>;
  static constexpr const int dimensions = D;

 public:
  explicit Command(CommandType type);
  Command(CommandType type, const Point& point);
  Command(CommandType type, const Point& control, const Point& point);
  Command(CommandType type, const Point& control, const Point& point, T weight);
  Command(CommandType type,
          const Point& control1,
          const Point& control2,
          const Point& point);

  // Copy semantics
  Command(const Command&) = default;
  Command& operator=(const Command&) = default;

  // Comparison
  bool operator==(const Command& other) const;
  bool operator!=(const Command& other) const;

  // Properties
  CommandType type() const { return type_; }
  const Point& control() const { return control_; }
  Point& control() { return control_; }
  const Point& control1() const { return control_; }
  Point& control1() { return control_; }
  const Point& control2() const { return control_or_weight_; }
  Point& control2() { return control_or_weight_; }
  const T& weight() const { return control_or_weight_.front(); }
  T& weight() { return control_or_weight_.front(); }
  const Point& point() const { return point_; }
  Point& point() { return point_; }

 private:
  CommandType type_;
  Point control_;
  Point control_or_weight_;
  Point point_;
};

#pragma mark -

template <class T, int D>
inline Command<T, D>::Command(CommandType type) : type_(type) {}

template <class T, int D>
inline Command<T, D>::Command(CommandType type, const Point& point)
    : type_(type),
      point_(point) {}

template <class T, int D>
inline Command<T, D>::Command(CommandType type,
                              const Point& control,
                              const Point& point)
    : type_(type),
      control_(control),
      point_(point) {}

template <class T, int D>
inline Command<T, D>::Command(CommandType type,
                              const Point& control,
                              const Point& point,
                              T weight)
    : type_(type),
      control_(control),
      control_or_weight_(weight),
      point_(point) {}

template <class T, int D>
inline Command<T, D>::Command(CommandType type,
                              const Point& control1,
                              const Point& control2,
                              const Point& point)
    : type_(type),
      control_(control1),
      control_or_weight_(control2),
      point_(point) {}

#pragma mark Comparison

template <class T, int D>
inline bool Command<T, D>::operator==(const Command& other) const {
  return (type_ == other.type_ &&
          control_ == other.control_ &&
          control_or_weight_ == other.control_or_weight_ &&
          point_ == other.point_);
}

template <class T, int D>
inline bool Command<T, D>::operator!=(const Command& other) const {
  return !operator==(other);
}

#pragma mark Stream

template <class T, int D>
inline std::ostream& operator<<(std::ostream& os, const Command<T, D>& other) {
  os << "( " << other.type();
  switch (other.type()) {
    case CommandType::MOVE:
    case CommandType::LINE:
      os << other.point();
      break;
    case CommandType::QUADRATIC:
      os << other.control() << " " << other.point();
      break;
    case CommandType::CONIC:
      os << other.control() << " " << other.point() << " " << other.weight();
      break;
    case CommandType::CUBIC:
      os << other.control1() << " " << other.control2() << " " << other.point();
      break;
    case CommandType::CLOSE:
      break;
    default:
      assert(false);
      break;
  }
  return os << " )";
}

}  // namespace graphics

namespace gfx = graphics;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_COMMAND_H_
