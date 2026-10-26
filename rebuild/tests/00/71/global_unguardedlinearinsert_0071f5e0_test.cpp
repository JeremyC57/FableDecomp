// Behaviour test for 0x0071F5E0 __unguarded_linear_insert over pair<EHeroMorphType,CTextureMorphEntry>.
#include <cstdio>
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
class CRGBColour { public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour() { }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class CCompositeBlendDesc { public:
    unsigned long TextureIndex;
    ECompositeBlendType BlendType;
    CRGBColour DiffuseColour;
    CRGBColour SpecularColour;
    CCompositeBlendDesc() { }
    CCompositeBlendDesc(const CCompositeBlendDesc& o)
        : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) { }
};
class CTextureMorphEntry { public: long TextureLayer; CCompositeBlendDesc Blend; };
struct CMorphPair { EHeroMorphType first; CTextureMorphEntry second; };
struct CMorphLess {
    char m_tag;
    bool operator()(const CMorphPair& a, const CMorphPair& b) const { return a.first < b.first; }
};
static void unguarded_linear_insert(CMorphPair* last, CMorphPair val, CMorphLess comp)
{
    CMorphPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
int main()
{
    CMorphPair a[5];
    int keys[5] = { 0, 2, 6, 9, 3 };
    for (int i = 0; i < 5; ++i) {
        a[i].first = (EHeroMorphType)keys[i];
        a[i].second.TextureLayer = 10 + i;
        a[i].second.Blend.TextureIndex = 20 + i;
        a[i].second.Blend.BlendType = (ECompositeBlendType)i;
        a[i].second.Blend.DiffuseColour.R = (unsigned char)i;
        a[i].second.Blend.SpecularColour.A = (unsigned char)(i + 1);
    }
    CMorphLess comp;
    comp.m_tag = 0;
    unguarded_linear_insert(a + 4, a[4], comp);
    int want[5] = { 0, 2, 3, 6, 9 };
    long layer[5] = { 10, 11, 14, 12, 13 };
    for (int i = 0; i < 5; ++i)
        if ((int)a[i].first != want[i] || a[i].second.TextureLayer != layer[i] || a[i].second.Blend.TextureIndex != (unsigned long)(layer[i] + 10)) {
            std::printf("0071f5e0_TEST FAIL\n");
            return 1;
        }
    if (a[2].second.Blend.SpecularColour.A != 5 || a[2].second.Blend.DiffuseColour.R != 4) { std::printf("0071f5e0_TEST FAIL\n"); return 1; }
    std::printf("0071f5e0_TEST PASS\n");
    return 0;
}