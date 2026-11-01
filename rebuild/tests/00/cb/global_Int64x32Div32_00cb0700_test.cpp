// Behaviour test for 0x00CB0700 Int64x32Div32 (DirectShow wxutil.cpp): the SDK body with EnlargedUnsignedDivide's div in C.
#include <cstdio>
#ifndef _MSC_VER
#define __int64 long long
#endif
typedef int LONG;
typedef unsigned int DWORD;
typedef unsigned int ULONG;
typedef int BOOL;
typedef __int64 LONGLONG;
typedef unsigned __int64 DWORDLONG;
typedef unsigned __int64 ULONGLONG;
typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    struct { DWORD LowPart; DWORD HighPart; } u;
    DWORDLONG QuadPart;
} ULARGE_INTEGER;
#define WINAPI
#define UInt32x32To64(a, b) ((DWORDLONG)((DWORD)(a)) * (DWORDLONG)((DWORD)(b)))
#define IN
#define NULL 0

static ULONG EnlargedUnsignedDivide(ULARGE_INTEGER Dividend, ULONG Divisor, ULONG* Remainder)
{
    // the asm's div: the quotient fits 32 bits (the callers guarantee it)
    if (Remainder) *Remainder = (ULONG)(Dividend.QuadPart % Divisor);
    return (ULONG)(Dividend.QuadPart / Divisor);
}

/* Compute (a * b + rnd) / c */
LONGLONG WINAPI Int64x32Div32(LONGLONG a, LONG b, LONG c, LONG d)
{
    ULARGE_INTEGER ua;
    DWORD ub;
    DWORD uc;

    /*  Compute the absolute values to avoid signed arithmetic problems */
    ua.QuadPart = (DWORDLONG)(a >= 0 ? a : -a);
    ub = (DWORD)(b >= 0 ? b : -b);
    uc = (DWORD)(c >= 0 ? c : -c);
    BOOL bSign = (a < 0) ^ (b < 0);

    /*  Do long multiplication */
    ULARGE_INTEGER p0;
    DWORD p1;
    p0.QuadPart  = UInt32x32To64(ua.LowPart, ub);

    if (ua.HighPart != 0) {
        ULARGE_INTEGER x;
        x.QuadPart     = UInt32x32To64(ua.HighPart, ub) + p0.HighPart;
        p0.HighPart  = x.LowPart;
        p1   = x.HighPart;
    } else {
        p1 = 0;
    }

    if (d != 0) {
        ULARGE_INTEGER ud0;
        DWORD ud1;

        if (bSign) {
            //
            //  Cast d to LONGLONG first otherwise -0x80000000 sign extends
            //  incorrectly
            //
            ud0.QuadPart = (DWORDLONG)(-(LONGLONG)d);
            if (d > 0) {
                /*  -d < 0 */
                ud1 = (DWORD)-1;
            } else {
                ud1 = (DWORD)0;
            }
        } else {
            ud0.QuadPart = (DWORDLONG)d;
            if (d < 0) {
                ud1 = (DWORD)-1;
            } else {
                ud1 = (DWORD)0;
            }
        }
        /*  Now do extended addition */
        ULARGE_INTEGER uliTotal;

        /* Add ls DWORDs */
        uliTotal.QuadPart  = (DWORDLONG)ud0.LowPart + p0.LowPart;
        p0.LowPart       = uliTotal.LowPart;

        /* Propagate carry */
        uliTotal.LowPart   = uliTotal.HighPart;
        uliTotal.HighPart  = 0;

        /* Add 2nd most ls DWORDs */
        uliTotal.QuadPart += (DWORDLONG)ud0.HighPart + p0.HighPart;
        p0.HighPart      = uliTotal.LowPart;

        /* Add MS DWORDLONGs - no carry expected */
        p1 += ud1 + uliTotal.HighPart;

        /* Now see if we got a sign change from the addition */
        if ((LONG)p1 < 0) {
            bSign = !bSign;

            /* Negate the current value (ugh!) */
            p0.QuadPart  = ~p0.QuadPart;
            p1 = ~p1;
            p0.QuadPart += 1;
            p1 += (p0.QuadPart == 0);
        }
    }

    /* Now for the division */
    if (c < 0) {
        bSign = !bSign;
    }


    /* This will catch c == 0 and overflow */
    if (uc <= p1) {
        return bSign ? (LONGLONG)0x8000000000000000 :
                       (LONGLONG)0x7FFFFFFFFFFFFFFF;
    }

    /*  Do the division */

    /*  We can do the division in 2 steps */
    ULARGE_INTEGER uliDividend;
    ULARGE_INTEGER uliResult;
    DWORD dwDivisor = uc;
    uliDividend.HighPart = p1;
    uliDividend.LowPart = p0.HighPart;
    /* NOTE - this routine will take exceptions if
       the result does not fit in a DWORD
    */
    if (uliDividend.QuadPart >= (DWORDLONG)dwDivisor) {
        uliResult.HighPart = EnlargedUnsignedDivide(
                                 uliDividend,
                                 dwDivisor,
                                 &p0.HighPart);
    } else {
        uliResult.HighPart = 0;
    }
    uliResult.LowPart = EnlargedUnsignedDivide(
                             p0,
                             dwDivisor,
                             NULL);
    return bSign ? -(LONGLONG)uliResult.QuadPart :
                    (LONGLONG)uliResult.QuadPart;
}

int main()
{
    static const struct { LONGLONG a; LONG b, c, d; LONGLONG want; } k[] = {
        { 10, 3, 2, 0, 15 }, { -10, 3, 2, 0, -15 }, { 10, -3, 2, 0, -15 }, { 10, 3, -2, 0, -15 },
        { 7, 1, 2, 1, 4 }, { -7, 1, 2, -1, -4 }, { 1000000000000, 1000, 7, 3, 142857142857143 },
        { 5, 5, 0, 0, 0x7FFFFFFFFFFFFFFF }, { -5, 5, 0, 0, (LONGLONG)0x8000000000000000 },
        { 0x7FFFFFFFFFFFFFFF, 4, 1, 0, 0x7FFFFFFFFFFFFFFF },
        { 0x123456789A, 0x10000, 0x3000, 0, 0x123456789A * 0x10000 / 0x3000 },
    };
    for (unsigned i = 0; i < sizeof(k) / sizeof(k[0]); i++)
        if (Int64x32Div32(k[i].a, k[i].b, k[i].c, k[i].d) != k[i].want) { std::printf("00cb0700_TEST FAIL\n"); return 1; }
    std::printf("00cb0700_TEST PASS\n");
    return 0;
}