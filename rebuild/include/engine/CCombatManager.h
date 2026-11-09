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
 // Chunk 732 (CThingPlayerCreature melee rows): Ego_r.pdb spellings, out of line in retail. (FableWin's inline
 // GetProximityForNavigation is left out: only the parked 0x006AA720 uses it, pending the owner's access-level decision.)
 CThingCreatureBase* GetBestEnemyForCombatant(const CThingCreatureBase&,const C3DVector&) const;                       // 0x006EB700
 bool IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase&,const CThing&,const C3DVector&) const;     // 0x006EA5F0
 ECombatZone GetDefenderZone(const CThingCreatureBase&,const CThingCreatureBase&,long) const;                          // 0x006E99B0
 // Chunk 734 (row 0x004A5A40 CWorld::UpdateFrame): Ego_r.pdb / FableWin.pdb ?Update@CCombatManager@@QAEXXZ, retail 0x006ED200.
 void Update();                                                                                                       // 0x006ED200
#ifdef FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
 // Nested opt-in (chunk 892, row 0x0096E830 CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureNearCombatPosition): Ego_r.pdb
 // ?GetPreferredPos@CCombatManager@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z (0x00713700), out of line in retail at 0x006EA770
 // (hidden C3DVector result, then the creature reference; `ret 8`). Declaration only: no layout change.
 C3DVector GetPreferredPos(const CThingCreatureBase&) const;                                                         // 0x006EA770
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY
 // Nested opt-in (chunk 760, row 0x0093D270): FableWin.pdb ?GetProximityForNavigation@CCombatManager@@QBEMXZ (public const,
 // inline in retail: the row reads +0x28 straight off the manager).
 float GetProximityForNavigation() const { return ProximityForNavigation; }
#endif
#endif
private:
 std::map<CCharString,CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&,CThingCreatureBase&)> RegisteredCombatSequences;
 std::map<unsigned __int64,CCombatantInfo> Combatants;bool CombatStrafeTurningEnabled;CWorld& World;
 bool DrawDebug;bool TraceDebug;float ProximityForNavigation;bool EnableAINonAttackMode;float AttackPatternIdleStandardInterval;
 bool AttacksFromNearestCombatantOnly;CombatDialogueManager DialogueManager;
};
FABLE_STATIC_ASSERT(sizeof(CCombatManager)==0x5c);
#endif
