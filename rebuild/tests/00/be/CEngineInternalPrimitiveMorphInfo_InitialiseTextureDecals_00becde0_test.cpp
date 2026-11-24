#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <utility>
struct CCompositeBlendDesc {unsigned long TextureIndex;int BlendType;unsigned int DiffuseColour,SpecularColour;bool operator==(const CCompositeBlendDesc&b)const{return TextureIndex==b.TextureIndex&&BlendType==b.BlendType&&DiffuseColour==b.DiffuseColour&&SpecularColour==b.SpecularColour;}};
struct CEngineTextureDecal {long OriginalTextureIndex,DecalID;std::vector<CCompositeBlendDesc>OverlaidBlends;};
static int inserts,sorts,clears;
template<class K,class V>struct CVectorMap {
 typedef std::pair<K,V>value_type;std::vector<value_type>Values;bool Dirty;
 void clear(){++clears;Values.clear();}
 void Insert(const K&key,const V&value){++inserts;Values.push_back(value_type(key,value));Dirty=true;}
 struct Less {bool operator()(const value_type&a,const value_type&b)const{return a.first<b.first;}};
 void Update(){if(Dirty){++sorts;std::sort(Values.begin(),Values.end(),Less());Dirty=false;}}
};
struct CEngineInternalPrimitiveMorphInfo {CVectorMap<std::pair<long,long>,CEngineTextureDecal>TextureDecals;int CompositeSurfaces,HiddenMaterialNames,HiddenMaterials;void InitialiseTextureDecals(const std::vector<CEngineTextureDecal>&texture_decals);};

void CEngineInternalPrimitiveMorphInfo::InitialiseTextureDecals(const std::vector<CEngineTextureDecal>& texture_decals)
{
    unsigned long size = texture_decals.size();
    TextureDecals.clear();
    for (long c1 = 0; c1 < size; ++c1) {
        const CEngineTextureDecal& decal = texture_decals[c1];
        std::pair<long, long> key(decal.OriginalTextureIndex, decal.DecalID);
        TextureDecals.Insert(key, decal);
    }
    TextureDecals.Update();
}
int main(){
 for(int scenario=0;scenario<5;++scenario)for(int dirty=0;dirty<2;++dirty){
  inserts=sorts=clears=0;int n=scenario==0?0:(scenario==1?1:(scenario==2?3:(scenario==3?5:64)));
  std::vector<CEngineTextureDecal>input;
  for(int i=0;i<n;++i){CEngineTextureDecal d;d.OriginalTextureIndex=scenario==3?4:((i*7)%13-6);d.DecalID=scenario==3?2:(n-i);for(int c=0;c<i%4;++c){CCompositeBlendDesc b;b.TextureIndex=i*10+c;b.BlendType=c;b.DiffuseColour=0xfedcba98u+i;b.SpecularColour=0x12345678u+c;d.OverlaidBlends.push_back(b);}input.push_back(d);}
  CEngineInternalPrimitiveMorphInfo morph;morph.CompositeSurfaces=123;morph.HiddenMaterialNames=456;morph.HiddenMaterials=789;morph.TextureDecals.Dirty=dirty!=0;
  CEngineTextureDecal old;old.OriginalTextureIndex=999;old.DecalID=999;morph.TextureDecals.Values.push_back(std::make_pair(std::make_pair(999L,999L),old));
  morph.InitialiseTextureDecals(input);
  assert(clears==1&&inserts==n&&sorts==((n||dirty)?1:0)&&!morph.TextureDecals.Dirty&&morph.TextureDecals.Values.size()==input.size());
  assert(morph.CompositeSurfaces==123&&morph.HiddenMaterialNames==456&&morph.HiddenMaterials==789);
  std::vector<bool>used(input.size(),false);
  for(unsigned int i=0;i<morph.TextureDecals.Values.size();++i){
   const CVectorMap<std::pair<long,long>,CEngineTextureDecal>::value_type&p=morph.TextureDecals.Values[i];if(i)assert(!(p.first<morph.TextureDecals.Values[i-1].first));
   assert(p.first.first==p.second.OriginalTextureIndex&&p.first.second==p.second.DecalID);bool found=false;
   for (unsigned int j = 0; j < input.size(); ++j) {
    if (!used[j] && p.first == std::make_pair(input[j].OriginalTextureIndex, input[j].DecalID)
        && p.second.OverlaidBlends == input[j].OverlaidBlends) {
     used[j] = true;
     found = true;
     break;
    }
   }
   assert(found);
  }
  if(n>1){std::vector<std::vector<CCompositeBlendDesc> >saved;for(unsigned int i=0;i<morph.TextureDecals.Values.size();++i)saved.push_back(morph.TextureDecals.Values[i].second.OverlaidBlends);for(int i=0;i<n;++i)input[i].OverlaidBlends.clear();for(unsigned int i=0;i<saved.size();++i)assert(morph.TextureDecals.Values[i].second.OverlaidBlends==saved[i]);}
 }
 puts("00becde0_TEST PASS");return 0;
}