//
//  takram/graphics/color4.h
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
#ifndef TAKRAM_GRAPHICS_COLOR4_H_
#define TAKRAM_GRAPHICS_COLOR4_H_

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
class Color<T, 4> final {
 public:
  using Type = T;
  using Iterator = typename Vec4<T>::Iterator;
  using ConstIterator = typename Vec4<T>::ConstIterator;
  using ReverseIterator = typename Vec4<T>::ReverseIterator;
  using ConstReverseIterator = typename Vec4<T>::ConstReverseIterator;
  static constexpr const auto channels = Vec4<T>::dimensions;

 public:
  Color();
  explicit Color(T gray, T alpha = Depth<T>::max);
  Color(T red, T green, T blue, T alpha = Depth<T>::max);
  explicit Color(const T *values, int size = channels);
  template <class... Args>
  Color(const std::tuple<Args...>& tuple);
  Color(std::initializer_list<T> list);
  template <class U>
  explicit Color(const Color3<U>& color, T alpha);
  template <class U>
  explicit Color(const Color4<U>& color, T alpha);

  // Implicit conversion
  template <class U>
  Color(const Color4<U>& other);

#if TAKRAM_HAS_OPENCV
  template <class U>
  Color(const cv::Vec<U, channels>& other);
  operator cv::Vec<T, channels>() const;
#endif  // TAKRAM_HAS_OPENCV

#if TAKRAM_HAS_OPENFRAMEWORKS
  template <class U>
  Color(const ofColor_<U>& other);
  operator ofColor_<T>() const;
#endif  // TAKRAM_HAS_OPENFRAMEWORKS

#if TAKRAM_HAS_CINDER
  template <class U>
  Color(const ci::ColorAT<U>& other);
  operator ci::ColorAT<T>() const;
#endif  // TAKRAM_HAS_CINDER

#if TAKRAM_HAS_NANOVG
  Color(const NVGcolor& other);
  operator NVGcolor() const;
#endif  // TAKRAM_HAS_NANOVG

  // Explicit conversion
  template <class U>
  explicit Color(const Color3<U>& other);
  explicit Color(const Vec4<T>& other);

  // Copy semantics
  Color(const Color& other);
  Color& operator=(const Color& other);

  // Factory
  static Color white(T alpha = Depth<T>::max);
  static Color gray(T alpha = Depth<T>::max);
  static Color black(T alpha = Depth<T>::max);
  static Color red(T alpha = Depth<T>::max);
  static Color green(T alpha = Depth<T>::max);
  static Color blue(T alpha = Depth<T>::max);
  static Color cyan(T alpha = Depth<T>::max);
  static Color magenta(T alpha = Depth<T>::max);
  static Color yellow(T alpha = Depth<T>::max);
  static Color hex(std::uint32_t hex);
  static Color hex(std::uint32_t hex, math::Promote<T> alpha);
  static Color hexA(std::uint32_t hex);

  // Mutators
  void set(T gray, T alpha = Depth<T>::max);
  void set(T red, T green, T blue, T alpha = Depth<T>::max);
  void set(const T *values, int size = channels);
  template <class... Args>
  void set(const std::tuple<Args...>& tuple);
  void set(std::initializer_list<T> list);
  template <class U>
  void set(const Color3<U>& color, T alpha = Depth<T>::max);
  template <class U>
  void set(const Color4<U>& color, T alpha = Depth<T>::max);
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

  // Comparison
  template <class U>
  bool operator==(const Color4<U>& other) const;
  template <class U>
  bool operator!=(const Color4<U>& other) const;

  // Interpolation
  template <class V>
  Color lerp(const Color& other, V factor) const;
  template <class V>
  Color4<T> lerp(const Color3<T>& other, V factor) const;

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
    Vec4<T> vector;
    struct { T r; T g; T b; T a; };
  };
};

using Color4u = Color4<std::uint8_t>;
using Color4s = Color4<std::uint16_t>;
using Color4i = Color4<std::uint32_t>;
using Color4f = Color4<float>;
using Color4d = Color4<double>;

#pragma mark -

template <class T>
inline Color4<T>::Color() : vector() {}

template <class T>
inline Color4<T>::Color(T gray, T alpha) : vector(gray, gray, gray, alpha) {}

template <class T>
inline Color4<T>::Color(T red, T green, T blue, T alpha)
    : vector(red, green, blue, alpha) {}

