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
#ifdef FABLE_RETAIL_AI_CREATURE_UPDATE_PASS29
class CCombatActionBase; class CThingCreatureBase;
#endif
class CCombatManager:public CBaseClassNonCopyable {
#ifdef FABLE_RETAIL_AI_CREATURE_UPDATE_PASS29
public:
    CCombatActionBase* GetNextActionForCreature(CThingCreatureBase& creature);
    void SetCreatureAsAttemptedToPerformedAction(CThingCreatureBase& combatant, bool action_plugged_in);
#endif
public:
#ifdef FABLE_RETAIL_COMBAT_DEFENDER_ZONE_PASS23
 // Original FableWin02412a40 and both PDBs; native006e92c0, expanded at006e99b0.
 bool IsAttacker(const CThingCreatureBase& creature)const {
     std::map<unsigned __int64,CCombatantInfo>::const_iterator found=Combatants.find(creature.GetUniqueID());
     return found!=Combatants.end() && found->second.AttackerInfo.Attacked!=0;
 }
#endif
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
#ifdef FABLE_RETAIL_CCOMBATMANAGER_ISATTACKER
 // Opt-in (agentNF25b, 2026-11-12): Ego_r.pdb (0x00712500) + FableWin.pdb (0x02412A40) ?IsAttacker@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z,
 // out of line at 0x006E92C0 (landed row; the Combatants lookup of the creature's UID, then its AttackerInfo.Attacked). Supersedes the
 // FABLE_RETAIL_CCOMBATMANAGER_ISATTACKING spelling (in neither PDB). Declaration only.
 bool IsAttacker(const CThingCreatureBase& creature) const;             // 0x006E92C0
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_ISATTACKERON
 // Opt-in (NF207, 2026-11-20, row 0x006E9310): Ego_r.pdb (0x00712570) + FableWin.pdb (0x02412B50)
 // ?IsAttackerOn@CCombatManager@@QBE_NABVCThingCreatureBase@@0@Z, out of line at 0x006E9310 (the IsAttacker twin: the Combatants
 // lookup of the attacker's UID, then its AttackerInfo.Attacked compared with the defender). Declaration only: no layout change.
 bool IsAttackerOn(const CThingCreatureBase& attacker, const CThingCreatureBase& defender) const;   // 0x006E9310
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_ISDEFENDERDUEFORATTACK
 // Opt-in (NF210, 2026-11-20, row 0x006EA050; drafted by NF207): the two public const overloads of Ego_r.pdb (0x00712BC0 / 0x00713BF0)
 // + FableWin.pdb (0x024187C0 / 0x024185D0) ?IsDefenderDueForAttack@CCombatManager@@QBE_NABUCCombatantInfo@@@Z (out of line at
 // 0x006E97F0, landed under that Leaf) and ?IsDefenderDueForAttack@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z (by body 0x006EA050:
 // the defender's Combatants entry, a non-empty DefenderInfo.Attackers, then the CCombatantInfo overload on the entry).
 // Declarations only: no layout change.
 bool IsDefenderDueForAttack(const CCombatantInfo& info) const;                // 0x006E97F0
 bool IsDefenderDueForAttack(const CThingCreatureBase& defender) const;        // 0x006EA050
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_GETCOMBATDEBUGINFO
 // Opt-in (NF210, 2026-11-20, row 0x006EAC50): Ego_r.pdb (0x00715140) + FableWin.pdb (0x0241C2D0)
 // ?GetCombatDebugInfo@CCombatManager@@QBE?AVCCombatDebugInfo@@ABVCThingCreatureBase@@@Z (public const; CCombatDebugInfo returned by
 // value, complete type supplied by the including row: no shared header). Declaration only: no layout change.
 CCombatDebugInfo GetCombatDebugInfo(const CThingCreatureBase& creature) const;   // 0x006EAC50
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
 // Retail opt-in (chunk Q17, row 0x0093C030): Ego_r.pdb ?CreatureUnsheathWeapon@CCombatManager@@QAEXAAVCThingCreatureBase@@W4EUnsheatheWeaponType@@@Z
 // (out of line at 0x006EBDD0). Declaration only; needs engine/EUnsheatheWeaponType.h.
 void CreatureUnsheathWeapon(CThingCreatureBase& creature, EUnsheatheWeaponType type);   // 0x006EBDD0
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_REGISTERCOMBATSEQUENCES
 // Opt-in (agentNF69, row 0x008B0D10): Ego_r.pdb (0x008EC2B0) + FableWin.pdb (0x02A2F600) private ?RegisterCombatSequences@CCombatManager@@AAEXXZ
 // (retail 0x008B0D10, called once from the constructor 0x006ED3F0, which binds that spelling) and the name -> creator insert it calls 63
 // times, 0x006EAB60, under the landed binding ?RegisterCombatSequence@CCombatManager@@QAEXP6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@ZVCCharString@@@Z
 // (in neither PDB name set: Ego_r and FableWin inline it). Declarations only; the includer supplies CCombatSequenceBase.
 void RegisterCombatSequence(CCombatSequenceBase* (__fastcall* creator)(CThingCreatureBase&,CThingCreatureBase&),CCharString name);   // 0x006EAB60
private:
 void RegisterCombatSequences();                                                                                     // 0x008B0D10
public:
#endif
 void RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool);
