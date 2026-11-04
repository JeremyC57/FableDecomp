// _global::JenkinsHashBuffer(const char* data, unsigned long length, unsigned long seed) -- Bob Jenkins' lookup2 hash (public domain, 1996) of a byte buffer: 12-byte blocks mixed into a/b/c, the 0..11-byte tail added little-endian, final mix, c returned
// Leaf: ?JenkinsHashBuffer@@YIKPBDKK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1D500, 532 bytes, __fastcall (data ecx, length edx, seed on the stack, `ret 4`).
// Identity: Bob Jenkins' lookup2 hash() (public domain, golden ratio 0x9E3779B9, the 13/8/13/12/16/5/3/10/15 mix); landed callers
//   CString::GetHash 0x00C0DBB0 and CVtable_012b2f98::Hash 0x00C19970 bind ?JenkinsHashBuffer@@YIKPBDKK@Z.
// Shape: the reference source verbatim (VC7.1 strength-reduces the 12-byte loop into a trip count).
typedef unsigned long  ub4;
typedef unsigned char  ub1;

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

unsigned long __fastcall JenkinsHashBuffer(const char* data, unsigned long length, unsigned long seed)
{
    register ub4 a, b, c, len;
    register const ub1* k = (const ub1*)data;
    len = length;
    a = b = 0x9e3779b9;
    c = seed;
    while (len >= 12) {
        a += (k[0] + ((ub4)k[1] << 8) + ((ub4)k[2] << 16) + ((ub4)k[3] << 24));
        b += (k[4] + ((ub4)k[5] << 8) + ((ub4)k[6] << 16) + ((ub4)k[7] << 24));
        c += (k[8] + ((ub4)k[9] << 8) + ((ub4)k[10] << 16) + ((ub4)k[11] << 24));
        mix(a, b, c);
        k += 12; len -= 12;
    }
    c += length;
    switch (len) {
    case 11: c += ((ub4)k[10] << 24);
    case 10: c += ((ub4)k[9] << 16);
    case 9 : c += ((ub4)k[8] << 8);
    case 8 : b += ((ub4)k[7] << 24);
    case 7 : b += ((ub4)k[6] << 16);
    case 6 : b += ((ub4)k[5] << 8);
    case 5 : b += k[4];
    case 4 : a += ((ub4)k[3] << 24);
    case 3 : a += ((ub4)k[2] << 16);
    case 2 : a += ((ub4)k[1] << 8);
    case 1 : a += k[0];
    }
    mix(a, b, c);
    return c;
}