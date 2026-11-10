#ifndef FABLE_ENGINE_CRANDOMAPPEARANCEMORPH_H
#define FABLE_ENGINE_CRANDOMAPPEARANCEMORPH_H
// Retail opt-in (Codex pass 19): Ego_r original types and TLC 12-byte arrays.
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
#include "engine/CDefString.h"
class CRandomAppearanceMorph {
public:
 class CTextureMorph { public: unsigned long NewTexture; unsigned long GroupID; };
 typedef CVectorMap<unsigned long,CTextureMorph> TTextureMorphMap;
 class CBodyPartMesh { public: long MeshIndex; long Priority; TTextureMorphMap TextureMorphs; };
 class CSkeletalMorph { public: long GroupID; CDefString Name; bool AllowVariation; };
 class CMorphSet {
 public:
  class CMorphSetTextureMorph { public: long OriginalTexture; long NewTexture; };
  class CMorphSetSkeletalMorph { public: CCharString Name; float Strength; };
  class CMeshTextureSet { public: long MeshIndex; CArray<CMorphSetTextureMorph> TextureMorphs; ~CMeshTextureSet() {} };
  CArray<CMeshTextureSet> BodyParts;
  CArray<CMorphSetSkeletalMorph> SkeletalMorphs;
  CArray<CMorphSetTextureMorph> GeneralTextureMorphs;
 };
protected:
 CArray<CArray<CBodyPartMesh> > BodyParts;
 CArray<TTextureMorphMap> BodyPartTextureMorphs;
 CVectorMap<long,CSkeletalMorph> SkeletalMorphs;
 TTextureMorphMap GeneralTextureMorphs;
 unsigned long MaxTextureGroupID;
 void GetRandomBodyParts(unsigned long&,CMorphSet&,bool) const;
 void GetRandomTextureMorphs(unsigned long&,unsigned long,const TTextureMorphMap&,CArray<CMorphSet::CMorphSetTextureMorph>&) const;
};
#endif
