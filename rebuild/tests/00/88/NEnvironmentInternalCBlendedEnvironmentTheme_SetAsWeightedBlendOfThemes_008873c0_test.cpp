// Leaf: ?SetAsWeightedBlendOfThemes@CBlendedEnvironmentTheme@NEnvironmentInternal@@QAEXABV?$CArray@VCBlendedEnvironmentTheme@NEnvironmentInternal@@@@ABV?$CArray@M@@@Z
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_WEIGHTED
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "rebuild_abi.h"
#include "fable_assert.h"
#include "engine/CBlendedEnvironmentTheme.h"
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

#include <stdio.h>
#include <stdlib.h>
static void ck(bool ok){if(!ok)exit(1);}
using NEnvironmentInternal::CBlendedEnvironmentTheme;
CBlendedEnvironmentTheme::CBlendedEnvironmentTheme():TimeOfDay(-47.0f) {}
CBlendedEnvironmentTheme::~CBlendedEnvironmentTheme() {}
static unsigned int count,seed,compactions;
static unsigned int num(unsigned int i,unsigned int t){return (i*3+t+seed)%5;}
static float weight(unsigned int i){return ((int)((i+seed)%5)-2)*0.25f;}
static long id(unsigned int i,unsigned int t,unsigned int j){return 1000*i+32*t+j;}
static float component(unsigned int i,unsigned int n,int k){return ((int)i*11+(int)n*2-k)*0.5f;}
void CBlendedEnvironmentTheme::CompactTextureSet(CIDrawEnvironmentTextureSet& set,long keep) const{
 unsigned int t=compactions++;ck(t<3 && &set==&TextureSets[t] && keep==num(0,t));
 unsigned int n=0;
 for(unsigned int i=0;i<count;++i)for(unsigned int j=0;j<num(i,t);++j,++n){
  ck(n<set.Textures.size() && n<set.Blends.size());
  ck(set.Textures[n]==id(i,t,j) && set.Blends[n]==component(i,j,t)*weight(i));
 }
 ck(n==set.Textures.size() && n==set.Blends.size());
}
int main(){
 for(count=0;count<8;++count)for(seed=0;seed<15;++seed){
  CArray<CBlendedEnvironmentTheme> themes;
  CArray<float> weights;
  for(unsigned int i=0;i<count;++i){
   CBlendedEnvironmentTheme f;
   for(int n=0;n<19;++n)f.Colours[n].Initialise(component(i,n,0),component(i,n,1),component(i,n,2),component(i,n,3));
   for(int n=0;n<30;++n)f.Scalers[n]=component(i,n,4);
   for(unsigned int t=0;t<3;++t)for(unsigned int j=0;j<num(i,t);++j){
    f.TextureSets[t].Textures.push_back(id(i,t,j));f.TextureSets[t].Blends.push_back(component(i,j,t));
   }
   themes.push_back(f);weights.push_back(weight(i));
  }
  CBlendedEnvironmentTheme target;
  for(int n=0;n<19;++n)target.Colours[n].Initialise(50,51,52,53);
  for(int n=0;n<30;++n)target.Scalers[n]=54;
  for(int t=0;t<3;++t){target.TextureSets[t].Textures.push_back(-1);target.TextureSets[t].Blends.push_back(-1);}
  compactions=0;target.SetAsWeightedBlendOfThemes(themes,weights);
  ck(compactions==(count?3u:0u) && target.TimeOfDay==-47.0f);
  for(int n=0;n<30;++n){
   float expected[5]={0,0,0,0,0};
   for(unsigned int i=0;i<count;++i)for(int k=0;k<5;++k)expected[k]+=component(i,n,k)*weight(i);
   if(!count)for(int k=0;k<5;++k)expected[k]=50.0f+k;
   if(n<19)ck(target.Colours[n].R==expected[0] && target.Colours[n].G==expected[1]
       && target.Colours[n].B==expected[2] && target.Colours[n].A==expected[3]);
   ck(target.Scalers[n]==expected[4]);
  }
  if(!count)for(int t=0;t<3;++t)ck(target.TextureSets[t].Textures.size()==1 && target.TextureSets[t].Textures[0]==-1);
 }
 puts("008873c0_TEST PASS");return 0;
}
bool __fastcall GFAssertFailed(const char*,const char*,const char*,unsigned long){return false;}