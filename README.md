<!--
   SPDX-FileCopyrightText: 2026 Daniel Aimé Valcour <fosssweeper@gmail.com>
  
   SPDX-License-Identifier: MIT
 -->

# gld.h

gld.h is a header-only C library to help with debugging while working with OpenGL. It contains macros which wrap OpenGL functions in order to clearly break when error occur while providing a clear error message. This may be preferrable to `GL_KHR_debug` callback based error handling supported since OpenGL 4.3, because this error handling mechanism will break immediatly when and where an error occurs. This library is meant to help in debug builds only, and is designed to leave your code as if it was never even there when you compile release builds.

## How To Use

In order to use this library, include it after including your OpenGL loader
(such as GLAD or GLEW).

```cpp
#include <glad/glad.h>
#include <gld/gld.h>
```

This library normally has no effect in release builds. You can enable gld.h in release builds by defining `GLD_RELEASE` before including the header.

```cpp
#include <glad/glad.h>
#define GLD_RELEASE
#include <gld/gld.h>
```

To use the features of this library, call your OpenGL functions with gld in front instead of gl:

```cpp
gldActiveTexture(GL_TEXTURE0);
```

For Opengl functions that return something, you now have to pass the variable you want to return to as first argument. 

```cpp
GLuint gl_program;
gldCreateProgram(gl_program);
```

## How It Works

When configuring this CMake project, a python script is run which analyzes the official OpenGL specification xml document (gl.xml) and generates macros for each OpenGL function in the header gld/gld.h. These macros appear to be exactly like normal gl calls, except they begin with gld instead of gl.