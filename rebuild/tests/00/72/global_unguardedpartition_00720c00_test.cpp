// Behaviour test for 0x00720C00 __unguarded_partition over pair<EHeroMorphType,CTextureMorphEntry>
// (iter_swap 0x0071FDF0 modelled as a plain swap).
#include <cstdio>
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
struct CCompositeBlendDesc { unsigned long TextureIndex; long BlendType; unsigned char Diffuse[4]; unsigned char Specular[4]; };
struct CTextureMorphEntry { long TextureLayer; CCompositeBlendDesc Blend; };
struct CMorphPair { EHeroMorphType first; CTextureMorphEntry second; };
struct CMorphLess {
    char m_tag;
    bool operator()(const CMorphPair& a, const CMorphPair& b) const { return a.first < b.first; }
};
static void iter_swap_(CMorphPair* a, CMorphPair* b) { CMorphPair t = *a; *a = *b; *b = t; }
static CMorphPair* unguarded_partition(CMorphPair* first, CMorphPair* last, CMorphPair pivot, CMorphLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap_(first, last);
        ++first;
    }
}
int main()
{
    CMorphPair a[7];
    int keys[7] = { 9, 4, 1, 6, 3, 8, 2 };
    for (int i = 0; i < 7; ++i) {
        a[i].first = (EHeroMorphType)keys[i];
        a[i].second.TextureLayer = keys[i] * 3;
        a[i].second.Blend.TextureIndex = keys[i] + 100;
    }
    CMorphLess comp;
    comp.m_tag = 0;
    CMorphPair pivot = a[1];                 // key 4
    CMorphPair* cut = unguarded_partition(a, a + 7, pivot, comp);
    long k = (long)(cut - a);
    if (k <= 0 || k >= 7) { std::printf("00720c00_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) {
        if (i < k && a[i].first > 4) { std::printf("00720c00_TEST FAIL\n"); return 1; }
        if (i >= k && a[i].first < 4) { std::printf("00720c00_TEST FAIL\n"); return 1; }
        if (a[i].second.TextureLayer != a[i].first * 3 || a[i].second.Blend.TextureIndex != (unsigned long)(a[i].first + 100)) { std::printf("00720c00_TEST FAIL\n"); return 1; }
    }
    std::printf("00720c00_TEST PASS\n");
    return 0;
}