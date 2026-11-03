// test_EnlargedUnsignedDivide.cpp -- dual-path test of strmbase/EnlargedUnsignedDivide.h (DirectShow wxutil.h 64-by-32
// unsigned divide). Inputs satisfy the vendor precondition (Dividend.HighPart < Divisor, Divisor != 0): outside it the asm
// raises #DE. Expected values are plain 64-bit arithmetic, which is the documented semantics; the dual-path build also
// compares the asm path with fable_portable::EnlargedUnsignedDivide input by input.
#include <stdio.h>
#include "prelude_win32_types.h"
#include "strmbase/EnlargedUnsignedDivide.h"

struct Case { unsigned int hi, lo, divisor; };
static const Case kCases[] = {
    {0u, 0u, 1u}, {0u, 1u, 1u}, {0u, 0xFFFFFFFFu, 1u}, {0u, 0xFFFFFFFFu, 0xFFFFFFFFu}, {0u, 12345u, 7u},
    {1u, 0u, 2u}, {1u, 0u, 3u}, {6u, 0x89ABCDEFu, 7u}, {0x7FFFFFFFu, 0xFFFFFFFFu, 0x80000000u},
    {0xFFFFFFFEu, 0xFFFFFFFFu, 0xFFFFFFFFu}, {0x12345678u, 0x9ABCDEF0u, 0x87654321u}, {0u, 1000000u, 1000u},
    {0x000F4240u, 0x00000000u, 0x3B9ACA00u}, {2u, 0x80000000u, 3u}
};

int main()
{
    int n = (int)(sizeof kCases / sizeof kCases[0]), bad = 0;
    for (int i = 0; i < n; ++i) {
        ULARGE_INTEGER d;
        d.QuadPart = ((ULONGLONG)kCases[i].hi << 32) | kCases[i].lo;
        ULONG wantQ = (ULONG)(d.QuadPart / kCases[i].divisor), wantR = (ULONG)(d.QuadPart % kCases[i].divisor);
        ULONG rem = 0xDEADBEEFu;
        ULONG q = EnlargedUnsignedDivide(d, kCases[i].divisor, &rem);
        ULONG q2 = EnlargedUnsignedDivide(d, kCases[i].divisor, 0);   // NULL remainder: nothing stored
        if (q != wantQ || rem != wantR || q2 != wantQ) {
            printf("  case %d: q %08X r %08X (want %08X %08X)\n", i, (unsigned)q, (unsigned)rem, (unsigned)wantQ, (unsigned)wantR);
            ++bad;
        }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
        ULONG trem = 0xDEADBEEFu;
        ULONG tq = fable_portable::EnlargedUnsignedDivide(d, kCases[i].divisor, &trem);
        if (tq != q || trem != rem) {
            printf("  case %d: asm %08X/%08X, portable twin %08X/%08X\n", i, (unsigned)q, (unsigned)rem, (unsigned)tq, (unsigned)trem);
            ++bad;
        }
#endif
    }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    const char* mode = "asm+portable";
#elif defined(FABLE_PORTABLE)
    const char* mode = "portable";
#else
    const char* mode = "asm";
#endif
    printf("test_EnlargedUnsignedDivide %s: %s (%d cases)\n", mode, bad ? "FAIL" : "PASS", n);
    return bad ? 1 : 0;
}
