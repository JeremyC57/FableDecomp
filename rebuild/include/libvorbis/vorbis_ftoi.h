// vorbis_ftoi(double) -- libvorbis (Xiph.Org libVorbis I 20030909, linked into Fable.exe at 0x00C89xxx..0x00CAxxxx): round a
// double to int with the x87 rounding mode. lib/os.h, MSVC branch (`#if defined(_WIN32) && !defined(__GNUC__) &&
// !defined(__BORLANDC__)`): `static __inline int vorbis_ftoi(double f){ int i; __asm{ fld f  fistp i } return i; }`.
// The libvorbis TUs that include os.h keep an out-of-line copy each (link-time code generation made them callee-pops
// `ret 8`): 0x00CA3B10 (vorbis_ftoi, callers 0x00CA3E6D / 0x00CA3EC3 / 0x00CA3F16, the lookup-table interpolators) and
// 0x00C89BE0 (the copy in the vorbisfile TU, just before getdata 0x00C89C10).
// Retail-asm-helper: VORBIS_FTOI
// Retail-asm: 0x00CA3B11 `fld qword ptr [esp+8]` + 0x00CA3B15 `fistp dword ptr [esp]` and 0x00C89BE1 `fld qword ptr [esp+8]`
//   + 0x00C89BE5 `fistp dword ptr [esp]`: a double reloaded with `fld` and stored with a bare dword `fistp` into a `push ecx`
//   word. VC7.1 `(long)f` calls _ftol2, `/QIfist` emits `fistp qword` into an 8-byte slot and /arch:SSE2 cvttsd2si
//   (the batch ATTEMPTS.tsv, family FISTP): the original is the os.h inline asm quoted above.
// Form: a macro (`VORBIS_FTOI(i, f)` names the caller's double and int directly, as os.h's block does; no parameter copy).
// Semantics: fistp under the control word's RC (round to nearest, ties to even; libvorbis never changes RC), NaN / out of
//   range store 0x80000000 (the integer indefinite).
// Portable twin (FABLE_PORTABLE): plain C++ (fable_x87_twins.h X87FistpNear); test rebuild/tests/portable/test_vorbis_ftoi.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_LIBVORBIS_VORBIS_FTOI_H
#define FABLE_LIBVORBIS_VORBIS_FTOI_H
#include "fable_x87_twins.h"

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline long vorbis_ftoi(double f)
    {
        return X87FistpNear(f);
    }
}
#endif

#ifdef FABLE_PORTABLE
#define VORBIS_FTOI(i, f) ((i) = fable_portable::vorbis_ftoi(f))
#else
#define VORBIS_FTOI(i, f) \
    __asm fld f \
    __asm fistp i
#endif

#endif  // FABLE_LIBVORBIS_VORBIS_FTOI_H
