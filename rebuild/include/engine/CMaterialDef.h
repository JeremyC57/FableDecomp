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
#ifdef FABLE_RETAIL_CMATERIALDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CMaterialDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
#ifdef FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS
// AUTH24 opt-in (2026-11-22, row 0x00839420 NThingHelpers::CreateEffectOnHit): Ego_r.pdb + FableWin.pdb
// ?GetBlendedSwordHitEffect@CMaterialDef@@QBE_NJMAAJ0AAM@Z (retail 0x008EE790, landed), ?GetRandomBerserkedSwordHitParticleEffect@CMaterialDef@@QBEJXZ
// (0x008EE720, landed) and ?GetRandomSwordHitParticleEffect@CMaterialDef@@QBEJXZ (0x008EE600, landed). Declarations only; no layout change.
public: bool GetBlendedSwordHitEffect(long region_def_index, float delta_health, long& from_effect_id, long& to_effect_id, float& interpolate) const;
public: long GetRandomBerserkedSwordHitParticleEffect() const;
public: long GetRandomSwordHitParticleEffect() const;
#endif
#ifdef FABLE_RETAIL_CMATERIALDEF_GETRANDOMSLIDEFOOTPARTICLEEFFECT
// NF49 opt-in (2026-11-17, row 0x008F7950 NCombatActionHelpers::CreateDustEffectForCreature): Ego_r.pdb
// ?GetRandomSlideFootParticleEffect@CMaterialDef@@QBEJXZ (retail 0x008EE5A0, landed; labels_familypass6 L093). Declaration only; no layout change.
public: long GetRandomSlideFootParticleEffect() const;
#endif
};
// Layout: original parent base, native OnHit reads +44/+48; six STL containers contract by4bytes.
FABLE_STATIC_ASSERT_AT(17, offsetof(CMaterialDef,OnHitRumbleLevel)==0x44);
FABLE_STATIC_ASSERT_AT(18, sizeof(CMaterialDef)==0x98);
#endif
