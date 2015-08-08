//
//  takram/graphics/color3.h
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
#ifndef TAKRAM_GRAPHICS_COLOR3_H_
#define TAKRAM_GRAPHICS_COLOR3_H_

#include <cstdint>
#include <initializer_list>
#include <ostream>
#include <tuple>

#if TAKRAM_HAS_OPENCV
#include "opencv2/core/core.hpp"
#endif  // TAKRAM_HAS_OPENCV

#if TAKRAM_HAS_OPENFRAMEWORKS
#include "ofColor.h"
#endif  // TAKRAM_HAS_OPENFRAMEWORKS

#if TAKRAM_HAS_CINDER
#include "cinder/Color.h"
#endif  // TAKRAM_HAS_CINDER

#if TAKRAM_HAS_NANOVG
#include "nanovg.h"
#endif  // TAKRAM_HAS_NANOVG

#include "takram/graphics/color_depth.h"
#include "takram/math/vector.h"

namespace takram {
namespace graphics {

template <class T, int C>
class Color;

template <class T>
using Color3 = Color<T, 3>;
template <class T>
using Color4 = Color<T, 4>;

template <class T>
class Color<T, 3> final {
 public:
  using Type = T;
  using Iterator = typename Vec3<T>::Iterator;
  using ConstIterator = typename Vec3<T>::ConstIterator;
  using ReverseIterator = typename Vec3<T>::ReverseIterator;
  using ConstReverseIterator = typename Vec3<T>::ConstReverseIterator;
  static constexpr const auto channels = Vec3<T>::dimensions;

 public:
  Color();
  explicit Color(T gray);
  Color(T red, T green, T blue);
  explicit Color(const T *values, int size = channels);
  template <class... Args>
  Color(const std::tuple<Args...>& tuple);
  Color(std::initializer_list<T> list);

  // Implicit conversion
  template <class U>
  Color(const Color3<U>& other);

#if TAKRAM_HAS_OPENCV
  template <class U>
  Color(const cv::Vec<U, channels>& other);
  operator cv::Vec<T, channels>() const;
#endif  // TAKRAM_HAS_OPENCV

#if TAKRAM_HAS_OPENFRAMEWORKS
  template <class U>
  explicit Color(const ofColor_<U>& other);
  operator ofColor_<T>() const;
#endif  // TAKRAM_HAS_OPENFRAMEWORKS

#if TAKRAM_HAS_CINDER
  Color(const ci::ColorT<U>& other);
  operator ci::ColorT<T>() const;
#endif  // TAKRAM_HAS_CINDER

#if TAKRAM_HAS_NANOVG
  explicit Color(const NVGcolor& other);
  operator NVGcolor() const;
#endif  // TAKRAM_HAS_NANOVG

  // Explicit conversion
  template <class U>
  explicit Color(const Color4<U>& other);
  explicit Color(const Vec3<T>& other);

  // Copy semantics
  Color(const Color3<T>& other);
  Color3<T>& operator=(const Color3<T>& other);

  // Factory
  static Color3<T> white();
  static Color3<T> gray();
  static Color3<T> black();
  static Color3<T> hex(std::uint32_t hex);

  // Mutators
  void set(T gray);
  void set(T red, T green, T blue);
  void set(const T *values, int size = channels);
  template <class... Args>
  void set(const std::tuple<Args...>& tuple);
  void set(std::initializer_list<T> list);
  void reset();

  // Element access
  T& operator[](int index) { return at(index); }
  const T& operator[](int index) const { return at(index); }
  T& at(int index);
  const T& at(int index) const;
  T& front() { return vector.front(); }
  const T& front() const { return vector.front(); }
  T& back() { return vector.back(); }
  const T& back() const { return vector.back(); }

  // Comparison
  template <class U>
  bool operator==(const Color3<U>& other) const;
  template <class U>
  bool operator!=(const Color3<U>& other) const;

  // Iterator
  Iterator begin() { return std::begin(vector); }
  ConstIterator begin() const { return std::begin(vector); }
  Iterator end() { return std::end(vector); }
  ConstIterator end() const { return std::end(vector); }
  ReverseIterator rbegin() { return std::rbegin(vector); }
  ConstReverseIterator rbegin() const { return std::rbegin(vector); }
  ReverseIterator rend() { return std::rend(vector); }
  ConstReverseIterator rend() const { return std::rend(vector); }

  // Pointer
  T * ptr() { return vector.ptr(); }
  const T * ptr() const { return vector.ptr(); }

