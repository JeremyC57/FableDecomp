#include <cstdio>
// behaviour test of EnlargedUnsignedDivide (0x00CB0160): 64-by-32 unsigned divide with an optional remainder
typedef unsigned long ULONG;
#ifdef _MSC_VER
typedef unsigned __int64 ULONGLONG;
typedef union _ULARGE_INTEGER {
    struct { ULONG LowPart; ULONG HighPart; };
    ULONGLONG QuadPart;
} ULARGE_INTEGER;
#pragma optimize("y", off)
ULONG __stdcall EnlargedUnsignedDivide(ULARGE_INTEGER Dividend, ULONG Divisor, ULONG* Remainder)
{
    ULONG ulResult;
    _asm {
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
#else
typedef unsigned long long ULONGLONG;
typedef union _ULARGE_INTEGER {
    struct { unsigned int LowPart; unsigned int HighPart; };
    ULONGLONG QuadPart;
} ULARGE_INTEGER;
ULONG EnlargedUnsignedDivide(ULARGE_INTEGER Dividend, ULONG Divisor, ULONG* Remainder)
{
    ULONG ulResult = (ULONG)(Dividend.QuadPart / Divisor);
    if (Remainder)
        *Remainder = (ULONG)(Dividend.QuadPart % Divisor);
    return ulResult;
}
#endif
int main()
{
    ULARGE_INTEGER d;
    ULONG rem = 12345;
    d.QuadPart = 0x100000005ull;                       // 4294967301
    bool ok = EnlargedUnsignedDivide(d, 16, &rem) == 0x10000000ul && rem == 5;
    d.QuadPart = 1000000007ull;
    ok = ok && EnlargedUnsignedDivide(d, 1000, 0) == 1000000ul;   // a null remainder pointer is skipped
    rem = 77;
    d.QuadPart = 0x7fffffff0ull;                       // 34359738352 / 1000000000 = 34 r 359738352
    ok = ok && EnlargedUnsignedDivide(d, 1000000000ul, &rem) == 34 && rem == 359738352ul;
    std::printf(ok ? "00cb0160_TEST PASS\n" : "00cb0160_TEST FAIL\n");
    return ok ? 0 : 1;
}