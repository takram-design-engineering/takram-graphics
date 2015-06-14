//
//  takram/graphics/contour.cc
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

#include "takram/graphics/contour.h"

#include <algorithm>
#include <iterator>
#include <limits>
#include <list>

#include "takram/graphics/segment.h"
#include "takram/math/rect.h"

namespace takram {
namespace graphics {

#pragma mark Attributes

math::Rect<Contour::Real> Contour::bounds() const {
  Real min_x = std::numeric_limits<Real>::max();
  Real min_y = std::numeric_limits<Real>::max();
  Real max_x = std::numeric_limits<Real>::lowest();
  Real max_y = std::numeric_limits<Real>::lowest();
  for (const auto& segment : *this) {
    switch (segment.type()) {
      case Segment::Type::CUBIC:
        if (segment.control2().x < min_x) {
          min_x = segment.control2().x;
        }
        if (segment.control2().y < min_y) {
          min_y = segment.control2().y;
        }
        if (segment.control2().x > max_x) {
          max_x = segment.control2().x;
        }
        if (segment.control2().y > max_y) {
          max_y = segment.control2().y;
        }
        // Pass through
      case Segment::Type::QUADRATIC:
        if (segment.control().x < min_x) {
          min_x = segment.control().x;
        }
        if (segment.control().y < min_y) {
          min_y = segment.control().y;
        }
        if (segment.control().x > max_x) {
          max_x = segment.control().x;
        }
        if (segment.control().y > max_y) {
          max_y = segment.control().y;
        }
        // Pass through
      case Segment::Type::MOVE:
      case Segment::Type::LINE:
        if (segment.point().x < min_x) {
          min_x = segment.point().x;
        }
        if (segment.point().y < min_y) {
          min_y = segment.point().y;
        }
        if (segment.point().x > max_x) {
          max_x = segment.point().x;
        }
        if (segment.point().y > max_y) {
          max_y = segment.point().y;
        }
        break;
      default:
        break;
    }
  }
  if (min_x == std::numeric_limits<Real>::max()) {
    min_x = Real();
  }
  if (min_y == std::numeric_limits<Real>::max()) {
    min_y = Real();
  }
  if (max_x == std::numeric_limits<Real>::lowest()) {
    max_x = Real();
  }
  if (max_y == std::numeric_limits<Real>::lowest()) {
    max_y = Real();
  }
  return math::Rect<Real>(Point(min_x, min_y), Point(max_x, max_y));
}

#pragma mark Direction

Contour::Direction Contour::direction() const {
  if (segments_.size() < 3) {
    return Direction::UNDEFINED;
  }
  Real sum{};
  auto first = std::begin(segments_);
  auto second = std::next(first);
  for (; second != std::end(segments_); ++first, ++second) {
    switch (second->type()) {
      case Segment::Type::LINE:
      case Segment::Type::QUADRATIC:
      case Segment::Type::CUBIC:
        sum += first->point().cross(second->point());
        break;
      case Segment::Type::CLOSE:
        sum += second->point().cross(segments_.front().point());
        break;
      default:
        assert(false);
        break;
    }
  }
  return sum < 0.0 ? Direction::COUNTER_CLOCKWISE : Direction::CLOCKWISE;
}

Contour& Contour::reverse() {
  std::list<Point> points;
  for (auto& segment : segments_) {
    switch (segment.type()) {
      case Segment::Type::MOVE:
      case Segment::Type::LINE:
        points.emplace_back(segment.point());
        break;
      case Segment::Type::QUADRATIC:
        points.emplace_back(segment.control1());
        points.emplace_back(segment.point());
        break;
      case Segment::Type::CUBIC:
        points.emplace_back(segment.control1());
        points.emplace_back(segment.control2());
        points.emplace_back(segment.point());
        break;
      default:
        break;
    }
  }
  if (segments_.back().type() == Segment::Type::CLOSE) {
    std::reverse(std::next(std::begin(segments_)),
                 std::prev(std::end(segments_)));
  } else {
    std::reverse(std::next(std::begin(segments_)), std::end(segments_));
  }
  std::reverse(std::begin(points), std::end(points));
  auto itr = std::begin(points);
  for (auto& segment : segments_) {
    switch (segment.type()) {
      case Segment::Type::MOVE:
      case Segment::Type::LINE:
        segment.point() = *(itr++);
        break;
      case Segment::Type::QUADRATIC:
        segment.control1() = *(itr++);
        segment.point() = *(itr++);
        break;
      case Segment::Type::CUBIC:
        segment.control1() = *(itr++);
        segment.control2() = *(itr++);
        segment.point() = *(itr++);
        break;
      default:
        break;
    }
  }
  assert(itr == std::end(points));
  return *this;
}

}  // namespace graphics
}  // namespace takram
