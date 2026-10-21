// behaviour test for 0x004372C0
#include <stdio.h>
class C2DExtentsI {                                     // Ego_r.pdb: long X, Y; non-POD (returned through a hidden pointer)
public:
    long X;                                              // +0x00
    long Y;                                              // +0x04
    C2DExtentsI() {}
};
class CTexture {
public:
    C2DExtentsI GetDimensions(unsigned long level) const { C2DExtentsI e; e.X = 64 >> level; e.Y = 32 >> level; return e; }
    unsigned long GetHeight(unsigned long level) const;
};
unsigned long CTexture::GetHeight(unsigned long level) const
{
    return GetDimensions(level).Y;
}

int main()
{
    CTexture t; int ok = t.GetHeight(0) == 32 && t.GetHeight(2) == 8;
    printf(ok ? "004372c0_TEST PASS\n" : "004372c0_TEST FAIL\n");
    return ok ? 0 : 1;
}