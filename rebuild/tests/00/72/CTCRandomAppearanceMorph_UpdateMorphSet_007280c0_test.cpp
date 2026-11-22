#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CCOMPOSITEBLENDDESC_ORIGINAL_CTOR_PASS26
#include "engine/CCompositeBlendDesc.h"
template<class T>class CArray:public std::vector<T>{};
static int pattern,random_calls,dirty_calls,clear_calls,head_lookups,handle_count;
static bool expected_ignore;
static unsigned long expected_seed;
class CRandomAppearanceMorph {
public:
 class CMorphSet {
 public:
  class CMorphSetTextureMorph {public:long OriginalTexture,NewTexture;};
  class CMorphSetSkeletalMorph {public:std::string Name;float Strength;};
  class CMeshTextureSet {public:long MeshIndex;CArray<CMorphSetTextureMorph>TextureMorphs;};
  CArray<CMeshTextureSet>BodyParts;CArray<CMorphSetSkeletalMorph>SkeletalMorphs;CArray<CMorphSetTextureMorph>GeneralTextureMorphs;
 };
 void GetRandomMorphSet(unsigned long&seed,CMorphSet&m,bool ignore)const {
  assert(seed==expected_seed&&ignore==expected_ignore);++random_calls;seed+=9;
  for(int i=0;i<3;++i){CMorphSet::CMorphSetTextureMorph e;e.OriginalTexture=10+i;e.NewTexture=20+i;m.GeneralTextureMorphs.push_back(e);}
  if(pattern){m.BodyParts.resize(2);for(int p=0;p<2;++p){m.BodyParts[p].MeshIndex=100+p;for(int i=0;i<p+1;++i){CMorphSet::CMorphSetTextureMorph e;e.OriginalTexture=30+p*3+i;e.NewTexture=40+p*3+i;m.BodyParts[p].TextureMorphs.push_back(e);}}}
  if(pattern!=2){for(int i=0;i<2;++i){CMorphSet::CMorphSetSkeletalMorph e;e.Name=i?"jaw":"brow";e.Strength=i?0.75f:-0.25f;m.SkeletalMorphs.push_back(e);}}
 }
};
static CRandomAppearanceMorph random_morph;
class CCreatureDef {public:const CRandomAppearanceMorph&GetRandomAppearanceMorph()const{return random_morph;}};
static CCreatureDef creature_def;
template<class T>class CDefPointer {
public:T*Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer&b):Object(b.Object){if(Object)++handle_count;}~CDefPointer(){if(Object)--handle_count;}T&operator*()const{return *Object;}T*operator->()const{return Object;}
};
class CThingBaseDef {public:bool GetSubDef(CDefPointer<const CCreatureDef>*out)const{out->Object=&creature_def;++handle_count;return true;}};
static CThingBaseDef base_def;
class CTCDecapitation {public:bool decapitated;bool IsDecapitated()const{return decapitated;}};
static CTCDecapitation head;
class CThing {public:bool have_head;CDefPointer<const CThingBaseDef>GetPBaseDef()const{CDefPointer<const CThingBaseDef>p;p.Object=&base_def;++handle_count;return p;}bool PeekTC(const CTCDecapitation**out)const{++head_lookups;if(have_head)*out=&head;return have_head;}void SetAsRenderedThisFrame(unsigned long flags){assert(flags==8);++dirty_calls;}};
class CEngineTextureMorphSet {
public:
 struct Entry {long original,texture;};std::vector<Entry>entries;
 void AddTextureMorph(long original,const CCompositeBlendDesc&blend){assert(blend.BlendType==COMPOSITE_BLEND_SOLID);assert(blend.DiffuseColour.R==0&&blend.DiffuseColour.G==0&&blend.DiffuseColour.B==0&&blend.DiffuseColour.A==255);assert(blend.SpecularColour.R==0&&blend.SpecularColour.G==0&&blend.SpecularColour.B==0&&blend.SpecularColour.A==0);Entry e;e.original=original;e.texture=blend.TextureIndex;entries.push_back(e);}
};
class CTCAppearanceMorphBase {public:class CMeshMorphSet {public:class CMeshMorphSetParts {public:unsigned long MeshIndex;CEngineTextureMorphSet TextureMorphs;};CArray<CMeshMorphSetParts>MeshParts;CEngineTextureMorphSet OriginalMeshTextureMorphs;};};
class CTCSkeletalMorph {public:std::vector<std::string>names;std::vector<float>strengths;void ClearSkeletalMorphs(long id){assert(id==0x52414e44);++clear_calls;names.clear();strengths.clear();}void AddSkeletalMorph(long id,const std::string&name,float strength){assert(id==0x52414e44);names.push_back(name);strengths.push_back(strength);}};
class CTCRandomAppearanceMorph {public:CThing*ParentThing;CTCAppearanceMorphBase::CMeshMorphSet*PMeshMorphSet;unsigned long Seed;CTCSkeletalMorph*PTCSkeletalMorph;CThing&GetParentThing(){return *ParentThing;}void UpdateMorphSet();};

