// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::BlendInLevel(CBlendedEnvironmentTheme& result, const CBlendedEnvironmentTheme& base, float time_of_day, const CEnvironmentThemeResourcePallette& theme_palette) const -- a base weight near 1 copies the base theme; near 0 with one level theme takes that theme's blend from the palette; otherwise blends the base (when it has weight) and every level theme by their weights
// Inline-budget: the sibling GetLevelTheme 0x008884D0's scoped header controls (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE, FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE) keep vector::resize 0x0088A520 and the range destroy 0x006B67A0 out of line, as retail calls them.
// Leaf: ?BlendInLevel@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QBEXAAVCBlendedEnvironmentTheme@2@ABV32@MABVCEnvironmentThemeResourcePallette@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008886E0, 727 bytes (+ int3 padding), thiscall `ret 10h`, no EH frame.
// Callees: the implicit CBlendedEnvironmentTheme::operator= 0x006B5DF0, CEnvironmentThemeSetDay::GetBlendedTheme 0x00888260 (through
//   the palette's inline GetBlendedTheme), CBlendedEnvironmentTheme ctor 0x008864A0 / dtor 0x006B5D80, SetAsWeightedBlendOfThemes
//   0x008873C0; STLport vector<CBlendedEnvironmentTheme> reserve 0x00889CB0 / push_back 0x0088A350 / resize 0x0088A520 / range
//   destroy 0x006B67A0, vector<float> reserve 0x00724E30 / _M_insert_overflow 0x004408A0, free 0x00BFEA14.
// Layout: engine/CEnvironmentThemePriorityStackEntry.h (BaseWeight +0, Themes +8: CArray<CThemeWeight> of 12-byte records).
// Shape: the landed sibling GetLevelTheme 0x008884D0 (same TU, same header opt-ins and `/Gr` flag), with the base theme pushed
//   first when the base weight is above the lower threshold; `result = base` is the out-of-line implicit operator= (new opt-in
//   FABLE_RETAIL_CBLENDEDENVIRONMENTTHEME_ASSIGN). The port alone fixed the prior draft's esi/ecx swap in the array teardown.
//   Thresholds: 0x01278090 = 0.9990234375f, 0x01278080 = 0.0009765625f (read from Fable.exe).
// Flags: /Gr
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_WEIGHTED
#define FABLE_RETAIL_BLENDED_WEIGHTED
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBLENDEDENVIRONMENTTHEME_ASSIGN
#include "rebuild_abi.h"
#include "engine/CEnvironmentThemePriorityStackEntry.h"
void NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::BlendInLevel(CBlendedEnvironmentTheme& result,const CBlendedEnvironmentTheme& base,float time_of_day,const CEnvironmentThemeResourcePallette& theme_palette) const {
    if(BaseWeight>0.9990234375f) {
        result=base;
        return;
    }
    if(BaseWeight<0.0009765625f && Themes.size()==1) {
        theme_palette.GetBlendedTheme(result,Themes[0].ThemeID,time_of_day);
        return;
    }
    CArray<CBlendedEnvironmentTheme> themes;
    CArray<float> weights;
    if(BaseWeight>0.0009765625f) {
        themes.reserve(Themes.size()+1);
        weights.reserve(Themes.size()+1);
        themes.push_back(base);
        weights.push_back(BaseWeight);
    } else {
        themes.reserve(Themes.size());
        weights.reserve(Themes.size());
    }
    for(long n=0;n<Themes.size();++n) {
        const unsigned int index=themes.size();
        themes.resize(index+1,CBlendedEnvironmentTheme());
        theme_palette.GetBlendedTheme(themes[index],Themes[n].ThemeID,time_of_day);
        weights.push_back(Themes[n].Weight);
    }
    result.SetAsWeightedBlendOfThemes(themes,weights);
}