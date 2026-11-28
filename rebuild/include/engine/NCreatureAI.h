#ifndef FABLE_ENGINE_NCREATUREAI_H
#define FABLE_ENGINE_NCREATUREAI_H
#include "fable_compiler.h"
class CThingAICreature;class CThing;
#ifdef FABLE_RETAIL_NCREATUREAI_SPEAKING
class CThingCreatureBase;
#endif
#ifdef FABLE_RETAIL_NCREATUREAI_WASP_ATTACK_PASS23
class CThingCreatureBase;
#endif
class C3DVector;
class CThingCreatureBase;
namespace NCreatureAI {
#ifdef FABLE_RETAIL_CTCENEMY_ISENEMYOF_PASS30
bool FABLE_FASTCALL IsOverridingFactionEnemyOnTarget(const CThingAICreature*, const CThing*);
#endif
#ifdef FABLE_RETAIL_CREATURE_RANGED_COMBAT2_PASS27
bool FABLE_FASTCALL IsBestTargetForAttack(const CThingAICreature&, const CThingCreatureBase*);
#endif
#ifdef FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
bool FABLE_FASTCALL IsThingRunningAwayFromThing(const CThing&, const CThing&, const C3DVector&);  // 0x00864170
#endif
#ifdef FABLE_RETAIL_NCREATUREAI_WASP_ATTACK_PASS23
// Exact Ego_r/FableWin name and signature, native00864d00.
bool FABLE_FASTCALL IsToAttackTarget(CThingAICreature*,const CThingCreatureBase*);
#endif
// Both original PDBs, native008645e0; original IsRunnable donor02c2c6dd.
bool FABLE_FASTCALL IsAwareOfThing(const CThingAICreature*,const CThing*,bool);
#ifdef FABLE_RETAIL_NCREATUREAI_WILLING_TO_ATTACK
// Retail opt-in (egod3a, row 0x00803280 CTCTargetingPlayer::UpdateTargetThreatening): reloc_bindings / Ego_r.pdb
// ?IsWillingToAttackTarget@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@@Z (0x008648A0, __fastcall). Declaration only.
bool FABLE_FASTCALL IsWillingToAttackTarget(const CThingAICreature*,const CThing*);
#endif
#ifdef FABLE_RETAIL_NCREATUREAI_AWARE_INDIRECTLY
// Retail opt-in (agentNF41, row 0x00960FC0): Ego_r.pdb / FableWin.pdb ?IsAwareOfThingIndirectly@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z
// (0x00864700, __fastcall, ret 4).
bool FABLE_FASTCALL IsAwareOfThingIndirectly(const CThingAICreature*,const CThing*,bool);
#endif
#ifdef FABLE_RETAIL_NCREATUREAI_SPEAKING
// Retail opt-in (chunk 2023, row 0x0094C050): Ego_r.pdb / FableWin.pdb ?IsSpeaking@NCreatureAI@@YI_NABVCThingCreatureBase@@@Z
// (0x008644E0) and ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z (0x00864580, landed), free __fastcall functions.
bool FABLE_FASTCALL IsSpeaking(const CThingCreatureBase&);
void FABLE_FASTCALL StopSpeaking(CThingCreatureBase&);
#endif
}
#endif
