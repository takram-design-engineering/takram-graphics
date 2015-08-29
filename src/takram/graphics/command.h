//
//  takram/graphics/command.h
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
#ifndef TAKRAM_GRAPHICS_COMMAND_H_
#define TAKRAM_GRAPHICS_COMMAND_H_

#include <cassert>
#include <ostream>

#include "takram/graphics/command_type.h"
#include "takram/math/promotion.h"
#include "takram/math/vector.h"

namespace takram {
namespace graphics {

template <class T, int D>
class Command final {
 public:
  using Type = T;
  static constexpr const int dimensions = D;

 public:
  explicit Command(CommandType type);
  Command(CommandType type, const Vec2<T>& point);
  Command(CommandType type, const Vec2<T>& control, const Vec2<T>& point);
  Command(CommandType type,
          const Vec2<T>& control,
          const Vec2<T>& point,
          math::Promote<T> weight);
  Command(CommandType type,
          const Vec2<T>& control1,
          const Vec2<T>& control2,
          const Vec2<T>& point);

  // Copy semantics
  Command(const Command&) = default;
  Command& operator=(const Command&) = default;

  // Comparison
  bool operator==(const Command& other) const;
  bool operator!=(const Command& other) const;

  // Properties
  const CommandType& type() const { return type_; }
  CommandType& type() { return type_; }
  const Vec2<T>& control() const { return control1_; }
  Vec2<T>& control() { return control1_; }
  const Vec2<T>& control1() const { return control1_; }
  Vec2<T>& control1() { return control1_; }
  const Vec2<T>& control2() const { return control2_; }
  Vec2<T>& control2() { return control2_; }
  const T& weight() const { return weight_; }
  T& weight() { return weight_; }
  const Vec2<T>& point() const { return point_; }
  Vec2<T>& point() { return point_; }

 private:
  CommandType type_;
  Vec2<T> control1_;
  Vec2<T> control2_;
  math::Promote<T> weight_;
  Vec2<T> point_;
};

template <class T>
using Command2 = Command<T, 2>;
template <class T>
using Command3 = Command<T, 3>;

using Command2i = Command2<int>;
using Command2f = Command2<float>;
using Command2d = Command2<double>;

using Command3i = Command3<int>;
using Command3f = Command3<float>;
using Command3d = Command3<double>;

#pragma mark -

template <class T, int D>
inline Command<T, D>::Command(CommandType type) : type_(type), weight_() {}

template <class T, int D>
inline Command<T, D>::Command(CommandType type, const Vec2<T>& point)
    : type_(type),
      weight_(),
      point_(point) {}

template <class T, int D>
inline Command<T, D>::Command(CommandType type,
                              const Vec2<T>& control,
                              const Vec2<T>& point)
    : type_(type),
      control1_(control),
      weight_(),
      point_(point) {}

template <class T, int D>
inline Command<T, D>::Command(CommandType type,
                              const Vec2<T>& control,
                              const Vec2<T>& point,
                              math::Promote<T> weight)
    : type_(type),
      control1_(control),
      weight_(weight),
      point_(point) {}

template <class T, int D>
inline Command<T, D>::Command(CommandType type,
                              const Vec2<T>& control1,
                              const Vec2<T>& control2,
                              const Vec2<T>& point)
    : type_(type),
      control1_(control1),
      control2_(control2),
      weight_(),
      point_(point) {}

#pragma mark Comparison

template <class T, int D>
inline bool Command<T, D>::operator==(const Command& other) const {
  return (type_ == other.type_ &&
          control1_ == other.control1_ &&
          control2_ == other.control2_ &&
          weight_ == other.weight_ &&
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
      os << " "  << other.point();
      break;
    case CommandType::QUADRATIC:
      os << " "  << other.control();
      os << " " << other.point();
      break;
    case CommandType::CONIC:
      os << " "  << other.control();
      os << " " << other.point();
      os << " " << other.weight();
      break;
    case CommandType::CUBIC:
      os << " "  << other.control1();
      os << " " << other.control2();
      os << " " << other.point();
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
