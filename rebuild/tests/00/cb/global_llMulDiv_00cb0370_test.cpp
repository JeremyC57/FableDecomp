// Behaviour test for DirectShow wxutil.cpp llMulDiv (0x00CB0370): (a * b + d) / c with 128-bit intermediate precision,
// the sign tracked separately, saturating to the signed 64-bit extremes.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 LONGLONG; typedef unsigned __int64 DWORDLONG;
#else
typedef long long LONGLONG; typedef unsigned long long DWORDLONG;
#endif
typedef unsigned int DWORD; typedef int BOOL;
union ULARGE_INTEGER { struct { DWORD LowPart; DWORD HighPart; } u; DWORDLONG QuadPart; };
#define UInt32x32To64(a, b) ((DWORDLONG)((DWORD)(a)) * (DWORDLONG)((DWORD)(b)))
static LONGLONG llMulDiv(LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG d)
{
    ULARGE_INTEGER ua, ub, uc;
    BOOL bSign = (a < 0);
    ua.QuadPart = (DWORDLONG)(bSign ? -a : a);
    if (b < 0) { bSign = !bSign; ub.QuadPart = (DWORDLONG)(-b); } else ub.QuadPart = (DWORDLONG)b;
    ULARGE_INTEGER p[2];
    DWORDLONG ab_lo_lo = UInt32x32To64(ua.u.LowPart, ub.u.LowPart);
    DWORDLONG ab_lo_hi = UInt32x32To64(ua.u.LowPart, ub.u.HighPart);
    DWORDLONG ab_hi_lo = UInt32x32To64(ua.u.HighPart, ub.u.LowPart);
    DWORDLONG ab_hi_hi = UInt32x32To64(ua.u.HighPart, ub.u.HighPart);
    p[0].QuadPart = ab_lo_lo;
    p[1].QuadPart = ab_hi_hi;
    {
        ULARGE_INTEGER t; t.QuadPart = ab_lo_hi;
        DWORDLONG s = (DWORDLONG)p[0].u.HighPart + t.u.LowPart; p[0].u.HighPart = (DWORD)s;
        p[1].QuadPart += (s >> 32) + t.u.HighPart;
        t.QuadPart = ab_hi_lo;
        s = (DWORDLONG)p[0].u.HighPart + t.u.LowPart; p[0].u.HighPart = (DWORD)s;
        p[1].QuadPart += (s >> 32) + t.u.HighPart;
    }
    if (d != 0) {
        ULARGE_INTEGER ud; ud.QuadPart = (DWORDLONG)(d < 0 ? -d : d);
        if ((d < 0) != (bSign != 0)) {
            if (p[1].QuadPart == 0 && p[0].QuadPart < ud.QuadPart) { p[0].QuadPart = ud.QuadPart - p[0].QuadPart; bSign = !bSign; }
            else { DWORDLONG lo = p[0].QuadPart; p[0].QuadPart -= ud.QuadPart; if (p[0].QuadPart > lo) p[1].QuadPart -= 1; }
        } else {
            DWORDLONG lo = p[0].QuadPart; p[0].QuadPart += ud.QuadPart; if (p[0].QuadPart < lo) p[1].QuadPart += 1;
        }
    }
    if (c < 0) { bSign = !bSign; uc.QuadPart = (DWORDLONG)(-c); } else uc.QuadPart = (DWORDLONG)c;
    if (p[1].QuadPart >= uc.QuadPart) return bSign ? (LONGLONG)0x8000000000000000ULL : (LONGLONG)0x7FFFFFFFFFFFFFFFULL;
    ULARGE_INTEGER ullResult;
    if (p[1].QuadPart == 0) {
        ullResult.QuadPart = p[0].QuadPart / uc.QuadPart;
    } else {
        // 128-by-64 long division, one bit at a time (p[1]:p[0] / uc)
        ullResult.QuadPart = 0;
        DWORDLONG rem = 0;
        for (int i = 127; i >= 0; --i) {
            DWORDLONG hi = p[1].QuadPart, lo = p[0].QuadPart;
            DWORDLONG bit = i >= 64 ? (hi >> (i - 64)) & 1 : (lo >> i) & 1;
            rem = (rem << 1) | bit;
            if (rem >= uc.QuadPart) { rem -= uc.QuadPart; if (i < 64) ullResult.QuadPart |= (DWORDLONG)1 << i; }
        }
    }
    if (ullResult.QuadPart > 0x7FFFFFFFFFFFFFFFULL) return bSign ? (LONGLONG)0x8000000000000000ULL : (LONGLONG)0x7FFFFFFFFFFFFFFFULL;
    return bSign ? -(LONGLONG)ullResult.QuadPart : (LONGLONG)ullResult.QuadPart;
}
static int fail(const char* why) { std::printf("00cb0370_TEST FAIL %s\n", why); return 1; }
int main()
{
    if (llMulDiv(10, 7, 3, 0) != 23) return fail("small");
    if (llMulDiv(-10, 7, 3, 0) != -23) return fail("negative a");
    if (llMulDiv(10, -7, -3, 0) != 23) return fail("double negative");
    if (llMulDiv(10, 7, 3, 2) != 24) return fail("rounding term");
    LONGLONG big = (LONGLONG)1 << 40;
    if (llMulDiv(big, big, big, 0) != big) return fail("128-bit intermediate");
    if (llMulDiv(big, big * 4, big, 0) != big * 4) return fail("128-bit intermediate 2");
    if (llMulDiv(big * 1000, big * 1000, 1, 0) != (LONGLONG)0x7FFFFFFFFFFFFFFFULL) return fail("saturate +");
    if (llMulDiv(-big * 1000, big * 1000, 1, 0) != (LONGLONG)0x8000000000000000ULL) return fail("saturate -");
    if (llMulDiv(90000, 10000000, 1000, 500) != 900000000) return fail("reference time scale");
    std::printf("00cb0370_TEST PASS\n");
    return 0;
}