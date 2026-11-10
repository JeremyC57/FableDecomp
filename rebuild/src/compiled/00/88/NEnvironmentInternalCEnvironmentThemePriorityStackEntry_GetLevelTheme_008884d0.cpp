// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::GetLevelTheme
// Inline-budget: plain source and /d2inlS10 expand resize; use existing scoped header control to retain native resize and nontrivial range-destroy COMDAT calls.
// Leaf: ?GetLevelTheme@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QAEXAAVCBlendedEnvironmentTheme@2@MABVCEnvironmentThemeResourcePallette@2@@Z
// Retail: 0x008884d0, 514 bytes, thiscall.
// Flags: /Gr
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_WEIGHTED
#define FABLE_RETAIL_BLENDED_WEIGHTED
#define FABLE_RETAIL_CARRAY
#include "rebuild_abi.h"
#include "engine/CEnvironmentThemePriorityStackEntry.h"
void NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::GetLevelTheme(CBlendedEnvironmentTheme& result,float time_of_day,const CEnvironmentThemeResourcePallette& theme_palette) {
    if(Themes.size()==1) {
        theme_palette.GetBlendedTheme(result,Themes[0].ThemeID,time_of_day);
        return;
    }
    CArray<CBlendedEnvironmentTheme> themes;
    CArray<float> weights;
    themes.reserve(Themes.size());
    weights.reserve(Themes.size());
    for(long n=0;n<Themes.size();++n) {
        const unsigned int index=themes.size();
        themes.resize(index+1,CBlendedEnvironmentTheme());
        theme_palette.GetBlendedTheme(themes[index],Themes[n].ThemeID,time_of_day);
        weights.push_back(Themes[n].Weight);
    }
    result.SetAsWeightedBlendOfThemes(themes,weights);
}