void CTCRandomAppearanceMorph::UpdateMorphSet()
{
 const CThingBaseDef& def=*GetParentThing().GetPBaseDef();
 bool ignore_head=false;
 const CTCDecapitation* ptc_decapitation;
 if(GetParentThing().PeekTC(&ptc_decapitation)&&ptc_decapitation->IsDecapitated())ignore_head=true;
 CRandomAppearanceMorph::CMorphSet morph_set;
 CDefPointer<const CCreatureDef> pcreature_def;
 def.GetSubDef(&pcreature_def);
 unsigned long temp_seed=Seed;
 pcreature_def->GetRandomAppearanceMorph().GetRandomMorphSet(temp_seed,morph_set,ignore_head);
 if(morph_set.BodyParts.size()<=0) {
  for(long c1=0;c1<morph_set.GeneralTextureMorphs.size();++c1) {
   CRandomAppearanceMorph::CMorphSet::CMorphSetTextureMorph entry=morph_set.GeneralTextureMorphs[c1];
   PMeshMorphSet->OriginalMeshTextureMorphs.AddTextureMorph(entry.OriginalTexture,CCompositeBlendDesc(COMPOSITE_BLEND_SOLID,entry.NewTexture,CRGBColour(0,0,0),CRGBColour(0,0,0,0)));
  }
 }else {
  PMeshMorphSet->MeshParts.resize(morph_set.BodyParts.size());
  for(long c1=0;c1<morph_set.BodyParts.size();++c1) {
   const CRandomAppearanceMorph::CMorphSet::CMeshTextureSet& body_part=morph_set.BodyParts[c1];
   PMeshMorphSet->MeshParts[c1].MeshIndex=body_part.MeshIndex;
   for(long c2=0;c2<body_part.TextureMorphs.size();++c2) {
    CRandomAppearanceMorph::CMorphSet::CMorphSetTextureMorph entry=body_part.TextureMorphs[c2];
    PMeshMorphSet->MeshParts[c1].TextureMorphs.AddTextureMorph(entry.OriginalTexture,CCompositeBlendDesc(COMPOSITE_BLEND_SOLID,entry.NewTexture,CRGBColour(0,0,0),CRGBColour(0,0,0,0)));
   }
  }
 }
 if(PTCSkeletalMorph) {
  PTCSkeletalMorph->ClearSkeletalMorphs('RAND');
  for(long c1=0;c1<morph_set.SkeletalMorphs.size();++c1)
   PTCSkeletalMorph->AddSkeletalMorph('RAND',morph_set.SkeletalMorphs[c1].Name,morph_set.SkeletalMorphs[c1].Strength);
 }
 GetParentThing().SetAsRenderedThisFrame(8);
}
int main(){
 for(pattern=0;pattern<3;++pattern)for(int head_state=0;head_state<3;++head_state)for(int skeletal=0;skeletal<2;++skeletal)for(int seed_case=0;seed_case<2;++seed_case){
  random_calls=dirty_calls=clear_calls=head_lookups=handle_count=0;expected_ignore=head_state==2;expected_seed=seed_case?12345:0;
  CThing thing;thing.have_head=head_state!=0;head.decapitated=head_state==2;
  CTCSkeletalMorph skel;skel.names.push_back("old");skel.strengths.push_back(7);
  CTCAppearanceMorphBase::CMeshMorphSet mesh;mesh.MeshParts.resize(3);for(int p=0;p<3;++p)mesh.MeshParts[p].MeshIndex=777;
  CTCRandomAppearanceMorph component;component.ParentThing=&thing;component.PMeshMorphSet=&mesh;component.Seed=expected_seed;component.PTCSkeletalMorph=skeletal?&skel:0;
  component.UpdateMorphSet();
  assert(random_calls==1&&dirty_calls==1&&head_lookups==1&&handle_count==0&&component.Seed==expected_seed);
  if(!pattern){assert(mesh.MeshParts.size()==3&&mesh.MeshParts[0].MeshIndex==777);assert(mesh.OriginalMeshTextureMorphs.entries.size()==3);for(int i=0;i<3;++i){assert(mesh.OriginalMeshTextureMorphs.entries[i].original==10+i);assert(mesh.OriginalMeshTextureMorphs.entries[i].texture==20+i);}}
  else {assert(mesh.OriginalMeshTextureMorphs.entries.empty()&&mesh.MeshParts.size()==2);for(int p=0;p<2;++p){assert(mesh.MeshParts[p].MeshIndex==100+p&&mesh.MeshParts[p].TextureMorphs.entries.size()==(unsigned)p+1);for(int i=0;i<p+1;++i){assert(mesh.MeshParts[p].TextureMorphs.entries[i].original==30+p*3+i);assert(mesh.MeshParts[p].TextureMorphs.entries[i].texture==40+p*3+i);}}}
  assert(clear_calls==skeletal);
  if(skeletal){assert(skel.names.size()==(pattern==2?0u:2u));if(pattern!=2){assert(skel.names[0]=="brow"&&skel.names[1]=="jaw"&&skel.strengths[0]==-0.25f&&skel.strengths[1]==0.75f);}}
  else assert(skel.names.size()==1&&skel.names[0]=="old");
 }
 puts("007280c0_TEST PASS");return 0;
}