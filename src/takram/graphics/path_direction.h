//
//  takram/graphics/path_direction.h
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
#ifndef TAKRAM_GRAPHICS_PATH_DIRECTION_H_
#define TAKRAM_GRAPHICS_PATH_DIRECTION_H_

#include <cassert>
#include <ostream>

namespace takram {
namespace graphics {

enum class PathDirection {
  UNDEFINED,
  CLOCKWISE,
  COUNTER_CLOCKWISE
};

inline std::ostream& operator<<(std::ostream& os, PathDirection direction) {
  switch (direction) {
    case PathDirection::UNDEFINED: os << "undefined"; break;
    case PathDirection::CLOCKWISE: os << "clockwise"; break;
    case PathDirection::COUNTER_CLOCKWISE: os << "counter clockwise"; break;
    default:
      assert(false);
      break;
  }
  return os;
}

}  // namespace graphics

namespace gfx = graphics;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_PATH_DIRECTION_H_
