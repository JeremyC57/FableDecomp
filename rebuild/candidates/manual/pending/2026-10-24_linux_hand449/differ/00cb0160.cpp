// EnlargedUnsignedDivide(ULARGE_INTEGER, ULONG, ULONG*) -- DirectShow BaseClasses (wxutil.h): 64-by-32-bit unsigned divide returning the quotient and, when asked, the remainder
// Grade: DIFFER(53v34) -- retail is the wxutil.h x86 inline-assembler block (one `div dword [ebp+10h]`, `or ecx,ecx` remainder test, ulResult homed at [ebp-4], frame pointer from the block); C++ can only call _aullrem/_aulldiv. Retail-inline-assembler candidate: land as grade retail-asm with the minimal block.
// Leaf: ?EnlargedUnsignedDivide@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB0160, 34 bytes (+14 int3), __stdcall (WINAPI), `ret 10h`.
// Evidence: the body is `push ebp; mov ebp,esp; push ecx; mov eax,[ebp+8]; mov edx,[ebp+0Ch]; mov ecx,[ebp+14h];
// div dword [ebp+10h]; or ecx,ecx; jz; mov [ecx],edx; mov [ebp-4],eax; mov eax,[ebp-4]` -- the DX9 wxutil.h
// `#ifdef _X86_` version (an inline-assembler block ending `mov ulResult,eax`, then `return ulResult;`). VC7.1 cannot emit
// a 64/32 `div` from C++ (it calls the CRT long-division helpers), so this row is a retail-asm candidate; the body below
// is the C++ reference version (the non-x86 branch of wxutil.h), kept as the behaviour oracle.
// Manifest label "EnlargedUnsignedDivide" is correct.
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef unsigned __int64 ULONGLONG;
typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    struct { DWORD LowPart; DWORD HighPart; } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER;

ULONG __stdcall
EnlargedUnsignedDivide (
    ULARGE_INTEGER Dividend,
    ULONG Divisor,
    ULONG* Remainder
    )
{
    // return remainder if necessary
    if (Remainder != 0)
        *Remainder = (ULONG)(Dividend.QuadPart % Divisor);
    return (ULONG)(Dividend.QuadPart / Divisor);
}
