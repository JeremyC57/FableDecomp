// behaviour test for 0x0081a200 (std::__uninitialized_fill_n over 24-byte morph entries)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
#include <new>
enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };
struct CTextureMorphEntry {                     // 20 bytes
    EHeroMorphType      MorphType;              // +0x00
    long                TextureLayer;           // +0x04
    long                ReplacingTextureIndex;  // +0x08
    long                BankIndex;              // +0x0C
    ECompositeBlendType Blend;                  // +0x10
};
struct __false_type { __false_type() {} };
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
typedef pair<EHeroMorphType, CTextureMorphEntry> CMorphEntryPair;
template CMorphEntryPair* __fastcall __uninitialized_fill_n(CMorphEntryPair*, unsigned int, const CMorphEntryPair&, const __false_type&);
}

int main()
{
    static double raw[64];
    std::CMorphEntryPair* p = (std::CMorphEntryPair*)raw;
    for (int i = 0; i < 4; ++i) { p[i].first = HERO_MORPH_TYPE_NONE; p[i].second.BankIndex = -1; }
    static double xs[16];
    std::CMorphEntryPair& x = *(std::CMorphEntryPair*)xs;
    x.first = (EHeroMorphType)2; x.second.MorphType = (EHeroMorphType)9; x.second.TextureLayer = 1; x.second.ReplacingTextureIndex = 2;
    x.second.BankIndex = 3; x.second.Blend = (ECompositeBlendType)4;
    __false_type tag;
    std::CMorphEntryPair* e0 = std::__uninitialized_fill_n(p, 0u, x, tag);
    std::CMorphEntryPair* e = std::__uninitialized_fill_n(p, 3u, x, tag);
    int ok = e0 == p && e == p + 3 && p[0].first == 2 && p[0].second.MorphType == 9 && p[2].second.Blend == 4
          && p[1].second.ReplacingTextureIndex == 2 && p[3].second.BankIndex == -1;
    if (!ok) { printf("0081a200_TEST FAIL\n"); return 1; }
    printf("0081a200_TEST PASS\n");
    return 0;
}