//
//  takram/graphics/path.h
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
#ifndef TAKRAM_GRAPHICS_PATH_H_
#define TAKRAM_GRAPHICS_PATH_H_

#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

#include "takram/algorithm/leaf_iterator_iterator.h"
#include "takram/graphics/contour.h"
#include "takram/graphics/segment.h"
#include "takram/math/vector.h"

namespace takram {
namespace graphics {

class Path final {
 public:
  using Real = double;
  using Point = math::Vec2<Real>;
  using Iterator = LeafIteratorIterator<
      std::vector<Contour>::iterator,
      Contour::Iterator>;
  using ConstIterator = LeafIteratorIterator<
      std::vector<Contour>::const_iterator,
      Contour::ConstIterator>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

 public:
  Path() = default;
  explicit Path(const std::vector<Contour>& contours);

  // Copy semantics
  Path(const Path& other) = default;
  Path& operator=(const Path& other) = default;

  // Mutators
  void set(const std::vector<Contour>& contours);
  void reset();

  // Comparison
  bool operator==(const Path& other) const;
  bool operator!=(const Path& other) const;

  // Attributes
  bool empty() const { return contours_.empty(); }
  std::size_t size() const { return contours_.size(); }

  // Properties
  const std::vector<Contour>& contours() const { return contours_; }
  std::vector<Contour>& contours() { return contours_; }

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

  // Element access
  Contour& operator[](int index) { return contours_.at(index); }
  const Contour& operator[](int index) const { return contours_.at(index); }
  Contour& at(int index) { return contours_.at(index); }
  const Contour& at(int index) const { return contours_.at(index); }
  Contour& front() { return contours_.front(); }
  const Contour& front() const { return contours_.front(); }
  Contour& back() { return contours_.back(); }
  const Contour& back() const { return contours_.back(); }

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
  std::vector<Contour> contours_;
};

#pragma mark -

inline Path::Path(const std::vector<Contour>& contours)
    : contours_(contours) {}

#pragma mark Mutators

inline void Path::set(const std::vector<Contour>& contours) {
  contours_ = contours;
}

inline void Path::reset() {
  contours_.clear();
}

#pragma mark Comparison

inline bool Path::operator==(const Path& other) const {
  return contours_ == other.contours_;
}

inline bool Path::operator!=(const Path& other) const {
  return !operator==(other);
}

#pragma mark Configuring segments

inline void Path::close() {
  if (!contours_.empty()) {
    contours_.back().close();
  }
}

inline void Path::moveTo(Real x, Real y) {
  contours_.emplace_back();
  contours_.back().moveTo(x, y);
}

inline void Path::moveTo(const Point& point) {
  contours_.emplace_back();
  contours_.back().moveTo(point);
}

inline void Path::lineTo(Real x, Real y) {
  if (!contours_.empty()) {
    contours_.back().lineTo(x, y);
  }
}

inline void Path::lineTo(const Point& point) {
  if (!contours_.empty()) {
    contours_.back().lineTo(point);
  }
}

inline void Path::quadraticTo(Real cx, Real cy, Real x, Real y) {
  if (!contours_.empty()) {
    contours_.back().quadraticTo(cx, cy, x, y);
  }
}

inline void Path::quadraticTo(const Point& control, const Point& point) {
  if (!contours_.empty()) {
    contours_.back().quadraticTo(control, point);
  }
}

inline void Path::cubicTo(Real cx1, Real cy1,
                          Real cx2, Real cy2,
                          Real x, Real y) {
  if (!contours_.empty()) {
    contours_.back().cubicTo(cx1, cy1, cx2, cy2, x, y);
  }
}

inline void Path::cubicTo(const Point& control1,
                          const Point& control2,
                          const Point& point) {
  if (!contours_.empty()) {
    contours_.back().cubicTo(control1, control2, point);
  }
}

#pragma mark Iterator

inline Path::Iterator Path::begin() {
  return Iterator(std::begin(contours_), std::end(contours_));
}

inline Path::ConstIterator Path::begin() const {
  return ConstIterator(std::begin(contours_), std::end(contours_));
}

inline Path::Iterator Path::end() {
  return Iterator(std::end(contours_), std::end(contours_));
}

inline Path::ConstIterator Path::end() const {
  return ConstIterator(std::end(contours_), std::end(contours_));
}

}  // namespace graphics

namespace gfx = graphics;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_PATH_H_
