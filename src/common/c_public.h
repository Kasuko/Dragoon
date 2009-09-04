/******************************************************************************\
 Dragoon - Copyright (C) 2008 - Michael Levin

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
\******************************************************************************/

/* Package configuration header (generated by SConstruct) */
#include "config.h"

/* Standard library */
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <time.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

/* OpenGL */
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glext.h"

/* SDL */
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_ttf.h"

/* libpng */
#include <png.h>

/* Ensure common definitions */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL ((void*)0)
#endif
#ifndef NUL
#define NUL '\0'
#endif

/* Single-precision mathematical constants */
#define C_PI 3.14159265358979323846f
#define C_SQRT2 1.414213562373095f

/* Maximum length of a name */
#define C_NAME_MAX 256

/* If you are going to use functions that return pointers to static buffers,
   keep in mind that after calling any of those functions [C_BUFS] times, you
   will begin overwriting the buffers starting from the first. Each buffer also
   has a fixed size. Note that [C_BUFS] * [C_BUF_SIZE] has to be less than
   32k or the function will not compile on some systems. */
#define C_BUFS 16
#define C_BUF_SIZE 2000

/* Define a compact boolean type. This actually does not make much of an impact
   as far as memory usage is concerned but is helpful to denote usage. */
typedef unsigned char bool;

/* Include vector definitions */
#include "c_vectors.h"

/* Generic callback function */
typedef void (*CCallback)(void *);

/* Callback for signal catchers */
typedef void (*CSignalHandler)(int signal);

/* A counter for counting how often something happens per frame */
typedef struct {
        int startFrame, startTime, lastTime;
        float value;
} CCount;

/* Log severity */
typedef enum {
        CLL_DEBUG,
        CLL_WARNING,
        CLL_ERROR,
} CLogLevel;

/* Variable types */
typedef enum {
        CVT_INT,
        CVT_FLOAT,
        CVT_STRING,
} CVarType;

/* Named structure allocation policy for existing objects */
typedef enum {
        CNP_NULL = FALSE,
        CNP_OVERWRITE = TRUE,
        CNP_RETURN,
} CNamedPolicy;

/* Named structure single-linked-list */
typedef struct CNamed {
        char name[C_NAME_MAX];
        CCallback cleanupFunc;
        int size;
        struct CNamed *next;
} CNamed;

/* Generic double-linked list */
typedef struct CLink {
        void *data;
        struct CLink *prev, *next, **root;
} CLink;

/* Resizable arrays */
typedef struct {
        int cap, len, size;
        void *data;
} CArray;

/* CArray.c */
int CArray_append(CArray *, void *item);
void CArray_cleanup(CArray *);
#define CArray_get(a, t, i) (*CArray_p(a, t, i))
void CArray_init(CArray *, size_t itemSize, int cap);
void CArray_insert(CArray *, int index, void *item);
void CArray_remove(CArray *, int index);
void CArray_reserve(CArray *, int n);
#define CArray_p(a, t, i) (((t *)(a)->data) + i)
#define CArray_set(a, t, i, value) (CArray_get(a, t, i) = value)
void CArray_set_copy(CArray *, int index, void *item);
void *CArray_steal(CArray *);
void CArray_sort(CArray *, int (*compare)(const void *, const void *));

/* c_file.c */
void C_closeBrace(FILE *);
bool C_fileExists(const char *name);
FILE *C_fopen_full(const char *func, const char *filename, bool write);
#define C_fopen_read(f) C_fopen_full(__func__, (f), FALSE)
#define C_fopen_write(f) C_fopen_full(__func__, (f), TRUE)
#define C_fread(f, p, s) fread(p, 1, s, f)
#define C_fwrite(f, p, s) fwrite(p, 1, s, f)
bool C_openBrace(FILE *);
char *C_readString(FILE *, int *length);
const char *C_token(FILE *);
#define C_token_int(f) atoi(C_token(f))
#define C_token_float(f) atof(C_token(f))
CVec C_token_vec(FILE *);

/* CLink.c */
void CLink_add(CLink *, CLink **root, void *data);
void CLink_add_rear(CLink *, CLink **root, void *data);
void CLink_back(CLink *);
void CLink_forward(CLink *);
#define CLink_get(l) ((l) ? (l)->data : NULL)
#define CLink_linked(l) ((l)->root != NULL)
#define CLink_next(l) ((l) ? (l)->next : NULL)
#define CLink_prev(l) ((l) ? (l)->prev : NULL)
void CLink_remove(CLink *);

/* c_log.c */
#if CHECKED
#define C_assert(s) C_assert_full((int)(s), __func__, #s)
void C_assert_full(int eval, const char *func, const char *statement);
void C_dump(const char *file, const char *fmt, ...);
#define C_debug(fmt, ...) C_log(CLL_DEBUG, __func__, fmt, ## __VA_ARGS__)
#else
#define C_assert(s)
#define C_debug(...)
#define C_dump(...)
#endif
void C_log(CLogLevel, const char *file, const char *fmt, ...);
#define C_error(fmt, ...) C_log(CLL_ERROR, __func__, fmt, ## __VA_ARGS__)
#define C_warning(fmt, ...) C_log(CLL_WARNING, __func__, fmt, ## __VA_ARGS__)
#define TODO C_log(CLL_ERROR, __func__, "Not implemented");

