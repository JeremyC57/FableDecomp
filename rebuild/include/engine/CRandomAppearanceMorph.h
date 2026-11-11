#ifndef FABLE_ENGINE_CRANDOMAPPEARANCEMORPH_H
#define FABLE_ENGINE_CRANDOMAPPEARANCEMORPH_H
// Retail opt-in (Codex pass 19): Ego_r original types and TLC 12-byte arrays.
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
#include "engine/CDefString.h"
#ifdef FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN
class CMemoryDataInputStream;
#endif
class CRandomAppearanceMorph {
public:
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
  class CMorphSetTextureMorph { public: long OriginalTexture; long NewTexture; };
  class CMorphSetSkeletalMorph { public: CCharString Name; float Strength; };
  class CMeshTextureSet { public: long MeshIndex; CArray<CMorphSetTextureMorph> TextureMorphs; ~CMeshTextureSet() {} };
  CArray<CMeshTextureSet> BodyParts;
  CArray<CMorphSetSkeletalMorph> SkeletalMorphs;
  CArray<CMorphSetTextureMorph> GeneralTextureMorphs;
 };
#ifdef FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN
 // Opt-in (chunk Q7, row 0x0086BFB0): Ego_r.pdb ?TransferBinaryIn@CRandomAppearanceMorph@@QAEXAAVCMemoryDataInputStream@@@Z.
 void TransferBinaryIn(CMemoryDataInputStream& stream);
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
#endif