 public:
  union {
    Vec3<T> vector;
    struct { T red; T green; T blue; };
    struct { T r; T g; T b; };
  };
};

using Color3u = Color3<std::uint8_t>;
using Color3s = Color3<std::uint16_t>;
using Color3i = Color3<std::uint32_t>;
using Color3f = Color3<float>;
using Color3d = Color3<double>;

#pragma mark -

template <class T>
inline Color3<T>::Color() : vector() {}

template <class T>
inline Color3<T>::Color(T gray) : vector(gray, gray, gray) {}

template <class T>
inline Color3<T>::Color(T red, T green, T blue) : vector(red, green, blue) {}

template <class T>
inline Color3<T>::Color(const T *values, int size) : vector(values, size) {}

template <class T>
template <class... Args>
inline Color3<T>::Color(const std::tuple<Args...>& tuple) : vector(tuple) {}

template <class T>
inline Color3<T>::Color(std::initializer_list<T> list) : vector(list) {}

#pragma mark Implicit conversion

template <class T>
template <class U>
inline Color3<T>::Color(const Color3<U>& other)
    : vector(ColorDepth<T>::convert(other.r),
             ColorDepth<T>::convert(other.g),
             ColorDepth<T>::convert(other.b)) {}

#if TAKRAM_HAS_OPENCV

template <class T>
template <class U>
inline Color3<T>::Color(const cv::Vec<U, channels>& other)
    : vector(ColorDepth<T>::convert(other[0]),
             ColorDepth<T>::convert(other[1]),
             ColorDepth<T>::convert(other[2])) {}

template <class T>
inline Color3<T>::operator cv::Vec<T, channels>() const {
  return cv::Vec<T, channels>(
    ColorDepth<T>::convert(r),
    ColorDepth<T>::convert(g),
    ColorDepth<T>::convert(b)
  );
}

#endif  // TAKRAM_HAS_OPENCV

#if TAKRAM_HAS_OPENFRAMEWORKS

template <class T>
template <class U>
inline Color3<T>::Color(const ofColor_<U>& other)
    : vector(ColorDepth<T>::convert(other.r),
             ColorDepth<T>::convert(other.g),
             ColorDepth<T>::convert(other.b)) {}

template <class T>
inline Color3<T>::operator ofColor_<T>() const {
  return ofColor_<T>(
    ColorDepth<T>::convert(r),
    ColorDepth<T>::convert(g),
    ColorDepth<T>::convert(b)
  );
}

#endif  // TAKRAM_HAS_OPENFRAMEWORKS

#if TAKRAM_HAS_CINDER

template <class T>
template <class U>
inline Color3<T>::Color(const ci::ColorT<U>& other)
    : vector(ColorDepth<T>::convert(other.r),
             ColorDepth<T>::convert(other.g),
             ColorDepth<T>::convert(other.b)) {}

template <class T>
inline Color3<T>::operator ci::ColorT<T>() const {
  return ci::ColorT<T>(
    ColorDepth<T>::convert(r),
    ColorDepth<T>::convert(g),
    ColorDepth<T>::convert(b)
  );
}

#endif  // TAKRAM_HAS_CINDER

#if TAKRAM_HAS_NANOVG

template <class T>
inline Color3<T>::Color(const NVGcolor& other)
    : vector(ColorDepth<T>::convert(other.r),
             ColorDepth<T>::convert(other.g),
             ColorDepth<T>::convert(other.b)) {}

template <class T>
inline Color3<T>::operator NVGcolor() const {
  return {{{
    ColorDepth<float>::convert(r),
    ColorDepth<float>::convert(g),
    ColorDepth<float>::convert(b),
    ColorDepth<float>::max
  }}};
}

#endif  // TAKRAM_HAS_NANOVG

#pragma mark Explicit conversion

template <class T>
template <class U>
inline Color3<T>::Color(const Color4<U>& other)
    : vector(ColorDepth<T>::convert(other.r),
             ColorDepth<T>::convert(other.g),
             ColorDepth<T>::convert(other.b)) {}

template <class T>
inline Color3<T>::Color(const Vec3<T>& other) : vector(other) {}

#pragma mark Copy semantics

template <class T>
inline Color3<T>::Color(const Color3<T>& other) : vector(other.vector) {}

template <class T>
inline Color3<T>& Color3<T>::operator=(const Color3<T>& other) {
  if (&other != this) {
    vector = other.vector;
  }
  return *this;
}

#pragma mark Factory

template <class T>
inline Color3<T> Color3<T>::white() {
  return Color3<T>(ColorDepth<T>::max);
}

template <class T>
inline Color3<T> Color3<T>::gray() {
  return Color3<T>((ColorDepth<T>::min + ColorDepth<T>::max) / 2);
}

template <class T>
inline Color3<T> Color3<T>::black() {
  return Color3<T>(ColorDepth<T>::min);
}

template <class T>
inline Color3<T> Color3<T>::hex(std::uint32_t hex) {
  return Color3<T>(
    ColorDepth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 16))),
    ColorDepth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 8))),
    ColorDepth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 0)))
  );
}

#pragma mark Mutators

template <class T>
inline void Color3<T>::set(T gray) {
  vector.set(gray, gray, gray);
}

template <class T>
inline void Color3<T>::set(T red, T green, T blue) {
  vector.set(red, green, blue);
}

template <class T>
inline void Color3<T>::set(const T *values, int size) {
  vector.set(values, size);
}

template <class T>
template <class... Args>
inline void Color3<T>::set(const std::tuple<Args...>& tuple) {
  vector.set(tuple);
}

template <class T>
inline void Color3<T>::set(std::initializer_list<T> list) {
  vector.set(list);
}

template <class T>
inline void Color3<T>::reset() {
  vector.reset();
}

#pragma mark Element access

template <class T>
inline T& Color3<T>::at(int index) {
  return vector.at(index);
}

template <class T>
inline const T& Color3<T>::at(int index) const {
  return vector.at(index);
}

#pragma mark Comparison

template <class T>
template <class U>
inline bool Color3<T>::operator==(const Color3<U>& other) const {
  return vector == other.vector;
}

template <class T>
template <class U>
inline bool Color3<T>::operator!=(const Color3<U>& other) const {
  return vector != other.vector;
}

#pragma mark Stream

template <class T>
inline std::ostream& operator<<(std::ostream& os, const Color3<T>& other) {
  return os << other.vector;
}

}  // namespace graphics

namespace gfx = graphics;

using graphics::Color;
using graphics::Color3;
using graphics::Color3u;
using graphics::Color3s;
using graphics::Color3i;
using graphics::Color3f;
using graphics::Color3d;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_COLOR3_H_
