// Behaviour test for 0x00720050 CCompositeBlendEntry::CCompositeBlendEntry(key, desc): stores the key and copy-constructs the blend description
#include <cstdio>
class CRGBColour { public: unsigned char B, G, R, A; };
static int g_copies = 0;
class CCompositeBlendDesc { public:
    unsigned long TextureIndex;
    long          BlendType;
    CRGBColour    DiffuseColour;
    CRGBColour    SpecularColour;
    CCompositeBlendDesc() { }
    CCompositeBlendDesc(const CCompositeBlendDesc& other)
        : TextureIndex(other.TextureIndex), BlendType(other.BlendType), DiffuseColour(other.DiffuseColour),
          SpecularColour(other.SpecularColour) { ++g_copies; }
};
struct CCompositeBlendEntry {
    unsigned long       Key;
    CCompositeBlendDesc Desc;
    CCompositeBlendEntry(unsigned long key, const CCompositeBlendDesc& desc);
};
CCompositeBlendEntry::CCompositeBlendEntry(unsigned long key, const CCompositeBlendDesc& desc)
    : Key(key), Desc(desc)
{
}
int main()
{
    CCompositeBlendDesc d;
    d.TextureIndex = 7; d.BlendType = 3;
    d.DiffuseColour.R = 200; d.SpecularColour.A = 55;
    CCompositeBlendEntry e(0x1234, d);
    if (e.Key != 0x1234 || e.Desc.TextureIndex != 7 || e.Desc.BlendType != 3 || e.Desc.DiffuseColour.R != 200 ||
        e.Desc.SpecularColour.A != 55 || g_copies != 1) { std::printf("00720050_TEST FAIL\n"); return 1; }
    std::printf("00720050_TEST PASS\n");
    return 0;
}