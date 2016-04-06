//
//  takram/graphics/color3.h
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

#include "takram/graphics/channel.h"
#include "takram/graphics/depth.h"
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
  explicit Color(const T *values, int size = 3);
  template <class... Args>
  Color(const std::tuple<Args...>& tuple);
  Color(std::initializer_list<T> list);

  // Implicit conversion
  template <class U>
  Color(const Color3<U>& other);

#if TAKRAM_HAS_OPENCV
  template <class U>
  Color(const cv::Vec<U, 3>& other);
  operator cv::Vec<T, 3>() const;
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
  Color(const Color& other);
  Color& operator=(const Color& other);

  // Factory
  static Color white();
  static Color gray();
  static Color black();
  static Color red();
  static Color green();
  static Color blue();
  static Color cyan();
  static Color magenta();
  static Color yellow();
  static Color hex(std::uint32_t hex);

  // Mutators
  void set(T gray);
  void set(T red, T green, T blue);
  void set(const T *values, int size = 3);
  template <class... Args>
  void set(const std::tuple<Args...>& tuple);
  void set(std::initializer_list<T> list);
  void reset();

  // Element access
  T& operator[](int index) { return at(index); }
  const T& operator[](int index) const { return at(index); }
  T& operator[](Channel channel) { return at(channel); }
  const T& operator[](Channel channel) const { return at(channel); }
  T& at(int index);
  const T& at(int index) const;
  T& at(Channel channel);
  const T& at(Channel channel) const;
  T& front() { return vector.front(); }
  const T& front() const { return vector.front(); }
  T& back() { return vector.back(); }
  const T& back() const { return vector.back(); }

  // Interpolation
  template <class V>
  Color lerp(const Color& other, V factor) const;
  template <class V>
  Color4<T> lerp(const Color4<T>& other, V factor) const;

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
  T * pointer() { return vector.pointer(); }
  const T * pointer() const { return vector.pointer(); }

 public:
  union {
    Vec3<T> vector;
    struct { T r; T g; T b; };
  };
};

// Comparison
template <class T, class U>
bool operator==(const Color3<T>& lhs, const Color3<U>& rhs);
template <class T, class U>
bool operator!=(const Color3<T>& lhs, const Color3<U>& rhs);

using Color3u = Color3<std::uint8_t>;
using Color3s = Color3<std::uint16_t>;
using Color3i = Color3<std::uint32_t>;
using Color3f = Color3<float>;
using Color3d = Color3<double>;

#pragma mark -

template <class T>
inline Color<T, 3>::Color() : vector() {}

template <class T>
inline Color<T, 3>::Color(T gray) : vector(gray, gray, gray) {}

template <class T>
inline Color<T, 3>::Color(T red, T green, T blue) : vector(red, green, blue) {}

template <class T>
inline Color<T, 3>::Color(const T *values, int size) : vector(values, size) {}

template <class T>
template <class... Args>
inline Color<T, 3>::Color(const std::tuple<Args...>& tuple) : vector(tuple) {}

template <class T>
inline Color<T, 3>::Color(std::initializer_list<T> list) : vector(list) {}

#pragma mark Implicit conversion

template <class T>
template <class U>
inline Color<T, 3>::Color(const Color3<U>& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b)) {}

#if TAKRAM_HAS_OPENCV

template <class T>
template <class U>
inline Color<T, 3>::Color(const cv::Vec<U, 3>& other)
    : vector(Depth<T>::convert(other[0]),
             Depth<T>::convert(other[1]),
             Depth<T>::convert(other[2])) {}

template <class T>
inline Color<T, 3>::operator cv::Vec<T, 3>() const {
  return cv::Vec<T, 3>(Depth<T>::convert(r),
                       Depth<T>::convert(g),
                       Depth<T>::convert(b));
}

#endif  // TAKRAM_HAS_OPENCV

#if TAKRAM_HAS_OPENFRAMEWORKS

template <class T>
template <class U>
inline Color<T, 3>::Color(const ofColor_<U>& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b)) {}

template <class T>
inline Color<T, 3>::operator ofColor_<T>() const {
  return ofColor_<T>(Depth<T>::convert(r),
                     Depth<T>::convert(g),
                     Depth<T>::convert(b));
}

#endif  // TAKRAM_HAS_OPENFRAMEWORKS

#if TAKRAM_HAS_CINDER

template <class T>
template <class U>
inline Color<T, 3>::Color(const ci::ColorT<U>& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b)) {}

template <class T>
inline Color<T, 3>::operator ci::ColorT<T>() const {
  return ci::ColorT<T>(Depth<T>::convert(r),
                       Depth<T>::convert(g),
                       Depth<T>::convert(b));
}

