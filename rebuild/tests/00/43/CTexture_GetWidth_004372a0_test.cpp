// behaviour test for 0x004372A0
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
    unsigned long GetWidth(unsigned long level) const;
};
unsigned long CTexture::GetWidth(unsigned long level) const
{
    return GetDimensions(level).X;
}

int main()
{
    CTexture t; int ok = t.GetWidth(0) == 64 && t.GetWidth(2) == 16;
    printf(ok ? "004372a0_TEST PASS\n" : "004372a0_TEST FAIL\n");
    return ok ? 0 : 1;
}