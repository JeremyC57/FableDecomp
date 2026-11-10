#ifndef FABLE_CBLENDEDENVIRONMENTTHEME_H
#define FABLE_CBLENDEDENVIRONMENTTHEME_H
#include "engine/CRGBFloatColour.h"
#include "engine/CIDrawEnvironmentTextureSet.h"
class CEnvironmentDef;
class CEnvironmentThemeDaySetDef;
class CEnvironmentThemeDef;
namespace NEnvironmentInternal {
class CEnvironmentLookupTexture;
class CBlendedEnvironmentTheme {
public:
    CBlendedEnvironmentTheme();
    ~CBlendedEnvironmentTheme();
    void BuildFromSource(const CEnvironmentDef&,const CEnvironmentLookupTexture&,const CEnvironmentThemeDaySetDef&,const CEnvironmentThemeDef&,long);
    void BlendTextureSets(const CIDrawEnvironmentTextureSet&,const CIDrawEnvironmentTextureSet&,CIDrawEnvironmentTextureSet&,float) const;
    void Blend(CBlendedEnvironmentTheme&,const CBlendedEnvironmentTheme&,float) const;
    void CompactTextureSet(CIDrawEnvironmentTextureSet&,long) const;
    void SetAsWeightedBlendOfThemes(const CArray<CBlendedEnvironmentTheme>&,const CArray<float>&);
    float TimeOfDay;
    CRGBFloatColour Colours[19];
    float Scalers[30];
    CIDrawEnvironmentTextureSet TextureSets[3];
};
}
FABLE_STATIC_ASSERT(sizeof(NEnvironmentInternal::CBlendedEnvironmentTheme)==500);
#ifdef FABLE_RETAIL_BLENDED_WEIGHTED
void NEnvironmentInternal::CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes(const CArray<CBlendedEnvironmentTheme>& themes,const CArray<float>& weights) {
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
#endif

#endif
