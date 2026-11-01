// llMulDiv(LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG d) -- DirectShow BaseClasses wxutil.cpp: (a * b + d) / c in 128-bit intermediate precision with the sign tracked separately, saturating to the LONGLONG limits on overflow or c == 0
// Leaf: ?llMulDiv@@YG_J_J000@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB0370, 779 bytes, WINAPI (`__stdcall`, `ret 20h`), 0x28-byte frame, no /GS cookie.
// Identity: the DirectX 9 SDK BaseClasses source (strmbase compiled into the exe, GOTCHAS 0x00CA4xxx-0x00CA7xxx and chunk 594):
//   the published llMulDiv body with USE_LARGEINT undefined -- UInt32x32To64 is the 64x64 `_allmul` 0x00BFEB50 on zero-extended
//   words, `p[0].QuadPart / uc` is `_aulldiv` 0x00BFEE00, the DWORD-divisor arm's `/` and `%` pair is one `_aulldvrm` 0x00F3FD50,
//   and the 64-step long division loop (`shld`) closes it. Sole caller 0x00CA5FD0 (CBaseVideoRenderer::GetStdDev region,
//   landed). The catalog label "Math_Divide64bit" is a stand-in; the neighbours are wxutil.cpp (EnlargedUnsignedDivide 0x00CB0160
//   landed, Int64x32Div32 next). No PDB symbol (library source); the SDK name is the function's own.
// Shape: the SDK text verbatim; VC7.1 reuses the dead argument slots (a.LowPart for bSign, b for uc's words).
typedef long LONG;
typedef unsigned long DWORD;
typedef int BOOL;
typedef __int64 LONGLONG;
typedef unsigned __int64 DWORDLONG;
typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    struct { DWORD LowPart; DWORD HighPart; } u;
    DWORDLONG QuadPart;
} ULARGE_INTEGER;
#define WINAPI __stdcall
#define UInt32x32To64(a, b) ((DWORDLONG)((DWORD)(a)) * (DWORDLONG)((DWORD)(b)))

