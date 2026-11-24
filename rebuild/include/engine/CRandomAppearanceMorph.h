#ifndef FABLE_ENGINE_CRANDOMAPPEARANCEMORPH_H
#define FABLE_ENGINE_CRANDOMAPPEARANCEMORPH_H
// Retail opt-in (Codex pass 19): Ego_r original types and TLC 12-byte arrays.
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
#include "engine/CDefString.h"
#if defined(FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN) || defined(FABLE_RETAIL_CPERSISTTRAITS_CREATURE_FULL_TYPES_PASS22)
class CMemoryDataInputStream;
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_CREATURE_FULL_TYPES_PASS22
class CDataOutputStream;
#endif
class CRandomAppearanceMorph {
public:
#ifdef FABLE_RETAIL_CPERSISTTRAITS_CREATURE_FULL_TYPES_PASS22
 // Inline-budget: native00676740 expands automatic destruction on all three return paths; plain in-class body remains called.
 FABLE_FORCEINLINE ~CRandomAppearanceMorph() {}
 CRandomAppearanceMorph(); // Original ctor, native0086C4D0; initializes real arrays and resizes two to3.
 void TransferBinaryOut(CDataOutputStream&) const; // Original0086B6D0
#endif
 class CTextureMorph { public: unsigned long NewTexture; unsigned long GroupID; };
 typedef CVectorMap<unsigned long,CTextureMorph> TTextureMorphMap;
 class CBodyPartMesh { public:
#ifdef FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN
  // Opt-in (chunk Q7, row 0x0086BFB0 TransferBinaryIn): Ego_r.pdb ??0CBodyPartMesh@CRandomAppearanceMorph@@QAE@XZ, inline in retail
  // (the resize default temporary zeroes MeshIndex and Priority, 0x0086C086 / 0x0086C08A, before the map's three words and Dirty).
  CBodyPartMesh():MeshIndex(0),Priority(0){}
#endif
  long MeshIndex; long Priority; TTextureMorphMap TextureMorphs; };
 class CSkeletalMorph { public:
#ifdef FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN
  // Opt-in (chunk Q7, row 0x0086BFB0): Ego_r.pdb ??0CSkeletalMorph@CRandomAppearanceMorph@@QAE@XZ, inline in retail (the resize default
  // pair zeroes GroupID, sets Name to -1 and clears AllowVariation, 0x0086C29C..0x0086C2A8).
  CSkeletalMorph():GroupID(0),AllowVariation(false){}
#endif
  long GroupID; CDefString Name; bool AllowVariation; };
 class CMorphSet {
 public:
#ifdef FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TEXTUREMORPH_CTOR
  // e01 opt-in (row 0x0086BE40, same-TU body of GetRandomTextureMorphs 0x0086B810): Ego_d.pdb random_appearance_morph.obj
  // CMorphSetTextureMorph::CMorphSetTextureMorph(long original_texture, long new_texture) (inline; listed right after
  // GetRandomTextureMorphs, its first user). No layout change.
  class CMorphSetTextureMorph { public: CMorphSetTextureMorph(long original_texture,long new_texture):OriginalTexture(original_texture),NewTexture(new_texture){} long OriginalTexture; long NewTexture; };
#else
  class CMorphSetTextureMorph { public: long OriginalTexture; long NewTexture; };
#endif
  class CMorphSetSkeletalMorph { public: CCharString Name; float Strength; };
  class CMeshTextureSet { public: long MeshIndex; CArray<CMorphSetTextureMorph> TextureMorphs; ~CMeshTextureSet() {} };
  CArray<CMeshTextureSet> BodyParts;
  CArray<CMorphSetSkeletalMorph> SkeletalMorphs;
  CArray<CMorphSetTextureMorph> GeneralTextureMorphs;
 };
#if defined(FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN) || defined(FABLE_RETAIL_CPERSISTTRAITS_CREATURE_FULL_TYPES_PASS22)
 // Opt-in (chunk Q7, row 0x0086BFB0): Ego_r.pdb ?TransferBinaryIn@CRandomAppearanceMorph@@QAEXAAVCMemoryDataInputStream@@@Z.
 void TransferBinaryIn(CMemoryDataInputStream& stream);
#endif
#ifdef FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMMORPHSET
 // NF93 opt-in (round 8, q11250 / q11251 row 0x0086BF70): Ego_r.pdb (0x00857780) + FableWin.pdb
 // ?GetRandomMorphSet@CRandomAppearanceMorph@@QBEXAAKAAVCMorphSet@1@_N@Z (public const), out of line in retail at 0x0086BF70.
 // Declaration only: no layout change.
 void GetRandomMorphSet(unsigned long& seed,CMorphSet& morph_set,bool ignore_head) const;            // 0x0086BF70
#endif
protected:
 CArray<CArray<CBodyPartMesh> > BodyParts;
 CArray<TTextureMorphMap> BodyPartTextureMorphs;
 CVectorMap<long,CSkeletalMorph> SkeletalMorphs;
 TTextureMorphMap GeneralTextureMorphs;
 unsigned long MaxTextureGroupID;
 void GetRandomBodyParts(unsigned long&,CMorphSet&,bool) const;
#ifdef FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_SKELETAL
 // Opt-in (chunk Q15, row 0x0086B9D0): Ego_r.pdb ?GetRandomSkeletalMorph@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@@Z and
 // ?AddSkeletalMorphToMorphSet@CRandomAppearanceMorph@@IBEXAAKABVCSkeletalMorph@1@AAVCMorphSet@1@@Z (0x0086B900). Declarations only.
 void GetRandomSkeletalMorph(unsigned long& seed,CMorphSet& morph_set) const;
 void AddSkeletalMorphToMorphSet(unsigned long& seed,const CSkeletalMorph& morph,CMorphSet& morph_set) const;
#endif
 void GetRandomTextureMorphs(unsigned long&,unsigned long,const TTextureMorphMap&,CArray<CMorphSet::CMorphSetTextureMorph>&) const;
};
#ifdef FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMTEXTUREMORPHS_BODY
// NF98 opt-in (round 8, q11251, one definition): Ego_r.pdb / FableWin.pdb
// ?GetRandomTextureMorphs@CRandomAppearanceMorph@@IBEXAAKKABV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@AAV?$CArray@VCMorphSetTextureMorph@CMorphSet@CRandomAppearanceMorph@@@@@Z
// (retail 0x0086B810), the e01 same-TU body of row 0x0086BE40 moved here unchanged. inline __declspec(noinline): retail calls it out of line
// (0x0086BE40 twice, 0x0086BF70); row 0x0086BE40 includes this body and row 0x0086B810 emits it as a pick-any COMDAT (nf88 scaffolding), so
// no TU defines it strongly. Needs FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TEXTUREMORPH_CTOR, FABLE_RETAIL_CVECTORMAP_UPPERBOUND and
// FABLE_RETAIL_CARRAY_PUSHBACK.
#include "engine/GFRandom.h"
inline __declspec(noinline)
void CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long& seed,unsigned long texture_group_id,const TTextureMorphMap& texture_morphs,CArray<CMorphSet::CMorphSetTextureMorph>& morph_set) const
{
 TTextureMorphMap::const_iterator upper_it;
 TTextureMorphMap::const_iterator lower_it=texture_morphs.begin();
 while(lower_it!=texture_morphs.end()) {
  upper_it=texture_morphs.UpperBound(lower_it->first);
  unsigned long no_valid_textures=0;
  for(TTextureMorphMap::const_iterator it=lower_it;it!=upper_it;++it) {
   if(it->second.GroupID==0||it->second.GroupID==texture_group_id)
    ++no_valid_textures;
  }
  if(no_valid_textures) {
   unsigned long wanted_n=GFRandom(no_valid_textures,&seed);
   unsigned long current_n=0;
   for(TTextureMorphMap::const_iterator it=lower_it;it!=upper_it;++it) {
    if(it->second.GroupID==0||it->second.GroupID==texture_group_id) {
     if(current_n==wanted_n) {
      CMorphSet::CMorphSetTextureMorph entry(it->first,it->second.NewTexture);
      morph_set.push_back(entry);
      break;
     }
     ++current_n;
    }
   }
  }
  lower_it=upper_it;
 }
}
#endif
#endif
