//
//  takram/graphics/depth.h
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
#ifndef TAKRAM_GRAPHICS_DEPTH_H_
#define TAKRAM_GRAPHICS_DEPTH_H_

#include <cmath>
#include <limits>

#include "takram/math/functions.h"
#include "takram/math/enablers.h"

namespace takram {
namespace graphics {

template <class T, class Enable = void>
struct Depth {};

template <class T>
using IntegralDepth = Depth<T, EnableIfIntegral<T>>;
template <class T>
using FloatingDepth = Depth<T, EnableIfFloating<T>>;

template <class T>
struct Depth<T, EnableIfIntegral<T>> {
  static constexpr const int bits = std::numeric_limits<T>::digits;
  static constexpr const T min = std::numeric_limits<T>::min();
  static constexpr const T max = std::numeric_limits<T>::max();

  static T clamp(T value);
  template <class U>
  static EnableIfIntegral<U, T> convert(U value);
  template <class U>
  static EnableIfFloating<U, T> convert(U value);
};

template <class T>
struct Depth<T, EnableIfFloating<T>> {
  static constexpr const int bits = sizeof(T) * 8;
  static constexpr const T min = 0;
  static constexpr const T max = 1;

  static T clamp(T value);
  template <class U>
  static EnableIfIntegral<U, T> convert(U value);
  template <class U>
  static EnableIfFloating<U, T> convert(U value);
};

#pragma mark -

#pragma mark Conversions

template <class T>
inline T IntegralDepth<T>::clamp(T value) {
  return math::clamp(value, min, max);
}

template <class T>
inline T FloatingDepth<T>::clamp(T value) {
  return math::clamp(value, min, max);
}

template <class T>
template <class U>
inline EnableIfIntegral<U, T> IntegralDepth<T>::convert(U value) {
  if (Depth<U>::bits < bits) {
    return (static_cast<T>(value) << (bits - Depth<U>::bits)) | value;
  } else if (Depth<U>::bits > bits) {
    return value >> (Depth<U>::bits - bits);
  }
  return value;
}

template <class T>
template <class U>
inline EnableIfFloating<U, T> IntegralDepth<T>::convert(U value) {
  return std::round(max * value);
}

template <class T>
template <class U>
inline EnableIfIntegral<U, T> FloatingDepth<T>::convert(U value) {
  return static_cast<T>(value) / Depth<U>::max;
}

template <class T>
template <class U>
inline EnableIfFloating<U, T> FloatingDepth<T>::convert(U value) {
  return value;
}

}  // namespace graphics

namespace gfx = graphics;

using graphics::Depth;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_DEPTH_H_