template <class T>
inline Color4<T>::Color(const T *values, int size) : vector(values, size) {}

template <class T>
template <class... Args>
inline Color4<T>::Color(const std::tuple<Args...>& tuple) : vector(tuple) {}

template <class T>
inline Color4<T>::Color(std::initializer_list<T> list) : vector(list) {}

template <class T>
template <class U>
inline Color4<T>::Color(const Color3<U>& color, T alpha) : vector() {
  set(color, alpha);
}

template <class T>
template <class U>
inline Color4<T>::Color(const Color4<U>& color, T alpha) : vector() {
  set(color, alpha);
}

#pragma mark Implicit conversion

template <class T>
template <class U>
inline Color4<T>::Color(const Color4<U>& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b),
             Depth<T>::convert(other.a)) {}

#if TAKRAM_HAS_OPENCV

template <class T>
template <class U>
inline Color4<T>::Color(const cv::Vec<U, channels>& other)
    : vector(Depth<T>::convert(other[0]),
             Depth<T>::convert(other[1]),
             Depth<T>::convert(other[2]),
             Depth<T>::convert(other[3])) {}

template <class T>
inline Color4<T>::operator cv::Vec<T, channels>() const {
  return cv::Vec<T, channels>(Depth<T>::convert(r),
                              Depth<T>::convert(g),
                              Depth<T>::convert(b),
                              Depth<T>::convert(a));
}

#endif  // TAKRAM_HAS_OPENCV

#if TAKRAM_HAS_OPENFRAMEWORKS

template <class T>
template <class U>
inline Color4<T>::Color(const ofColor_<U>& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b),
             Depth<T>::convert(other.a)) {}

template <class T>
inline Color4<T>::operator ofColor_<T>() const {
  return ofColor_<T>(Depth<T>::convert(r),
                     Depth<T>::convert(g),
                     Depth<T>::convert(b),
                     Depth<T>::convert(a));
}

#endif  // TAKRAM_HAS_OPENFRAMEWORKS

#if TAKRAM_HAS_CINDER

template <class T>
template <class U>
inline Color4<T>::Color(const ci::ColorAT<U>& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b),
             Depth<T>::convert(other.a)) {}

template <class T>
inline Color4<T>::operator ci::ColorAT<T>() const {
  return ci::ColorT<T>(Depth<T>::convert(r),
                       Depth<T>::convert(g),
                       Depth<T>::convert(b),
                       Depth<T>::convert(a));
}

#endif  // TAKRAM_HAS_CINDER

#if TAKRAM_HAS_NANOVG

template <class T>
inline Color4<T>::Color(const NVGcolor& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b),
             Depth<T>::convert(other.a)) {}

template <class T>
inline Color4<T>::operator NVGcolor() const {
  return {{{
    Depth<float>::convert(r),
    Depth<float>::convert(g),
    Depth<float>::convert(b),
    Depth<float>::convert(a)
  }}};
}

#endif  // TAKRAM_HAS_NANOVG

#pragma mark Explicit conversion

template <class T>
template <class U>
inline Color4<T>::Color(const Color3<U>& other)
    : vector(Depth<T>::convert(other.r),
             Depth<T>::convert(other.g),
             Depth<T>::convert(other.b)) {}

template <class T>
inline Color4<T>::Color(const Vec4<T>& other) : vector(other) {}

#pragma mark Copy semantics

template <class T>
inline Color4<T>::Color(const Color& other) : vector(other.vector) {}

template <class T>
inline Color4<T>& Color4<T>::operator=(const Color& other) {
  if (&other != this) {
    vector = other.vector;
  }
  return *this;
}

#pragma mark Factory

template <class T>
inline Color4<T> Color4<T>::white(T alpha) {
  return Color(Depth<T>::max, alpha);
}

template <class T>
inline Color4<T> Color4<T>::gray(T alpha) {
  return Color((Depth<T>::min + Depth<T>::max) / 2, alpha);
}

template <class T>
inline Color4<T> Color4<T>::black(T alpha) {
  return Color(Depth<T>::min, alpha);
}

template <class T>
inline Color4<T> Color4<T>::red(T alpha) {
  return Color(Depth<T>::max, Depth<T>::min, Depth<T>::min, alpha);
}

template <class T>
inline Color4<T> Color4<T>::green(T alpha) {
  return Color(Depth<T>::min, Depth<T>::max, Depth<T>::min, alpha);
}

