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
#if defined(FABLE_RETAIL_LEADER_INITIALISE) || defined(FABLE_RETAIL_CCOMBATMANAGER_DIALOGUE)
 // FABLE_RETAIL_CCOMBATMANAGER_DIALOGUE (chunk Q17, row 0x0093C030): the same Ego_r.pdb inline accessor
 // ?GetCombatDialogueManager@CCombatManager@@QAEAAVCombatDialogueManager@@XZ (`lea ecx,[eax+38h]`, 0x0093C11B) without the LEADER opt-in.
 CombatDialogueManager& GetCombatDialogueManager(){return DialogueManager;}
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_ISATTACKING
 // Retail opt-in (chunk Q17, row 0x008BF500): ?IsCreatureAttacking@CCombatManager@@QAE_NABVCThingCreatureBase@@@Z, out of line at
 // 0x006E92C0 (the Combatants lookup of the creature's UID, then its AttackerInfo.Attacked). The name is the landed binding's (in neither
 // PDB name set). Declaration only.
 bool IsCreatureAttacking(const CThingCreatureBase& creature);           // 0x006E92C0
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
 // Retail opt-in (chunk Q17, row 0x0093C030): Ego_r.pdb ?CreatureUnsheathWeapon@CCombatManager@@QAEXAAVCThingCreatureBase@@W4EUnsheatheWeaponType@@@Z
 // (out of line at 0x006EBDD0). Declaration only; needs engine/EUnsheatheWeaponType.h.
 void CreatureUnsheathWeapon(CThingCreatureBase& creature, EUnsheatheWeaponType type);   // 0x006EBDD0
#endif
 void RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool);
 void DeregisterCombatant(CThingCreatureBase&);
#ifdef FABLE_RETAIL_CCOMBATMANAGER_PERFORMSEQUENCERESET
 // Opt-in (agentNF22, row 0x006EA430 DeregisterCombatant): Ego_r.pdb + FableWin.pdb private
 // ?PerformSequenceReset@CCombatManager@@AAEXAAVCThingCreatureBase@@@Z, out of line at 0x006EA2C0 (landed under that name).
private:
 void PerformSequenceReset(CThingCreatureBase&);
public:
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_GETCOMBATANTINFO_INLINE
 // Per-TU opt-in (chunk 1020, row 0x006E94D0): the public const ?GetCombatantInfo@CCombatManager@@QBEABUCCombatantInfo@@ABVCThingCreatureBase@@@Z
 // (out of line at 0x006E9360) with its body visible: the UniqueID copied into a key temporary and looked up in Combatants (the tree's
 // _M_find 0x006F0250, node + 0x18). IsCreatureInAttackRingSector 0x006E94D0 expands it twice. Same body as the out-of-line copy.
 const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature)const{return Combatants.find(creature.GetUniqueID())->second;}
 bool IsCreatureInAttackRingSector(const CThingCreatureBase& attacker)const;   // 0x006E94D0 ?IsCreatureInAttackRingSector@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z
#else
 const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase&)const;
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER
 // Chunk 732 (CThingPlayerCreature melee rows): Ego_r.pdb spellings, out of line in retail. (FableWin's inline
 // GetProximityForNavigation is left out: only the parked 0x006AA720 uses it, pending the owner's access-level decision.)
 CThingCreatureBase* GetBestEnemyForCombatant(const CThingCreatureBase&,const C3DVector&) const;                       // 0x006EB700
 bool IsTargetOverriddenByAttemptedFacingDirection(const CThingCreatureBase&,const CThing&,const C3DVector&) const;     // 0x006EA5F0
 ECombatZone GetDefenderZone(const CThingCreatureBase&,const CThingCreatureBase&,long) const;                          // 0x006E99B0
 // Chunk 734 (row 0x004A5A40 CWorld::UpdateFrame): Ego_r.pdb / FableWin.pdb ?Update@CCombatManager@@QAEXXZ, retail 0x006ED200.
#ifdef FABLE_RETAIL_CCOMBATMANAGER_OUTERRING
 bool IsCreatureOnOuterRing(const CThingCreatureBase&) const;
#endif
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