#endif  // TAKRAM_HAS_CINDER

#if TAKRAM_HAS_NANOVG

template <class T>
inline Color<T, 3>::Color(const NVGcolor& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b)) {}

template <class T>
inline Color<T, 3>::operator NVGcolor() const {
  return {{{
    Depth<float>::convert(r),
    Depth<float>::convert(g),
    Depth<float>::convert(b),
    Depth<float>::max
  }}};
}

#endif  // TAKRAM_HAS_NANOVG

#pragma mark Explicit conversion

template <class T>
template <class U>
inline Color<T, 3>::Color(const Color4<U>& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b)) {}

template <class T>
inline Color<T, 3>::Color(const Vec3<T>& other) : vector(other) {}

#pragma mark Copy semantics

template <class T>
inline Color<T, 3>::Color(const Color& other) : vector(other.vector) {}

template <class T>
inline Color3<T>& Color<T, 3>::operator=(const Color& other) {
  if (&other != this) {
    vector = other.vector;
  }
  return *this;
}

#pragma mark Factory

template <class T>
inline Color3<T> Color<T, 3>::white() {
  return Color(Depth<T>::max);
}

template <class T>
inline Color3<T> Color<T, 3>::gray() {
  return Color((Depth<T>::min + Depth<T>::max) / 2);
}

template <class T>
inline Color3<T> Color<T, 3>::black() {
  return Color(Depth<T>::min);
}

template <class T>
inline Color3<T> Color<T, 3>::red() {
  return Color(Depth<T>::max, Depth<T>::min, Depth<T>::min);
}

template <class T>
inline Color3<T> Color<T, 3>::green() {
  return Color(Depth<T>::min, Depth<T>::max, Depth<T>::min);
}

template <class T>
inline Color3<T> Color<T, 3>::blue() {
  return Color(Depth<T>::min, Depth<T>::min, Depth<T>::max);
}

template <class T>
inline Color3<T> Color<T, 3>::cyan() {
  return Color(Depth<T>::min, Depth<T>::max, Depth<T>::max);
}

template <class T>
inline Color3<T> Color<T, 3>::magenta() {
  return Color(Depth<T>::max, Depth<T>::min, Depth<T>::max);
}

template <class T>
inline Color3<T> Color<T, 3>::yellow() {
  return Color(Depth<T>::max, Depth<T>::max, Depth<T>::min);
}

template <class T>
inline Color3<T> Color<T, 3>::hex(std::uint32_t hex) {
  return Color(
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 16))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 8))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 0))));
}

#pragma mark Mutators

template <class T>
inline void Color<T, 3>::set(T gray) {
  vector.set(gray, gray, gray);
}

template <class T>
inline void Color<T, 3>::set(T red, T green, T blue) {
  vector.set(red, green, blue);
}

template <class T>
inline void Color<T, 3>::set(const T *values, int size) {
  vector.set(values, size);
}

template <class T>
template <class... Args>
inline void Color<T, 3>::set(const std::tuple<Args...>& tuple) {
  vector.set(tuple);
}

template <class T>
inline void Color<T, 3>::set(std::initializer_list<T> list) {
  vector.set(list);
}

template <class T>
inline void Color<T, 3>::reset() {
  vector.reset();
}

#pragma mark Element access

template <class T>
inline T& Color<T, 3>::at(int index) {
  return vector.at(index);
}

template <class T>
inline const T& Color<T, 3>::at(int index) const {
  return vector.at(index);
}

template <class T>
inline T& Color<T, 3>::at(Channel channel) {
  return at(static_cast<int>(channel));
}

template <class T>
inline const T& Color<T, 3>::at(Channel channel) const {
  return at(static_cast<int>(channel));
}

#pragma mark Comparison

template <class T, class U>
inline bool operator==(const Color3<T>& lhs, const Color3<U>& rhs) {
  return lhs.vector == rhs.vector;
}

template <class T, class U>
inline bool operator!=(const Color3<T>& lhs, const Color3<U>& rhs) {
  return !(lhs == rhs);
}

#pragma mark Interpolation

template <class T>
template <class V>
inline Color3<T> Color<T, 3>::lerp(const Color& other, V factor) const {
  return Color(r + (other.r - r) * factor,
               g + (other.g - g) * factor,
               b + (other.b - b) * factor);
}

template <class T>
template <class V>
inline Color4<T> Color<T, 3>::lerp(const Color4<T>& other, V factor) const {
  return Color4<T>(r + (other.r - r) * factor,
                   g + (other.g - g) * factor,
                   b + (other.b - b) * factor);
}

#pragma mark Stream

template <class T>
inline std::ostream& operator<<(std::ostream& os, const Color3<T>& color) {
  return os << color.vector;
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