template <class T>
inline Color4<T> Color4<T>::blue(T alpha) {
  return Color(Depth<T>::min, Depth<T>::min, Depth<T>::max, alpha);
}

template <class T>
inline Color4<T> Color4<T>::cyan(T alpha) {
  return Color(Depth<T>::min, Depth<T>::max, Depth<T>::max, alpha);
}

template <class T>
inline Color4<T> Color4<T>::magenta(T alpha) {
  return Color(Depth<T>::max, Depth<T>::min, Depth<T>::max, alpha);
}

template <class T>
inline Color4<T> Color4<T>::yellow(T alpha) {
  return Color(Depth<T>::max, Depth<T>::max, Depth<T>::min, alpha);
}

template <class T>
inline Color4<T> Color4<T>::hex(std::uint32_t hex) {
  return Color(
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 16))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 8))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 0))));
}

template <class T>
inline Color4<T> Color4<T>::hex(std::uint32_t hex, math::Promote<T> alpha) {
  return Color(
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 16))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 8))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 0))),
      Depth<T>::convert(alpha));
}

template <class T>
inline Color4<T> Color4<T>::hexA(std::uint32_t hex) {
  return Color(
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 16))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 8))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 0))),
      Depth<T>::convert(static_cast<std::uint8_t>(0xff & (hex >> 24))));
}

#pragma mark Mutators

template <class T>
inline void Color4<T>::set(T gray, T alpha) {
  vector.set(gray, gray, gray, alpha);
}

template <class T>
inline void Color4<T>::set(T red, T green, T blue, T alpha) {
  vector.set(red, green, blue, alpha);
}

template <class T>
inline void Color4<T>::set(const T *values, int size) {
  vector.set(values, size);
}

template <class T>
template <class... Args>
inline void Color4<T>::set(const std::tuple<Args...>& tuple) {
  vector.set(tuple);
}

template <class T>
inline void Color4<T>::set(std::initializer_list<T> list) {
  vector.set(list);
}

template <class T>
template <class U>
inline void Color4<T>::set(const Color3<U>& color, T alpha) {
  r = Depth<T>::convert(color.r);
  g = Depth<T>::convert(color.g);
  b = Depth<T>::convert(color.b);
  a = alpha;
}

template <class T>
template <class U>
inline void Color4<T>::set(const Color4<U>& color, T alpha) {
  r = Depth<T>::convert(color.r);
  g = Depth<T>::convert(color.g);
  b = Depth<T>::convert(color.b);
  a = alpha;
}

template <class T>
inline void Color4<T>::reset() {
  vector.reset();
}

#pragma mark Element access

template <class T>
inline T& Color4<T>::at(int index) {
  return vector.at(index);
}

template <class T>
inline const T& Color4<T>::at(int index) const {
  return vector.at(index);
}

template <class T>
inline T& Color4<T>::at(Channel channel) {
  return at(static_cast<int>(channel));
}

template <class T>
inline const T& Color4<T>::at(Channel channel) const {
  return at(static_cast<int>(channel));
}

#pragma mark Comparison

template <class T>
template <class U>
inline bool Color4<T>::operator==(const Color4<U>& other) const {
  return vector == other.vector;
}

template <class T>
template <class U>
inline bool Color4<T>::operator!=(const Color4<U>& other) const {
  return vector != other.vector;
}

#pragma mark Interpolation

template <class T>
template <class V>
inline Color4<T> Color4<T>::lerp(const Color& other, V factor) const {
  return Color(r + (other.r - r) * factor,
               g + (other.g - g) * factor,
               b + (other.b - b) * factor,
               a + (other.a - a) * factor);
}

template <class T>
template <class V>
inline Color4<T> Color4<T>::lerp(const Color3<T>& other, V factor) const {
  return Color4<T>(r + (other.r - r) * factor,
                   g + (other.g - g) * factor,
                   b + (other.b - b) * factor,
                   a);
}

#pragma mark Stream

template <class T>
inline std::ostream& operator<<(std::ostream& os, const Color4<T>& color) {
  return os << color.vector;
}

}  // namespace graphics

namespace gfx = graphics;

using graphics::Color;
using graphics::Color4;
using graphics::Color4u;
using graphics::Color4s;
using graphics::Color4i;
using graphics::Color4f;
using graphics::Color4d;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_COLOR4_H_
