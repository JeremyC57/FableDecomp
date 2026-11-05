// STRMBASE_MEMMOVE_FORWARD(dst, src, count) / STRMBASE_MEMMOVE_BACKWARD(dst, src, count) -- the two `_asm` blocks of the
// DirectShow BaseClasses wxutil.cpp memmoveInternal (DX9 SDK, `#ifdef _X86_` branch), linked into Fable.exe at 0x00CB0310.
// Retail-asm-helper: STRMBASE_MEMMOVE_FORWARD, STRMBASE_MEMMOVE_BACKWARD
// Retail-asm: 0x00CB0331 `std` / 0x00CB033A `cld` / 0x00CB034A `cld` and 0x00CB0353 `rep movsd` (memmoveInternal 0x00CB0310):
//   VC7.1 has no std / cld / rep movs intrinsic (the __movsb family arrived with VC8), and wxutil.cpp writes this function as
//   two `_asm { ... }` blocks under `#ifdef _X86_`.
// Semantics: FORWARD copies count bytes from src to dst, lower addresses first (count/4 dwords, then count%4 bytes); BACKWARD
//   copies count bytes highest address first (byte by byte), then clears the direction flag. Both leave dst / src / count
//   unchanged; the caller picks BACKWARD when dst overlaps the tail of src.
// Portable twin (FABLE_PORTABLE): the same two copy orders in plain C++ (wxutil.cpp's non-_X86_ branch is one memmove); test
//   rebuild/tests/portable/test_memmoveInternal.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_STRMBASE_MEMMOVEINTERNAL_H
#define FABLE_STRMBASE_MEMMOVEINTERNAL_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void MemmoveForward(void* dst, const void* src, unsigned int count)
    {
        unsigned char* d = (unsigned char*)dst;
        const unsigned char* s = (const unsigned char*)src;
        for (unsigned int i = 0; i < count; ++i)
            d[i] = s[i];
    }
    inline void MemmoveBackward(void* dst, const void* src, unsigned int count)
    {
        unsigned char* d = (unsigned char*)dst;
        const unsigned char* s = (const unsigned char*)src;
        for (unsigned int i = count; i != 0; --i)
            d[i - 1] = s[i - 1];
    }
}
#endif

#ifdef FABLE_PORTABLE
#define STRMBASE_MEMMOVE_FORWARD(dst, src, count) fable_portable::MemmoveForward((dst), (src), (count))
#define STRMBASE_MEMMOVE_BACKWARD(dst, src, count) fable_portable::MemmoveBackward((dst), (src), (count))
#else
#define STRMBASE_MEMMOVE_FORWARD(dst, src, count) \
    __asm mov esi, src \
    __asm mov edi, dst \
    __asm mov ecx, count \
    __asm cld \
    __asm mov edx, ecx \
    __asm and edx, 3 \
    __asm shr ecx, 2 \
    __asm rep movsd \
    __asm or ecx, edx \
    __asm jz strmbase_memmove_done \
    __asm rep movsb \
    __asm strmbase_memmove_done:
#define STRMBASE_MEMMOVE_BACKWARD(dst, src, count) \
    __asm mov esi, src \
    __asm mov edi, dst \
    __asm mov ecx, count \
    __asm std \
    __asm add esi, ecx \
    __asm add edi, ecx \
    __asm dec esi \
    __asm dec edi \
    __asm rep movsb \
    __asm cld
#endif

#endif  // FABLE_STRMBASE_MEMMOVEINTERNAL_H
