// Int64x32Div32(LONGLONG a, LONG b, LONG c, LONG d) -- DirectShow BaseClasses wxutil.cpp: (a * b + d) / c with 32-bit b, c, d, sign tracked separately, saturating to the LONGLONG limits on overflow or c == 0
// Leaf: ?Int64x32Div32@@YG_J_JJJJ@Z
// Retail-asm: 0x00CB0892..0x00CB08A4 `mov eax,[ebp-8] ; mov edx,[ebp-4] ; mov ecx,[ebp+18h] ; div dword ptr [ebp+14h] ; or ecx,ecx ; jz ; mov [ecx],edx ; mov [ebp+0Ch],eax` and 0x00CB08BA..0x00CB08CE `mov eax,[ebp+8] ; mov edx,[ebp+0Ch] ; mov ecx,0 ; div dword ptr [ebp+14h] ; or ecx,ecx ; jz ; mov [ecx],edx ; mov [ebp+18h],eax` -- the two inlined expansions of the DirectX 9 SDK wxutil.h `__inline` EnlargedUnsignedDivide, whose body is the published `_X86_` inline-assembler block (the out-of-line copy is the landed retail_asm row 0x00CB0160 with the same instructions). VC7.1 compiles a 64/32 `/` to the CRT __aulldiv helper, never to one `div`; the C++ around the block is the SDK text verbatim.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB0700, 493 bytes, WINAPI (`__stdcall`, `ret 14h` = LONGLONG + 3 LONG), ebp frame 0x10, no /GS.
// Identity: the published Int64x32Div32 of strmbase wxutil.cpp (USE_LARGEINT undefined is irrelevant here: this function always uses
//   EnlargedUnsignedDivide): the 32x32 UInt32x32To64 products are `_allmul` 0x00BFEB50 calls on zero-extended words, the
//   `ua.HighPart != 0` second product, the ud0/ud1 extended addition, the 0x8000000000000000 / 0x7FFFFFFFFFFFFFFF saturation and
//   the two-step division with the `uliDividend.QuadPart >= dwDivisor` guard all compile to the retail bytes (first probe). It sits
//   after llMulDiv 0x00CB0370 exactly as in wxutil.cpp (EnlargedUnsignedDivide 0x00CB0160 / llMulDiv / Int64x32Div32). No caller in
//   retail (unreferenced library body). The catalog label "SignedDecimal_Divide" is a forge stand-in. No PDB symbol (library
//   source); the SDK name is the function's own.
typedef long LONG;
typedef unsigned long DWORD;
typedef unsigned long ULONG;
typedef int BOOL;
typedef __int64 LONGLONG;
typedef unsigned __int64 DWORDLONG;
typedef unsigned __int64 ULONGLONG;
typedef union _ULARGE_INTEGER {
    struct { DWORD LowPart; DWORD HighPart; };
    struct { DWORD LowPart; DWORD HighPart; } u;
    DWORDLONG QuadPart;
} ULARGE_INTEGER;
#define WINAPI __stdcall
#define UInt32x32To64(a, b) ((DWORDLONG)((DWORD)(a)) * (DWORDLONG)((DWORD)(b)))
#define IN
#define NULL 0

__inline ULONG WINAPI EnlargedUnsignedDivide (
    IN ULARGE_INTEGER Dividend,
    IN ULONG Divisor,
    IN ULONG* Remainder
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