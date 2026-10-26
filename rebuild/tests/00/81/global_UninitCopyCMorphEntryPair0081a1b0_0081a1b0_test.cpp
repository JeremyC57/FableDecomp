// Behaviour test for 0x0081A1B0 UninitCopy_CMorphEntryPair_0081a1b0: placement copy of pair<EHeroMorphType, CTextureMorphEntry>.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
enum EHeroMorphType { HERO_MORPH_TYPE_NONE, HERO_MORPH_TYPE_A, HERO_MORPH_TYPE_B };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE, COMPOSITE_BLEND_ADD };
class CTextureMorphEntry { public:
    EHeroMorphType      MorphType;
    long                TextureLayer;
    long                ReplacingTextureIndex;
    long                BankIndex;
    ECompositeBlendType Blend;
};
struct CMorphEntryPair {
    EHeroMorphType     first;
    CTextureMorphEntry second;
    CMorphEntryPair() : first(HERO_MORPH_TYPE_NONE) { second.MorphType = HERO_MORPH_TYPE_NONE; second.TextureLayer = 0; second.ReplacingTextureIndex = 0; second.BankIndex = 0; second.Blend = COMPOSITE_BLEND_NONE; }
    CMorphEntryPair(const CMorphEntryPair& o) : first(o.first), second(o.second) { }
};

static CMorphEntryPair* UninitCopy(const CMorphEntryPair* first, const CMorphEntryPair* last, CMorphEntryPair* result, const __false_type&)
{
    CMorphEntryPair* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CMorphEntryPair(*first);
    return cur;
}

int main()
{
    CMorphEntryPair src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].first = (EHeroMorphType)(i % 3);
        src[i].second.MorphType = HERO_MORPH_TYPE_B;
        src[i].second.TextureLayer = i;
        src[i].second.ReplacingTextureIndex = 10 + i;
        src[i].second.BankIndex = 20 + i;
        src[i].second.Blend = (i & 1) ? COMPOSITE_BLEND_ADD : COMPOSITE_BLEND_NONE;
    }
    CMorphEntryPair dst[3];
    __false_type tag;
    CMorphEntryPair* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].first != (EHeroMorphType)(k % 3) || dst[k].second.MorphType != HERO_MORPH_TYPE_B || dst[k].second.TextureLayer != k
            || dst[k].second.ReplacingTextureIndex != 10 + k || dst[k].second.BankIndex != 20 + k
            || dst[k].second.Blend != ((k & 1) ? COMPOSITE_BLEND_ADD : COMPOSITE_BLEND_NONE)) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("0081a1b0_TEST FAIL\n"); return 1; }
    std::printf("0081a1b0_TEST PASS\n");
    return 0;
}