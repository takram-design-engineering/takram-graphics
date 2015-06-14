//
//  takram/graphics/contour.h
//
//  takram design engineering Confidential
//
//  Copyright (C) 2015 Shota Matsuda
//  Copyright (C) 2015 takram design engineering
//
//  All information contained herein is, and remains the property of takram
//  design engineering and its suppliers, if any. The intellectual and
//  technical concepts contained herein are proprietary to takram design
//  engineering and its suppliers and may be covered by U.S. and Foreign
//  Patents, patents in process, and are protected by trade secret or copyright
//  law. Dissemination of this information or reproduction of this material is
//  strictly forbidden unless prior written permission is obtained from takram
//  design engineering.
//

#pragma once
#ifndef TAKRAM_GRAPHICS_CONTOUR_H_
#define TAKRAM_GRAPHICS_CONTOUR_H_

#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

#include "takram/graphics/segment.h"
#include "takram/math/rect.h"
#include "takram/math/vector.h"

namespace takram {
namespace graphics {

class Contour final {
 public:
  using Real = double;
  using Point = math::Vec2<Real>;
  using Iterator = std::vector<Segment>::iterator;
  using ConstIterator = std::vector<Segment>::const_iterator;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

 public:
  enum class Direction {
    UNDEFINED,
    CLOCKWISE,
    COUNTER_CLOCKWISE
  };

 public:
  Contour();
  explicit Contour(const std::vector<Segment>& segments);

  // Copy semantics
  Contour(const Contour& other) = default;
  Contour& operator=(const Contour& other) = default;

  // Mutators
  void set(const std::vector<Segment>& segments);
  void reset();

  // Comparison
  bool operator==(const Contour& other) const;
  bool operator!=(const Contour& other) const;

  // Attributes
  bool empty() const { return segments_.empty(); }
  std::size_t size() const { return segments_.size(); }
  math::Rect<Real> bounds() const;

  // Properties
  const std::vector<Segment>& segments() const { return segments_; }
  std::vector<Segment>& segments() { return segments_; }

  // Adding segments
  void close();
  void moveTo(Real x, Real y);
  void moveTo(const Point& point);
  void lineTo(Real x, Real y);
  void lineTo(const Point& point);
  void quadraticTo(Real cx, Real cy, Real x, Real y);
  void quadraticTo(const Point& control, const Point& point);
  void cubicTo(Real cx1, Real cy1, Real cx2, Real cy2, Real x, Real y);
  void cubicTo(const Point& control1,
               const Point& control2,
               const Point& point);

  // Direction
  Direction direction() const;
  Contour& reverse();
  Contour reversed() const;

  // Element access
  Segment& operator[](int index) { return segments_.at(index); }
  const Segment& operator[](int index) const { return segments_.at(index); }
  Segment& at(int index) { return segments_.at(index); }
  const Segment& at(int index) const { return segments_.at(index); }
  Segment& front() { return segments_.front(); }
  const Segment& front() const { return segments_.front(); }
  Segment& back() { return segments_.back(); }
  const Segment& back() const { return segments_.back(); }

  // Iterator
  Iterator begin() { return std::begin(segments_); }
  ConstIterator begin() const { return std::begin(segments_); }
  Iterator end() { return std::end(segments_); }
  ConstIterator end() const { return std::end(segments_); }
  ReverseIterator rbegin() { return ReverseIterator(begin()); }
  ConstReverseIterator rbegin() const { return ConstReverseIterator(begin()); }
  ReverseIterator rend() { return ReverseIterator(end()); }
  ConstReverseIterator rend() const { return ConstReverseIterator(end()); }

 private:
  std::vector<Segment> segments_;
  mutable Direction direction_;
};

#pragma mark -

inline Contour::Contour()
    : direction_(Direction::UNDEFINED) {}

inline Contour::Contour(const std::vector<Segment>& segments)
    : segments_(segments),
      direction_(Direction::UNDEFINED) {}

#pragma mark Mutators

inline void Contour::set(const std::vector<Segment>& segments) {
  segments_ = segments;
}

inline void Contour::reset() {
  segments_.clear();
}

#pragma mark Comparison

inline bool Contour::operator==(const Contour& other) const {
  return segments_ == other.segments_;
}

inline bool Contour::operator!=(const Contour& other) const {
  return !operator==(other);
}

#pragma mark Adding segments

inline void Contour::close() {
  if (segments_.back().type() != Segment::Type::CLOSE) {
    segments_.emplace_back(Segment::Type::CLOSE);
  }
}

inline void Contour::moveTo(Real x, Real y) {
  moveTo(Point(x, y));
}

inline void Contour::moveTo(const Point& point) {
  segments_.clear();
  segments_.emplace_back(Segment::Type::MOVE, point);
}

inline void Contour::lineTo(Real x, Real y) {
  lineTo(Point(x, y));
}

inline void Contour::lineTo(const Point& point) {
  if (segments_.empty()) {
    moveTo(point);
  } else {
    segments_.emplace_back(Segment::Type::LINE, point);
    if (point == segments_.front().point()) {
      close();
    }
  }
}

inline void Contour::quadraticTo(Real cx, Real cy, Real x, Real y) {
  quadraticTo(Point(cx, cy), Point(x, y));
}

inline void Contour::quadraticTo(const Point& control, const Point& point) {
  if (segments_.empty()) {
    moveTo(point);
  } else {
    segments_.emplace_back(Segment::Type::QUADRATIC, control, point);
    if (point == segments_.front().point()) {
      close();
    }
  }
}

inline void Contour::cubicTo(Real cx1, Real cy1,
                             Real cx2, Real cy2,
                             Real x, Real y) {
  cubicTo(Point(cx1, cy1), Point(cx2, cy2), Point(x, y));
}

inline void Contour::cubicTo(const Point& control1,
                             const Point& control2,
                             const Point& point) {
  if (segments_.empty()) {
    moveTo(point);
  } else {
    segments_.emplace_back(Segment::Type::CUBIC, control1, control2, point);
    if (point == segments_.front().point()) {
      close();
    }
  }
}

#pragma mark Direction

inline Contour Contour::reversed() const {
  return std::move(Contour(*this).reverse());
}

}  // namespace graphics

namespace gfx = graphics;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_CONTOUR_H_
