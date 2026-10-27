// EnlargedUnsignedDivide(ULARGE_INTEGER Dividend, ULONG Divisor, ULONG* Remainder) -- DirectShow BaseClasses (wxutil.h): 64-by-32-bit unsigned divide returning the quotient and, when asked, the remainder
// Leaf: ?EnlargedUnsignedDivide@@
// Retail-asm: 0x00CB0164..0x00CB0176 `mov eax,[ebp+8] ; mov edx,[ebp+0Ch] ; mov ecx,[ebp+14h] ; div dword ptr [ebp+10h] ; or ecx,ecx ; jz ; mov [ecx],edx ; mov [ebp-4],eax` -- the published `#ifdef _X86_` inline-assembler block of the DirectX 9 SDK wxutil.h (spelt `__asm` here so the purity check sees it; the SDK writes `_asm`) (8 instructions, one jz: rasm1 family (h)); VC7.1 compiles a 64/32 `/` and `%` to the CRT __aulldiv/__aullrem helpers, never to one `div`. The `return ulResult` is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB0160, 34 bytes (+14 int3), __stdcall (WINAPI), `ret 10h`; the asm over the local forces the
// `push ebp ; mov ebp,esp ; push ecx` frame. Manifest label "EnlargedUnsignedDivide" is correct (the BaseClasses source names the
// parameters Dividend, Divisor, Remainder and the local ulResult).
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef unsigned __int64 ULONGLONG;
typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    struct { DWORD LowPart; DWORD HighPart; } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER;
#pragma optimize("y", off)
ULONG __stdcall
EnlargedUnsignedDivide (
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