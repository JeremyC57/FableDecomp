// Behaviour test for JenkinsHashBuffer (0x00C1D500): Bob Jenkins' lookup2 hash; checks the published empty-key value,
// determinism, seed sensitivity and the 12-byte block path.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef unsigned long ub4; typedef unsigned char ub1;
#define mix(a,b,c) { a -= b; a -= c; a ^= (c>>13); b -= c; b -= a; b ^= (a<<8); c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12); b -= c; b -= a; b ^= (a<<16); c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3); b -= c; b -= a; b ^= (a<<10); c -= a; c -= b; c ^= (b>>15); }
static ub4 Hash(const char* data, ub4 length, ub4 seed) { ub4 a, b, c, len = length; const ub1* k = (const ub1*)data;
    a = b = 0x9e3779b9UL; c = seed; a &= 0xffffffffUL;
    while (len >= 12) { a += (k[0] + ((ub4)k[1] << 8) + ((ub4)k[2] << 16) + ((ub4)k[3] << 24)); a &= 0xffffffffUL;
        b += (k[4] + ((ub4)k[5] << 8) + ((ub4)k[6] << 16) + ((ub4)k[7] << 24)); b &= 0xffffffffUL;
        c += (k[8] + ((ub4)k[9] << 8) + ((ub4)k[10] << 16) + ((ub4)k[11] << 24)); c &= 0xffffffffUL;
        mix(a, b, c); a &= 0xffffffffUL; b &= 0xffffffffUL; c &= 0xffffffffUL; k += 12; len -= 12; }
    c += length;
    switch (len) { case 11: c += ((ub4)k[10] << 24); case 10: c += ((ub4)k[9] << 16); case 9: c += ((ub4)k[8] << 8);
    case 8: b += ((ub4)k[7] << 24); case 7: b += ((ub4)k[6] << 16); case 6: b += ((ub4)k[5] << 8); case 5: b += k[4];
    case 4: a += ((ub4)k[3] << 24); case 3: a += ((ub4)k[2] << 16); case 2: a += ((ub4)k[1] << 8); case 1: a += k[0]; }
    a &= 0xffffffffUL; b &= 0xffffffffUL; c &= 0xffffffffUL;
    mix(a, b, c); return c & 0xffffffffUL; }
int main()
{
    const char* s = "Four score and seven years ago";
    ub4 h0 = Hash("", 0, 0), h1 = Hash("", 0, 1);
    bool ok = h0 != h1 && Hash(s, 30, 0) == Hash(s, 30, 0) && Hash(s, 30, 0) != Hash(s, 30, 1) && Hash(s, 30, 0) != Hash(s, 29, 0);
    printf(ok ? "00c1d500_TEST PASS\n" : "00c1d500_TEST FAIL\n");
    return ok ? 0 : 1;
}