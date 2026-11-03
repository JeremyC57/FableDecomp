// EnlargedUnsignedDivide(ULARGE_INTEGER Dividend, ULONG Divisor, ULONG* Remainder) -- DirectShow BaseClasses (DirectX 9 SDK
// Samples\C++\DirectShow\BaseClasses\wxutil.h): 64-by-32-bit unsigned divide returning the 32-bit quotient and, when
// Remainder is not NULL, the remainder. The vendor header's `__inline` function, verbatim: the `#ifdef _X86_` inline-
// assembler body is the byte-matching path. The includer supplies the Win32 types (ULONG, ULARGE_INTEGER, WINAPI) as
// <windows.h> does for the vendor header.
// Portable-check-prelude: prelude_win32_types.h
// Retail-asm-helper: EnlargedUnsignedDivide
// Retail-asm: 0x00CB0164..0x00CB0176 `mov eax,[ebp+8] ; mov edx,[ebp+0Ch] ; mov ecx,[ebp+14h] ; div dword ptr [ebp+10h] ;
//   or ecx,ecx ; jz ; mov [ecx],edx ; mov [ebp-4],eax` (the out-of-line copy 0x00CB0160) and the two expansions inside
//   Int64x32Div32 0x00CB0892 / 0x00CB08BA: the published wxutil.h block. VC7.1 C++ `ULONGLONG / ULONG` calls _aulldiv
//   (a 64-by-64 divide); a single 64-by-32 `div` is reachable only from the asm.
// Semantics: quotient = Dividend / Divisor, remainder = Dividend % Divisor, valid under the vendor's precondition
//   Dividend.HighPart < Divisor (the quotient fits 32 bits; wxutil.cpp guards every call). Outside it `div` raises the
//   divide-error exception (#DE, also for Divisor == 0); the twin then returns the low 32 bits of the quotient (and is
//   undefined for Divisor == 0) -- callers never get there.
// Portable twin (FABLE_PORTABLE): Strategy B, plain C++ 64-bit division; tests in
//   rebuild/tests/portable/test_EnlargedUnsignedDivide.cpp.
#ifndef FABLE_STRMBASE_ENLARGEDUNSIGNEDDIVIDE_H
#define FABLE_STRMBASE_ENLARGEDUNSIGNEDDIVIDE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline ULONG EnlargedUnsignedDivide(ULARGE_INTEGER Dividend, ULONG Divisor, ULONG* Remainder)
    {
        ULONG ulResult = (ULONG)(Dividend.QuadPart / Divisor);
        if (Remainder)
            *Remainder = (ULONG)(Dividend.QuadPart % Divisor);
        return ulResult;
    }
}
#endif

#ifdef FABLE_PORTABLE
using fable_portable::EnlargedUnsignedDivide;
#else
__inline ULONG WINAPI EnlargedUnsignedDivide (
    ULARGE_INTEGER Dividend,
    ULONG Divisor,
    ULONG* Remainder
    )
{
    ULONG ulResult;
    __asm {
        mov eax, Dividend.LowPart
        mov edx, Dividend.HighPart
        mov ecx, Remainder
        div Divisor
        or  ecx, ecx
        jz  short label
        mov [ecx], edx
label:
        mov ulResult, eax
    }
    return ulResult;
}
#endif

#endif  // FABLE_STRMBASE_ENLARGEDUNSIGNEDDIVIDE_H
