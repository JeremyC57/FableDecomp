// CTCRandomAppearanceMorph::UpdateMorphSet -- rebuild texture and skeletal morph sets.
// Leaf: ?UpdateMorphSet@CTCRandomAppearanceMorph@@AAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE_MESHMORPHSET
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET
#define FABLE_RETAIL_CENGINETEXTUREMORPHSET_ADDTEXTUREMORPH_PASS26
#define FABLE_RETAIL_CCOMPOSITEBLENDDESC_ORIGINAL_CTOR_PASS26
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_CCREATUREDEF_RESPAWN_PASS24
#define FABLE_RETAIL_CCREATUREDEF_RANDOMMORPH_PASS26
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMMORPHSET
#define FABLE_RETAIL_CTCSKELETALMORPH_RANDOM_PASS26
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CRandomAppearanceMorph.h"
#include "engine/CCreatureDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThing.h"
#include "engine/CTCDecapitation.h"
#include "engine/CTCRandomAppearanceMorph.h"
#include "engine/CTCSkeletalMorph.h"
#include "engine/CDefClassBase_GetSubDef.h"
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