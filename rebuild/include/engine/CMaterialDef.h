#ifndef FABLE_ENGINE_CMATERIALDEF_H
#define FABLE_ENGINE_CMATERIALDEF_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CBlendedParticleEffectSet;
class CMaterialDef:public CParentDefClassBase {public:
 static CCharString FABLE_FASTCALL StatGetTypeName();
 long FirstHitSound,LastHitSound;
 float OnHitRumbleLevel,OnHitRumbleFalloff;
 bool DeflectAllHits,IsDiggable;
 std::vector<long> SlideFootParticleEffect,SwordHitParticleEffect,KnockdownHitFloorParticleEffect,WindBlowDustEffect,BerserkedSwordHitParticleEffect;
 std::map<long,CBlendedParticleEffectSet> BlendedSwordHitEffects;
};
// Layout: original parent base, native OnHit reads +44/+48; six STL containers contract by4bytes.
FABLE_STATIC_ASSERT(offsetof(CMaterialDef,OnHitRumbleLevel)==0x44);
FABLE_STATIC_ASSERT(sizeof(CMaterialDef)==0x98);
#endif
