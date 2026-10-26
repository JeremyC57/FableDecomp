#include <cstdio>
#include <stddef.h>
class CRGBColour { public: unsigned char B, G, R, A; void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } CRGBColour() { } CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); } };
class CCompositeBlendDesc { public: unsigned long TextureIndex; long BlendType; CRGBColour DiffuseColour; CRGBColour SpecularColour; CCompositeBlendDesc() { }
    CCompositeBlendDesc(const CCompositeBlendDesc& o) : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) { } };
int main()
{
    CCompositeBlendDesc a; a.TextureIndex = 5; a.BlendType = 2; a.DiffuseColour.Set(1, 2, 3, 4); a.SpecularColour.Set(9, 8, 7, 6);
    CCompositeBlendDesc b(a);
    if (b.TextureIndex != 5 || b.BlendType != 2 || b.DiffuseColour.R != 1 || b.DiffuseColour.G != 2 || b.DiffuseColour.B != 3 || b.DiffuseColour.A != 4
        || b.SpecularColour.R != 9 || b.SpecularColour.G != 8 || b.SpecularColour.B != 7 || b.SpecularColour.A != 6 || sizeof(b) != 16 || offsetof(CCompositeBlendDesc, SpecularColour) != 0xc || offsetof(CRGBColour, R) != 2) { std::printf("005ba48a_TEST FAIL\n"); return 1; }
    std::printf("005ba48a_TEST PASS\n"); return 0;
}