#ifdef FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERREADY
 // Opt-in (row 0x00972CC0): Ego_r.pdb + FableWin.pdb ?SetAttackerAsReadyToAttack@CCombatManager@@QAEXABVCThingCreatureBase@@_N@Z (0x006EA450, landed).
 void SetAttackerAsReadyToAttack(const CThingCreatureBase& attacker, bool ready);   // 0x006EA450
#endif
 void DeregisterCombatant(CThingCreatureBase&);
#ifdef FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERASBEENSTRUCK
 // Opt-in (NF341, 2026-11-24, row 0x006EA430 and its binder 0x00734050 CTCCombat::SetBlocked): Ego_r.pdb (0x00713F20) + FableWin.pdb
 // (0x0241CC10) ?SetAttackerAsBeenStruck@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z, out of line at 0x006EA430 (landed; it was spelled
 // DeregisterCombatant, which is 0x006ED9C0). Ego_r release\combat_manager.obj lists PerformSequenceReset (record 80) then
 // SetAttackerAsBeenStruck (record 81, 32 B), as retail 0x006EA2C0 / 0x006EA430; the FableWin body calls IsInMeleeAsAttacker then
 // PerformSequenceReset, as retail 0x006EA430 (FableWin's DeregisterCombatant calls DeregisterAttackingCombatant, as retail 0x006ED9C0).
 // The PERFORMSEQUENCERESET note below names 0x006EA430 by its old spelling. Declaration only: no layout change.
 void SetAttackerAsBeenStruck(CThingCreatureBase& attacker);         // 0x006EA430
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_PERFORMSEQUENCERESET
 // Opt-in (agentNF22, row 0x006EA430 DeregisterCombatant): Ego_r.pdb + FableWin.pdb private
 // ?PerformSequenceReset@CCombatManager@@AAEXAAVCThingCreatureBase@@@Z, out of line at 0x006EA2C0 (landed under that name).
private:
 void PerformSequenceReset(CThingCreatureBase&);
public:
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_DEFENDERATTACKPATTERN
 // Opt-in (wf1, row 0x006EA0C0 UpdateDefenderAttackPattern): Ego_r.pdb + FableWin.pdb private
 // ?UpdateDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@@Z (out of line at 0x006EA0C0) and
 // ?ResetDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@V?$CDefPointer@$$CBVCAttackPatternDef@@@@@Z (out of line at
 // 0x006E8C30, landed under that name). Declarations only: no layout change.