/* Compute (a * b + d) / c */
LONGLONG WINAPI llMulDiv(LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG d)
{
    /* Compute the absolute values to avoid signed arithmetic problems */
    ULARGE_INTEGER ua, ub;
    DWORDLONG uc;

    ua.QuadPart = (DWORDLONG)(a >= 0 ? a : -a);
    ub.QuadPart = (DWORDLONG)(b >= 0 ? b : -b);
    uc          = (DWORDLONG)(c >= 0 ? c : -c);
    BOOL bSign = (a < 0) ^ (b < 0);

    /* Do long multiplication */
    ULARGE_INTEGER p[2];
    p[0].QuadPart  = UInt32x32To64(ua.LowPart, ub.LowPart);

    /* This next computation cannot overflow into p[1].HighPart because
       the max number we can compute here is:

                 (2 ** 32 - 1) * (2 ** 32 - 1) +  // ua.LowPart * ub.LowPart
    (2 ** 32) * (2 ** 31) * (2 ** 32 - 1) * 2     // x.LowPart * y.HighPart * 2

    == 2 ** 96 - 2 ** 64 + (2 ** 64 - 2 ** 33 + 1)
    == 2 ** 96 - 2 ** 33 + 1
    < 2 ** 96
    */

    ULARGE_INTEGER x;
    x.QuadPart     = UInt32x32To64(ua.LowPart, ub.HighPart) +
                     UInt32x32To64(ua.HighPart, ub.LowPart) +
                     p[0].HighPart;
    p[0].HighPart  = x.LowPart;
    p[1].QuadPart  = UInt32x32To64(ua.HighPart, ub.HighPart) + x.HighPart;

    if (d != 0) {
        ULARGE_INTEGER ud[2];
        if (bSign) {
            ud[0].QuadPart = (DWORDLONG)(-d);
            if (d > 0) {
                /*  -d < 0 */
                ud[1].QuadPart = (DWORDLONG)(-1);
            } else {
                ud[1].QuadPart = (DWORDLONG)0;
            }
        } else {
            ud[0].QuadPart = (DWORDLONG)d;
            if (d < 0) {
                ud[1].QuadPart = (DWORDLONG)(-1);
            } else {
                ud[1].QuadPart = (DWORDLONG)0;
            }
        }
        /* Now do extended addition */
        ULARGE_INTEGER uliTotal;

        /* Add ls DWORDs */
        uliTotal.QuadPart  = (DWORDLONG)ud[0].LowPart + p[0].LowPart;
        p[0].LowPart       = uliTotal.LowPart;

        /* Propagate carry */
        uliTotal.LowPart   = uliTotal.HighPart;
        uliTotal.HighPart  = 0;

        /* Add 2nd most ls DWORDs */
        uliTotal.QuadPart += (DWORDLONG)ud[0].HighPart + p[0].HighPart;
        p[0].HighPart      = uliTotal.LowPart;

        /* Propagate carry */
        uliTotal.LowPart   = uliTotal.HighPart;
        uliTotal.HighPart  = 0;

        /* Add MS DWORDLONGs - no carry expected */
        p[1].QuadPart     += ud[1].QuadPart + uliTotal.QuadPart;

        /* Now see if we got a sign change from the addition */
        if ((LONG)p[1].HighPart < 0) {
            bSign = !bSign;

            /* Negate the current value (ugh!) */
            p[0].QuadPart  = ~p[0].QuadPart;
            p[1].QuadPart  = ~p[1].QuadPart;
            p[0].QuadPart += 1;
            p[1].QuadPart += (p[0].QuadPart == 0);
        }
    }

    /* Now for the division */
    if (c < 0) {
        bSign = !bSign;
    }


    /* This will catch c == 0 and overflow */
    if (uc <= p[1].QuadPart) {
        return bSign ? (LONGLONG)0x8000000000000000 :
                       (LONGLONG)0x7FFFFFFFFFFFFFFF;
    }

    DWORDLONG ullResult;

    /* Do the division */
    /* If the dividend is a DWORD_LONG use the compiler */
    if (p[1].QuadPart == 0) {
        ullResult = p[0].QuadPart / uc;
        return (LONGLONG)(bSign ? -(LONGLONG)ullResult : ullResult);
    }

    /* If the divisor is a DWORD then its simpler */
    ULARGE_INTEGER ulic;
    ulic.QuadPart = uc;
    if (ulic.HighPart == 0) {
        ULARGE_INTEGER uliDividend;
        ULARGE_INTEGER uliResult;
        DWORD dwDivisor = (DWORD)uc;
        // ASSERT(p[1].HighPart == 0 && p[1].LowPart < dwDivisor);
        uliDividend.HighPart = p[1].LowPart;
        uliDividend.LowPart = p[0].HighPart;
#ifndef USE_LARGEINT
        uliResult.HighPart = (DWORD)(uliDividend.QuadPart / dwDivisor);
        p[0].HighPart = (DWORD)(uliDividend.QuadPart % dwDivisor);
        uliResult.LowPart = 0;
        uliResult.QuadPart = p[0].QuadPart / dwDivisor + uliResult.QuadPart;
#else
        /* NOTE - this routine will take exceptions if
           the result does not fit in a DWORD
        */
        if (uliDividend.QuadPart != 0) {
            uliResult.HighPart = EnlargedUnsignedDivide(
                                     uliDividend,
                                     dwDivisor,
                                     &p[0].HighPart);
        } else {
            uliResult.HighPart = 0;
        }
        uliResult.LowPart = EnlargedUnsignedDivide(
                                 p[0],
                                 dwDivisor,
                                 NULL);
#endif
        return (LONGLONG)(bSign ? -(LONGLONG)uliResult.QuadPart :
                                  uliResult.QuadPart);
    }


    ullResult = 0;
    int i;

    /* OK - do long division */
    for (i = 0; i < 64; i++) {
        ullResult <<= 1;

        /* Shift 128 bit p left 1 */
        p[1].QuadPart <<= 1;
        if ((p[0].HighPart & 0x80000000) != 0) {
            p[1].LowPart++;
        }
        p[0].QuadPart <<= 1;

        /* Compare */
        if (uc <= p[1].QuadPart) {
            p[1].QuadPart -= uc;
            ullResult += 1;
        }
    }

    return (LONGLONG)(bSign ? - (LONGLONG)ullResult : ullResult);
}