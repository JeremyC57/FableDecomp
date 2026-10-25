// Behaviour test for 0x0071FDF0 _STL::swap<pair<EHeroMorphType,CTextureMorphEntry>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0071fdf0_TEST FAIL\n"); return 1; } } while (0)

static int g_blendCopies = 0;
enum EHeroMorphType { HERO_MORPH_NONE = 0, HERO_MORPH_FAT = 2, HERO_MORPH_EVIL = 5 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0, COMPOSITE_BLEND_ADD = 3 };
struct CRGBColour { unsigned char B, G, R, A; };
struct CCompositeBlendDesc {
    unsigned long       TextureIndex;
    ECompositeBlendType BlendType;
    CRGBColour          DiffuseColour;
    CRGBColour          SpecularColour;
    CCompositeBlendDesc(unsigned long t, ECompositeBlendType b, unsigned char d, unsigned char s) : TextureIndex(t), BlendType(b)
    { DiffuseColour.B = DiffuseColour.G = DiffuseColour.R = DiffuseColour.A = d; SpecularColour.B = SpecularColour.G = SpecularColour.R = SpecularColour.A = s; }
    CCompositeBlendDesc(const CCompositeBlendDesc& o) : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) { ++g_blendCopies; }
};
struct CTextureMorphEntry {
    long                TextureLayer;
    CCompositeBlendDesc Blend;
    CTextureMorphEntry(long l, const CCompositeBlendDesc& b) : TextureLayer(l), Blend(b) { }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    pair<EHeroMorphType, CTextureMorphEntry> a(HERO_MORPH_FAT, CTextureMorphEntry(1, CCompositeBlendDesc(11, COMPOSITE_BLEND_ADD, 0x10, 0x20)));
    pair<EHeroMorphType, CTextureMorphEntry> b(HERO_MORPH_EVIL, CTextureMorphEntry(2, CCompositeBlendDesc(22, COMPOSITE_BLEND_NONE, 0x30, 0x40)));
    g_blendCopies = 0;
    swap_model(a, b);
    CHECK(a.first == HERO_MORPH_EVIL && a.second.TextureLayer == 2 && a.second.Blend.TextureIndex == 22 && a.second.Blend.BlendType == COMPOSITE_BLEND_NONE);
    CHECK(a.second.Blend.DiffuseColour.R == 0x30 && a.second.Blend.SpecularColour.A == 0x40);
    CHECK(b.first == HERO_MORPH_FAT && b.second.TextureLayer == 1 && b.second.Blend.TextureIndex == 11 && b.second.Blend.BlendType == COMPOSITE_BLEND_ADD);
    CHECK(b.second.Blend.DiffuseColour.G == 0x10 && b.second.Blend.SpecularColour.B == 0x20);
    CHECK(g_blendCopies == 1);
    std::printf("0071fdf0_TEST PASS\n");
    return 0;
}