private:
 void UpdateDefenderAttackPattern(CCombatantInfo& info);                                        // 0x006EA0C0
 void ResetDefenderAttackPattern(CCombatantInfo& info, CDefPointer<const CAttackPatternDef> pattern);   // 0x006E8C30
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
#ifdef FABLE_RETAIL_CCOMBATMANAGER_SETUPPLACEMENTWHEEL
 // Opt-in (hand4a, row 0x006ECEA0): Ego_r.pdb (0x007167A0) + FableWin.pdb (0x0241AD40) private
 // ?SetupDefenderPlacementWheel@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z, with the two private inlines it expands: FableWin
 // ?GetWorld@CCombatManager@@ABEAAVCWorld@@XZ (0x024128A0; retail reads World +0x20 in place) and the non-const
 // ?GetCombatantInfo@CCombatManager@@AAEAAUCCombatantInfo@@ABVCThingCreatureBase@@@Z (Ego_r + FableWin 0x02415CC0; retail expands it:
 // the UniqueID copied into a key temporary and looked up in Combatants, _M_find 0x006F0250, node + 0x18). No layout change.
private:
 CWorld& GetWorld() const { return World; }
 CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) { return Combatants.find(creature.GetUniqueID())->second; }
 void SetupDefenderPlacementWheel(CCombatantInfo& combat_info, bool force_wheel_update);   // 0x006ECEA0
public:
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
#ifdef FABLE_RETAIL_CCOMBATMANAGER_CTOR
 // Opt-in (chunk 2051, row 0x004A6E30 CWorld::Init): Ego_r.pdb ??0CCombatManager@@QAE@AAVCWorld@@@Z (retail 0x006ED3F0) and ?Init@CCombatManager@@QAEXXZ (retail
 // 0x006E8300: DialogueManager.Init() at +0x38, then +0x30 = 0.5f, +0x34 = false; the catalog's "ResetDialogue" label is wrong).
 CCombatManager(CWorld& world);                                                                                       // 0x006ED3F0
 void Init();                                                                                                         // 0x006E8300
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
 // Nested opt-in (chunk 892, row 0x0096E830 CAIStateGroup_MeleeCombatNearby::IsEnemyCreatureNearCombatPosition): Ego_r.pdb
 // ?GetPreferredPos@CCombatManager@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z (0x00713700), out of line in retail at 0x006EA770
 // (hidden C3DVector result, then the creature reference; `ret 8`). Declaration only: no layout change.
 C3DVector GetPreferredPos(const CThingCreatureBase&) const;                                                         // 0x006EA770
#endif
#ifdef FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY
 // Nested opt-in (chunk 760, row 0x0093D270): FableWin.pdb ?GetProximityForNavigation@CCombatManager@@QBEMXZ (public const,
 // inline in retail: the row reads +0x28 straight off the manager).
 // Owner decision 2026-11-22 (land as Inferred): retail reads this getter like a reference at both call sites (0x006AAA24,
 // 0x0093D351; a `const float&` return graded RELOCATION_MATCH on 0x0093D270, the by-value form DIFFER(305v295)), against all three
 // later VC10 PDBs (QBEMXZ, by value). Inferred TLC-era form; off by default. pending/2026-11-22_auth18/0093d270/TERMINAL.md.
#ifdef FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY_REF_INFERRED
 const float& GetProximityForNavigation() const { return ProximityForNavigation; }
#else
 float GetProximityForNavigation() const { return ProximityForNavigation; }
#endif
#endif
#endif
private:
#ifdef FABLE_RETAIL_COMBAT_SEQUENCE_SELECTION_PASS27
 long GetBestSequenceIndexAttackerOnDefender(const CCombatantInfo& attacker_info,const CCombatantInfo& defender_info) const;
 long PickBestSequenceFromIndexList(const std::vector<long>& index_list) const;
 CWorld& GetWorld() const { return World; }
 const CThingSearchTools& PeekThingSearchTools() const { return World.PeekThingSearchTools(); }
#endif
 std::map<CCharString,CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&,CThingCreatureBase&)> RegisteredCombatSequences;
 std::map<unsigned __int64,CCombatantInfo> Combatants;bool CombatStrafeTurningEnabled;CWorld& World;
 bool DrawDebug;bool TraceDebug;float ProximityForNavigation;bool EnableAINonAttackMode;float AttackPatternIdleStandardInterval;
 bool AttacksFromNearestCombatantOnly;CombatDialogueManager DialogueManager;
};
FABLE_STATIC_ASSERT_AT(103, sizeof(CCombatManager)==0x5c);
#endif