/* c_math.c */
#define C_degToRad(a) ((a) * C_PI / 180.f)
#define C_fade(v, u, r) C_fade_full(v, 0, 1, u, r)
bool C_fade_full(float *value, float from, float to, bool up, float rate);
#define C_isPow2(n) !(n & (n - 1))
void C_limit_float(float *, float min, float max);
void C_limit_int(int *, int min, int max);
int C_nextPow2(int);
void C_quadratic(float a, float b, float c, float *x1, float *x2);
#define C_radToDeg(a) ((a) * 180.f / C_PI)
#define C_rand() ((float)rand() / RAND_MAX)
#define C_rand_c() (2 * C_rand() - 1)
int C_randDet(int);
int C_rollDice(int num, int sides);
CVec C_shake(float *amount, float decay, float sec);
#define C_sign(n) ((n) < 0 ? -1 : (n) > 0)

/* c_memory.c */
#if CHECKED
#define C_calloc(s) C_recalloc_full(__func__, NULL, s)
void C_checkLeaks(void);
#define C_free(p) C_free_full(__func__, p)
void C_free_full(const char *func, void *ptr);
#define C_malloc(s) C_realloc(NULL, s)
#define C_realloc(p, s) C_realloc_full(__func__, p, s)
void *C_realloc_full(const char *func, void *ptr, size_t size);
void C_testMemCheck(int test);
#else
#define C_calloc(s) calloc(s, 1)
#define C_calloc_full(f, s) calloc(s, 1)
#define C_checkLeaks()
#define C_free(p) free(p)
#define C_malloc(s) malloc(s)
#define C_realloc(a, b) realloc(a, b)
#define C_realloc_full(f, a, b) realloc(a, b)
#define C_testMemCheck(a)
#endif
#define C_new(p) (*(p) = C_calloc(sizeof (**(p))))
#define C_one(s) memset(s, -1, sizeof (*(s)))
#define C_one_buf(s) memset(s, -1, sizeof (s))
void *C_recalloc_full(const char *func, void *ptr, size_t size);
#define C_zero(s) memset(s, 0, sizeof (*(s)))
#define C_zero_buf(s) memset(s, 0, sizeof (s))

/* CNamed.c */
void CNamed_free(CNamed *);
int CNamed_freeAll(CNamed **root);
#define CNamed_alloc(r, n, s, c, o) CNamed_alloc_full(__func__, r, n, s, c, o)
void *CNamed_alloc_full(const char *func, CNamed **root, const char *name,
                        int size, CCallback cleanupFunc, CNamedPolicy);
void *CNamed_get(CNamed *root, const char *name);

/* c_os_posix.c, c_os_windows.c */
const char *C_appDir(void);
int C_mkdir(const char *path);
const char *C_userDir(void);
void C_signalHandler(CSignalHandler);

/* c_string.c */
char *C_buf(void);
char *C_escape(const char *);
#define C_isComment(c) ((c) == '#')
#define C_isDigit(c) ((c) >= '0' && (c) <= '9')
bool C_isInteger(const char *);
#define C_isPrint(c) ((c) > 0 && (c) < 0x7f)
#define C_isSpace(c) ((c) > 0 && (c) <= ' ')
const char *C_keyName(SDLKey);
#define C_snprintf_buf(buf, fmt, ...) \
        snprintf(buf, sizeof (buf), fmt, ## __VA_ARGS__)
#define C_strncpy(d, s, l) C_strncpy_full(__FILE__, d, s, l)
#define C_strncpy_buf(buf, s) C_strncpy(buf, s, sizeof (buf))
int C_strncpy_full(const char *func, char *dest, const char *src, int len);
int C_utf8Append(char *dest, int *dest_i, int dest_sz, const char *src);
char *C_utf8Encode(wchar_t unicode, int *plen);
char *C_utf8Encode_str(wchar_t *wide, int *plen);
int C_utf8Size(unsigned char first_byte);
char *C_va(const char *fmt, ...);
char *C_van(int *output_len, const char *fmt, ...);
char *C_vanv(int *output_len, const char *fmt, va_list);

/* c_time.c */
#define CCount_add(c, v) ((c)->value += (v))
float CCount_fps(const CCount *);
float CCount_perFrame(const CCount *);
float CCount_perSec(const CCount *);
bool CCount_poll(CCount *, int interval);
void CCount_reset(CCount *);
void C_throttleFps(void);
void C_initTime(void);
void C_updateTime(void);
unsigned int C_timer(void);

extern CCount c_throttled;
extern int c_timeMsec, c_frameMsec, c_frame, c_throttleMsec;
extern float c_frameSec;

/* c_variables.c */
void C_archiveVars(const char *filename);
void C_cleanupVars(void);
#define C_dontArchive(v, ...) C_dontArchive_full(v, ## __VA_ARGS__, NULL)
void C_dontArchive_full(const char *varName, ...);
void C_parseCommandLine(int argc, char *argv[]);
void C_parseVarConfig(const char *filename);
void C_register(void *, const char *name, const char *comment, CVarType,
                int size, bool archive);
#define C_register_buf(p, n, c, a) \
        C_register(p, n, c, CVT_STRING, sizeof (p), a)
#define C_register_int(p, n, c, a) \
        C_register(p, n, c, CVT_INT, sizeof (*(p)), a)
#define C_register_float(p, n, c, a) \
        C_register(p, n, c, CVT_FLOAT, sizeof (*(p)), a)

