#ifndef FABLE_ENGINE_CDEFENDINGCOMBATANTINFO_H
#define FABLE_ENGINE_CDEFENDINGCOMBATANTINFO_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
class CThingCreatureBase;class CThing;class CAttackPatternDef;
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PASS22_INIT
class CCombatManager;class CWorldMap;class CCombatTypeDef;class CAttackingCombatantInfo;
#endif
#ifdef FABLE_RETAIL_CCOMBATRING_ISOUTERRING
class CCombatTypeDef; // parameter type of CCombatRing::IsOuterRing (opt-in below)
#endif
class CDefendingCombatantInfo {
public:
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
 // Retail opt-in (chunk 1020, row 0x006E94D0): the complete Ego_r TPI CDefendingCombatantInfo::CProng (28 bytes: Blocked +0, Prong +4,
 // ProngBlockageTimer +0x10, POccupier +0x14) so a TU can take vector<CProng>::size(), and Ego_r.pdb
 // ?GetProngVector@CProng@CDefendingCombatantInfo@@QBE?AVC3DVector@@M@Z (out of line in retail at 0x006E7FD0; declaration only).
 class CProng {public:
#ifdef FABLE_RETAIL_COMBAT_PRONG_OCCUPIER_PASS26
  // FableWin 0240FDD0; native 006F2C0E reads the pointer at +0x14.
  CThingCreatureBase* GetPOccupyingAttacker() const { return POccupier.Get(); }
#endif
  C3DVector GetProngVector(float rotationAngle) const;                           // 0x006E7FD0
  bool Blocked;C3DVector Prong;long ProngBlockageTimer;CIntelligentPointer<CThingCreatureBase> POccupier;};
#else
 class CProng;
#endif
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PASS22_INIT
 class CCombatWheel;
#endif
 class CCombatRing {public:
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PASS22_INIT
  // Ego_r00711D10 / FableWin024113C0, retail006E8B60.
  void Reset();
  // Both PDBs: const world/wheel, mutable attacker map; retail006EBF70.
  void AssignAttackersToRing(const CWorldMap&,const CCombatWheel&,std::map<CThingCreatureBase*,CAttackingCombatantInfo*>&,const CThingCreatureBase&);
#endif
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEEL
  // Retail opt-in (chunk 1020, row 0x006E94D0 CCombatManager::IsCreatureInAttackRingSector): Ego_r.pdb / FableWin.pdb
  // ?GetAttackerProng@CCombatRing@CDefendingCombatantInfo@@QBEPBVCProng@2@ABVCThingCreatureBase@@@Z (0x006E87D0),
  // ?GetAttackerProngVector@..@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z (0x006E8810) and ?GetProngSeparationHalfAngle@..@@QBEMXZ
  // (0x006E8460), all out of line in retail. Declarations only: no layout change.
  const CProng* GetAttackerProng(const CThingCreatureBase& attacker) const;     // 0x006E87D0
  C3DVector GetAttackerProngVector(const CThingCreatureBase& attacker) const;   // 0x006E8810
  float GetProngSeparationHalfAngle() const;                                    // 0x006E8460
#endif
#ifdef FABLE_RETAIL_CCOMBATRING_ISOUTERRING
  // Opt-in (hand2a, row 0x006E89B0): Ego_r.pdb ?IsOuterRing@CCombatRing@CDefendingCombatantInfo@@QBE_NABVCCombatTypeDef@@@Z (0x00711110,
  // 16 bytes) / FableWin 0x024103D0 (33 bytes), parameter defender_combat_type_def (Ego_d); defined in combat_manager.cpp and expanded
  // in retail. Declaration only: no layout change.
  bool IsOuterRing(const CCombatTypeDef& defender_combat_type_def) const;
#endif
#ifdef FABLE_RETAIL_CCOMBATRING_GETTERS
  // Opt-in (p25K, row 0x006F2DE0 CGetBestAttackerForProng::operator()): Ego_d.pdb / FableWin.pdb (0x0243A9D0 / 0x0243A9F0)
  // ?GetIndex@CCombatRing@CDefendingCombatantInfo@@QBEJXZ and ?GetRotationAngle@CCombatRing@CDefendingCombatantInfo@@QBEMXZ, expanded in
  // retail (0x006F2E38 `mov ecx,[ebp+8]`, 0x006F2E4F `mov eax,[ebp]`). Inferred bodies (each returns its member).
  long GetIndex() const { return Index; }
  float GetRotationAngle() const { return RotationAngle; }
#endif
  float RotationAngle;float DeltaRotation;long Index;float RingThickness;std::vector<CProng> Prongs;};
 class CCombatWheel {public:
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PASS22_INIT
  // Ego_r007115A0 / FableWin024111A0; expanded in retail006EC850.
  CCombatRing* GetRing(long index){if(index>=0 && (unsigned long)index<CombatRings.size())return &CombatRings[index];return 0;}
  CCombatRing& AddRing(const CCombatTypeDef&); //006EC7D0
  void Init(CCombatManager&,CWorldMap&,const std::map<CThingCreatureBase*,CAttackingCombatantInfo*>&,const CThingCreatureBase&);
#endif
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEEL
  // (same opt-in) ?GetRing@CCombatWheel@CDefendingCombatantInfo@@QBEPBVCCombatRing@2@J@Z (0x006E8520) and
  // ?GetRingRadius@..@@QBEMJ@Z (0x006E8850), out of line in retail. Declarations only.
  const CCombatRing* GetRing(long index) const;                                  // 0x006E8520
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PLACEMENTVECTOR
  // Retail opt-in (Codex pass 24). Both PDBs and the hidden result at
  // 0x006E89B0 identify this const method.
  C3DVector GetAttackerPlacementVector(const CThingCreatureBase&, const CThingCreatureBase&, long) const;
#endif
  float GetRingRadius(long ring) const;                                          // 0x006E8850
#endif
#ifdef FABLE_RETAIL_CCOMBATWHEEL_UPDATE
  // Opt-in (hand4a, row 0x006ECEA0): Ego_r.pdb + FableWin.pdb ?Update@CCombatWheel@CDefendingCombatantInfo@@QAEXXZ (FableWin
  // 0x02411220), out of line in retail at 0x006E8560 (landed). Declaration only.
  void Update();                                                                 // 0x006E8560
#endif
  std::vector<CCombatRing> CombatRings;};
 std::map<unsigned __int64,CThingCreatureBase*> Attackers;CCombatWheel CombatWheel;
 C3DVector LastVictimWheelUpdatePos;long LastWheelUpdateWorldFrame;CIntelligentPointer<CThing> PLastAttacker;
 CDefPointer<const CAttackPatternDef> PAttackPattern;long AttackPatternIndex;long FrameForNextInstruction;
};
FABLE_STATIC_ASSERT_AT(44, sizeof(CDefendingCombatantInfo)==0x3c);
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
FABLE_STATIC_ASSERT_AT(46, sizeof(CDefendingCombatantInfo::CProng)==0x1c);
#endif
#endif
