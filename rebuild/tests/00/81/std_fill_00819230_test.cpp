// behaviour test for 0x00819230 (std::fill over pair<EHeroMorphType, CTextureMorphEntry>)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
enum EHeroMorphType { HERO_MORPH_TYPE_NONE };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };
class CTextureMorphEntry { public:                     // 20 bytes
    EHeroMorphType      MorphType;              // +0x00
    long                TextureLayer;           // +0x04
    long                ReplacingTextureIndex;  // +0x08
    long                BankIndex;              // +0x0C
    ECompositeBlendType Blend;                  // +0x10
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
typedef pair<EHeroMorphType, CTextureMorphEntry> CMorphEntryPair;
template void __fastcall fill(CMorphEntryPair*, CMorphEntryPair*, const CMorphEntryPair&);
}

int main()
{
    std::CMorphEntryPair v[3];
    for (int i = 0; i < 3; ++i) { v[i].first = HERO_MORPH_TYPE_NONE; v[i].second.TextureLayer = -1; v[i].second.BankIndex = -1; }
    std::CMorphEntryPair val; val.first = (EHeroMorphType)3; val.second.MorphType = (EHeroMorphType)4; val.second.TextureLayer = 5;
    val.second.ReplacingTextureIndex = 6; val.second.BankIndex = 7; val.second.Blend = (ECompositeBlendType)8;
    std::fill(v, v + 2, val);
    int ok = v[0].first == 3 && v[0].second.Blend == 8 && v[1].second.TextureLayer == 5 && v[1].second.BankIndex == 7
          && v[2].second.TextureLayer == -1;
    if (!ok) { printf("00819230_TEST FAIL\n"); return 1; }
    printf("00819230_TEST PASS\n");
    return 0;
}