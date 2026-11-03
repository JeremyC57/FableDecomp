#ifndef FABLE_ENGINE_CCOMBATMANAGER_H
#define FABLE_ENGINE_CCOMBATMANAGER_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCombatantInfo.h"
#include "engine/CombatDialogueManager.h"
#include "fable_string.h"
class CWorld;
#ifdef FABLE_RETAIL_CCOMBATMANAGER
class C3DVector; class CThing; enum ECombatZone;
#endif
class CCombatManager:public CBaseClassNonCopyable {
public:
 void RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool);
 void DeregisterCombatant(CThingCreatureBase&);
 const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase&)const;
#ifdef FABLE_RETAIL_CCOMBATMANAGER
 // Chunk 732 (CThingPlayerCreature melee rows): Ego_r.pdb spellings. GetProximityForNavigation is FableWin-only (inline in retail:
 // `mov edx,[eax+28h]`, ProximityForNavigation); the others are out of line.
 float GetProximityForNavigation() const { return ProximityForNavigation; }
 CThingCreatureBase* GetBestEnemyForCombatant(const CThingCreatureBase&,const C3DVector&) const;                       // 0x006EB700
 bool IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase&,const CThing&,const C3DVector&) const;     // 0x006EA5F0
 ECombatZone GetDefenderZone(const CThingCreatureBase&,const CThingCreatureBase&,long) const;                          // 0x006E99B0
#endif
private:
 std::map<CCharString,CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&,CThingCreatureBase&)> RegisteredCombatSequences;
 std::map<unsigned __int64,CCombatantInfo> Combatants;bool CombatStrafeTurningEnabled;CWorld& World;
 bool DrawDebug;bool TraceDebug;float ProximityForNavigation;bool EnableAINonAttackMode;float AttackPatternIdleStandardInterval;
 bool AttacksFromNearestCombatantOnly;CombatDialogueManager DialogueManager;
};
FABLE_STATIC_ASSERT(sizeof(CCombatManager)==0x5c);
#endif
