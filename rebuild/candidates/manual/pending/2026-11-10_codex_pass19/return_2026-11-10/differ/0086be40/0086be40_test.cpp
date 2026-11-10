#include <cstdio>
#include <vector>
#include <cstdlib>
static unsigned long gCalls=0;
static unsigned long GFRandom(unsigned long range,unsigned long* seed) { ++gCalls; *seed=(*seed+1)&0xffffffffUL;return range?*seed%range:0; }
template<class T> class CArray: public std::vector<T> {};
template<class K,class V> class CVectorMap { public: int Marker;CVectorMap():Marker(0){} };
class CRandomAppearanceMorph {
public:
 class CTextureMorph {};
 class CBodyPartMesh {public:long MeshIndex;CVectorMap<unsigned long,CTextureMorph> TextureMorphs;};
 class CMorphSet {public:
  class CMorphSetTextureMorph {public:long OriginalTexture;long NewTexture;};
  class CMeshTextureSet {public:long MeshIndex;CArray<CMorphSetTextureMorph> TextureMorphs;};
  CArray<CMeshTextureSet> BodyParts;
 };
 CArray<CArray<CBodyPartMesh> > BodyParts;
 CArray<CVectorMap<unsigned long,CTextureMorph> > BodyPartTextureMorphs;
 unsigned long MaxTextureGroupID;
 mutable unsigned long LastVariant,TextureCalls;
 CRandomAppearanceMorph():MaxTextureGroupID(0),LastVariant(0),TextureCalls(0){BodyParts.resize(3);BodyPartTextureMorphs.resize(3);}
 void GetRandomBodyParts(unsigned long&,CMorphSet&,bool) const;
 void GetRandomTextureMorphs(unsigned long&,unsigned long variant,const CVectorMap<unsigned long,CTextureMorph>& map,CArray<CMorphSet::CMorphSetTextureMorph>& out) const {
  ++TextureCalls;LastVariant=variant;CMorphSet::CMorphSetTextureMorph e;e.OriginalTexture=map.Marker;e.NewTexture=variant;out.push_back(e);
 }
};
void CRandomAppearanceMorph::GetRandomBodyParts(unsigned long& seed,CMorphSet& morph_set,bool ignore_head) const
{
 unsigned long texture_group_id=0;
 if(MaxTextureGroupID)
  texture_group_id=GFRandom(MaxTextureGroupID,&seed)+1;
 for(long cbody_part_type=0;cbody_part_type!=3;++cbody_part_type) {
  if(!BodyParts[cbody_part_type].empty()) {
   CMorphSet::CMeshTextureSet mesh_texture_set;
   long body_part_index=GFRandom(BodyParts[cbody_part_type].size(),&seed);
   const CBodyPartMesh& body_part=BodyParts[cbody_part_type][body_part_index];
   mesh_texture_set.MeshIndex=body_part.MeshIndex;
   GetRandomTextureMorphs(seed,texture_group_id,body_part.TextureMorphs,mesh_texture_set.TextureMorphs);
   GetRandomTextureMorphs(seed,texture_group_id,BodyPartTextureMorphs[cbody_part_type],mesh_texture_set.TextureMorphs);
   if(!(ignore_head && cbody_part_type==0))
    morph_set.BodyParts.push_back(mesh_texture_set);
  }
 }
}

static void require(bool x){if(!x)std::exit(1);}
int main(){
 const unsigned long seeds[]={0,7,31,0xfffffffeUL,0xffffffffUL};unsigned int cases=0;
 for(unsigned int mask=0;mask<8;++mask)for(unsigned int skip=0;skip<2;++skip)for(unsigned int group=0;group<2;++group)
 for(unsigned int si=0;si<5;++si)for(unsigned int many=0;many<2;++many)for(unsigned int existing=0;existing<2;++existing){
  CRandomAppearanceMorph m;m.MaxTextureGroupID=group?4:0;unsigned int count=0,choices=many?3:1;
  for(unsigned int i=0;i<3;++i){m.BodyPartTextureMorphs[i].Marker=100+i;if(mask&(1u<<i)){++count;
   for(unsigned int j=0;j<choices;++j){CRandomAppearanceMorph::CBodyPartMesh b;b.MeshIndex=1000+10*i+j;b.TextureMorphs.Marker=200+10*i+j;m.BodyParts[i].push_back(b);}
  }}
  unsigned long seed=seeds[si];gCalls=0;CRandomAppearanceMorph::CMorphSet out;
  if(existing){CRandomAppearanceMorph::CMorphSet::CMeshTextureSet b;b.MeshIndex=-123;out.BodyParts.push_back(b);}
  m.GetRandomBodyParts(seed,out,skip!=0);
  require(gCalls==count+group&&m.TextureCalls==count*2&&seed==((seeds[si]+count+group)&0xffffffffUL));
  unsigned int expected=existing+count-((skip&&(mask&1))?1:0);require(out.BodyParts.size()==expected);
  if(existing)require(out.BodyParts[0].MeshIndex==-123&&out.BodyParts[0].TextureMorphs.empty());
  unsigned long variant=group?(((seeds[si]+1)&0xffffffffUL)%4+1):0;unsigned int k=existing,choice_call=group;
  for(unsigned int i=0;i<3;++i)if(mask&(1u<<i)){
   ++choice_call;unsigned int chosen=(unsigned int)(((seeds[si]+choice_call)&0xffffffffUL)%choices);
   if(skip&&i==0)continue;
   require(out.BodyParts[k].MeshIndex==1000+10*(long)i+chosen&&out.BodyParts[k].TextureMorphs.size()==2);
   require(out.BodyParts[k].TextureMorphs[0].OriginalTexture==200+10*(long)i+chosen&&out.BodyParts[k].TextureMorphs[1].OriginalTexture==100+(long)i);
   require(out.BodyParts[k].TextureMorphs[0].NewTexture==(long)variant&&out.BodyParts[k].TextureMorphs[1].NewTexture==(long)variant);++k;
  }
  ++cases;
 }
 require(cases==640);std::printf("0086be40_TEST PASS\n");return 0;
}
