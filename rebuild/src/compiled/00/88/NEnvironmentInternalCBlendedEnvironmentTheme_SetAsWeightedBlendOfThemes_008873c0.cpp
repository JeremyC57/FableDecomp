// 008873c0 NEnvironmentInternal::CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes
// Leaf: ?SetAsWeightedBlendOfThemes@CBlendedEnvironmentTheme@NEnvironmentInternal@@QAEXABV?$CArray@VCBlendedEnvironmentTheme@NEnvironmentInternal@@@@ABV?$CArray@M@@@Z
// Flags: /Gr
// Inline-budget: The size-specific shared vector<long>::clear uses erase. Restore the compiler defaults before the weighted-blend primary.
#define FABLE_RETAIL_STLP_INVENTORY_LONG_OWNERS_PASS27
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_WEIGHTED
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "rebuild_abi.h"
#include "fable_assert.h"
#include "engine/CBlendedEnvironmentTheme.h"
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template<> inline void std::vector<long>::clear(){erase(begin(),end());}
#ifdef _MSC_VER
#pragma optimize("",on)
#pragma inline_depth()
#endif

void NEnvironmentInternal::CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes(const CArray<CBlendedEnvironmentTheme>& themes,const CArray<float>& weights) {
    FABLE_RETAIL_ASSERT(weights.size()==themes.size());
    FABLE_RETAIL_ASSERT(!(weights.empty() || themes.empty()));
    if(weights.empty() || themes.empty()) return;
    for(long n=0;n<19;++n) {
        Colours[n].Initialise(0.0f,0.0f,0.0f,0.0f);
        for(long index=0;index<themes.size();++index)
            Colours[n]+=themes[index].Colours[n]*weights[index];
    }
    for(long n=0;n<30;++n) {
        Scalers[n]=0.0f;
        for(long index=0;index<themes.size();++index)
            Scalers[n]+=themes[index].Scalers[n]*weights[index];
    }
    for(long n=0;n<3;++n) {
        TextureSets[n].Textures.clear();
        TextureSets[n].Blends.clear();
        for(long index=0;index<themes.size();++index) {
            for(long ts=0;ts<themes[index].TextureSets[n].Textures.size();++ts) {
                TextureSets[n].Textures.push_back(themes[index].TextureSets[n].Textures[ts]);
                TextureSets[n].Blends.push_back(themes[index].TextureSets[n].Blends[ts]*weights[index]);
            }
        }
        CompactTextureSet(TextureSets[n],themes[0].TextureSets[n].Textures.size());
    }
}