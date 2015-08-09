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

#include "takram/math/vector.h"

namespace takram {
namespace graphics {

template <class T, int D>
class Command final {
 public:
  using Type = T;
  using Point = Vector2<T>;
  static constexpr const int dimensions = D;

 public:
  enum class Kind {
    MOVE,
    LINE,
    QUADRATIC,
    CUBIC,
    CLOSE
  };

 public:
  explicit Command(Kind kind);
  Command(Kind kind, const Point& point);
  Command(Kind kind, const Point& control, const Point& point);
  Command(Kind kind,
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
  Kind kind() const { return kind_; }
  const Point& control() const { return control1_; }
  Point& control() { return control1_; }
  const Point& control1() const { return control1_; }
  Point& control1() { return control1_; }
  const Point& control2() const { return control2_; }
  Point& control2() { return control2_; }
  const Point& point() const { return point_; }
  Point& point() { return point_; }

 private:
  Kind kind_;
  Point control1_;
  Point control2_;
  Point point_;
};

#pragma mark -

template <class T, int D>
inline Command<T, D>::Command(Kind kind) : kind_(kind) {}

template <class T, int D>
inline Command<T, D>::Command(Kind kind, const Point& point)
    : kind_(kind),
      point_(point) {}

template <class T, int D>
inline Command<T, D>::Command(Kind kind,
                              const Point& control,
                              const Point& point)
    : kind_(kind),
      control1_(control),
      point_(point) {}

template <class T, int D>
inline Command<T, D>::Command(Kind kind,
                              const Point& control1,
                              const Point& control2,
                              const Point& point)
    : kind_(kind),
      control1_(control1),
      control2_(control2),
      point_(point) {}

#pragma mark Comparison

template <class T, int D>
inline bool Command<T, D>::operator==(const Command& other) const {
  return (kind_ == other.kind_ &&
          control1_ == other.control1_ &&
          control2_ == other.control2_ &&
          point_ == other.point_);
}

template <class T, int D>
inline bool Command<T, D>::operator!=(const Command& other) const {
  return !operator==(other);
}

}  // namespace graphics

namespace gfx = graphics;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_COMMAND_H_
