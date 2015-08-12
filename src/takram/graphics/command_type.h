//
//  takram/graphics/command_type.h
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
#ifndef TAKRAM_GRAPHICS_COMMAND_TYPE_H_
#define TAKRAM_GRAPHICS_COMMAND_TYPE_H_

#include <cassert>
#include <ostream>

namespace takram {
namespace graphics {

enum class CommandType {
  MOVE,
  LINE,
  QUADRATIC,
  CONIC,
  CUBIC,
  CLOSE
};

inline std::ostream& operator<<(std::ostream& os, CommandType type) {
  switch (type) {
    case CommandType::MOVE: os << "move"; break;
    case CommandType::LINE: os << "line"; break;
    case CommandType::QUADRATIC: os << "quadratic"; break;
    case CommandType::CONIC: os << "conic"; break;
    case CommandType::CUBIC: os << "cubic"; break;
    case CommandType::CLOSE: os << "close"; break;
    default:
      assert(false);
      break;
  }
  return os;
}

}  // namespace graphics

namespace gfx = graphics;

using graphics::CommandType;

}  // namespace takram

#endif  // TAKRAM_GRAPHICS_COMMAND_TYPE_H_
