// Behaviour test for CCompositeBlendDesc::CCompositeBlendDesc(ECompositeBlendType, unsigned long, const CRGBColour&, const CRGBColour&) @ 0x006CABA0
#include <cstdio>

typedef long ECompositeBlendType;

struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CCompositeBlendDesc {
    unsigned long       TextureIndex;
    ECompositeBlendType BlendType;
    CRGBColour          DiffuseColour;
    CRGBColour          SpecularColour;
    CCompositeBlendDesc(ECompositeBlendType blendType, unsigned long textureIndex, const CRGBColour& diffuse, const CRGBColour& specular)
        : TextureIndex(textureIndex), BlendType(blendType)
    {
        DiffuseColour.Set(diffuse.R, diffuse.G, diffuse.B, diffuse.A);
        SpecularColour.Set(specular.R, specular.G, specular.B, specular.A);
    }
};

int main()
{
    CRGBColour d; d.B = 1; d.G = 2; d.R = 3; d.A = 4;
    CRGBColour s; s.B = 0x10; s.G = 0x20; s.R = 0x30; s.A = 0xff;
    CCompositeBlendDesc desc(7, 42, d, s);
    if (desc.TextureIndex != 42 || desc.BlendType != 7) { std::printf("006caba0_TEST FAIL\n"); return 1; }
    if (desc.DiffuseColour.B != 1 || desc.DiffuseColour.G != 2 || desc.DiffuseColour.R != 3 || desc.DiffuseColour.A != 4) { std::printf("006caba0_TEST FAIL\n"); return 1; }
    if (desc.SpecularColour.B != 0x10 || desc.SpecularColour.G != 0x20 || desc.SpecularColour.R != 0x30 || desc.SpecularColour.A != 0xff) { std::printf("006caba0_TEST FAIL\n"); return 1; }
    std::printf("006caba0_TEST PASS\n");
    return 0;
}
