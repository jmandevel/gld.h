// MIT License
//
// Copyright (c) 2026 Daniel Aimé Valcour
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/*
    gld.h - VERSION 1.0 - A header-only OpenGL debugging library
    ------------------------------------------------------------

    Created By Dan Valcour (jmandevel) - https://github.com/jmandevel


    In order to use this library, include it after including your OpenGL loader 
    (such as GLAD or GLEW).

    Wrap your gl function calls like so:

        GLD_CALL(glGenBuffers(1, buffers));

    For OpenGL funcitons that  return a value, do this:

        GLuint returned_gl_program;
        GLD_CALL_RET(returned_gl_program, glCreateProgram());

    To disable this library, define GLD_DISABLE before including it. By default, 
    this libary has no effect in release builds. If you want this library to
    work in release builds, define GLD_RELEASE before including this header.
*/

#if !defined(GLD_H)
#define GLD_H

#if defined(GLD_DISABLE)

#define GLD_DEBUG_TRAP() ((void)0)
#define GLD_LOG_ERROR() ((void)0)
#define GLD_START() ((void)0)
#define GLD_ERROR(X) X
#define GLD_ERROR_RET(OUT_RET, X) (OUT_RET) = (X)

#else

#if defined(GLD_RELEASE) || !defined(_NDEBUG)

#if !defined(GLD_DEBUG_TRAP)
#if defined(_MSC_VER)
#define GLD_DEBUG_TRAP() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#include <signal.h>
#define GLD_DEBUG_TRAP() raise(SIGTRAP)
#else
#define GLD_DEBUG_BREAK() ((void)0)
#endif
#endif

#if !defined(GLD_LOG_ERROR)
#include <stdio.h>
static int GLD_logErrors(const char *func, const char *file, int line)
{
    GLenum error = glGetError();
    int is_ok = 1;
    while (error != GL_NO_ERROR)
    {
        fprintf(stderr, "[Opengl Error] (0x%04x): %s %s:%d", error, func, file, line);
        is_ok = 0;
    }
    return is_ok;
}
#define GLD_LOG_ERROR(func, file, line) GLD_logError(func, file, line)
#endif

static void GLD_clearErrors(void)
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

#define GLD_CALL(X)                                 \
    do                                              \
    {                                               \
        GLD_clearErrors();                          \
        X;                                          \
        if (!GLD_LOG_ERROR(#X, __FILE__, __LINE__)) \
        {                                           \
            GLD_DEBUG_TRAP();                       \
        }                                           \
    } while (0)

#define GLD_CALL_RET(OUT_RET, X)                    \
    do                                              \
    {                                               \
        GLD_clearErrors();                          \
        (OUT_RET) = (X);                            \
        if (!GLD_LOG_ERROR(#X, __FILE__, __LINE__)) \
        {                                           \
            GLD_DEBUG_TRAP();                       \
        }                                           \
    } while (0)

#endif
#endif

#endif