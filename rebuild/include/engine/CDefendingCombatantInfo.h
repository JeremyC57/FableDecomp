#ifndef FABLE_ENGINE_CDEFENDINGCOMBATANTINFO_H
#define FABLE_ENGINE_CDEFENDINGCOMBATANTINFO_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
class CThingCreatureBase;class CThing;class CAttackPatternDef;
class CDefendingCombatantInfo {
public:
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
 // Retail opt-in (chunk 1020, row 0x006E94D0): the complete Ego_r TPI CDefendingCombatantInfo::CProng (28 bytes: Blocked +0, Prong +4,
 // ProngBlockageTimer +0x10, POccupier +0x14) so a TU can take vector<CProng>::size(), and Ego_r.pdb
 // ?GetProngVector@CProng@CDefendingCombatantInfo@@QBE?AVC3DVector@@M@Z (out of line in retail at 0x006E7FD0; declaration only).
 class CProng {public:
  C3DVector GetProngVector(float rotationAngle) const;                           // 0x006E7FD0
  bool Blocked;C3DVector Prong;long ProngBlockageTimer;CIntelligentPointer<CThingCreatureBase> POccupier;};
#else
 class CProng;
#endif
 class CCombatRing {public:
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEEL
  // Retail opt-in (chunk 1020, row 0x006E94D0 CCombatManager::IsCreatureInAttackRingSector): Ego_r.pdb / FableWin.pdb
  // ?GetAttackerProng@CCombatRing@CDefendingCombatantInfo@@QBEPBVCProng@2@ABVCThingCreatureBase@@@Z (0x006E87D0),
  // ?GetAttackerProngVector@..@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z (0x006E8810) and ?GetProngSeparationHalfAngle@..@@QBEMXZ
  // (0x006E8460), all out of line in retail. Declarations only: no layout change.
  const CProng* GetAttackerProng(const CThingCreatureBase& attacker) const;     // 0x006E87D0
  C3DVector GetAttackerProngVector(const CThingCreatureBase& attacker) const;   // 0x006E8810
  float GetProngSeparationHalfAngle() const;                                    // 0x006E8460
#endif
  float RotationAngle;float DeltaRotation;long Index;float RingThickness;std::vector<CProng> Prongs;};
 class CCombatWheel {public:
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEEL
  // (same opt-in) ?GetRing@CCombatWheel@CDefendingCombatantInfo@@QBEPBVCCombatRing@2@J@Z (0x006E8520) and
  // ?GetRingRadius@..@@QBEMJ@Z (0x006E8850), out of line in retail. Declarations only.
  const CCombatRing* GetRing(long index) const;                                  // 0x006E8520
  float GetRingRadius(long ring) const;                                          // 0x006E8850
#endif
  std::vector<CCombatRing> CombatRings;};
 std::map<unsigned __int64,CThingCreatureBase*> Attackers;CCombatWheel CombatWheel;
 C3DVector LastVictimWheelUpdatePos;long LastWheelUpdateWorldFrame;CIntelligentPointer<CThing> PLastAttacker;
 CDefPointer<const CAttackPatternDef> PAttackPattern;long AttackPatternIndex;long FrameForNextInstruction;
};
FABLE_STATIC_ASSERT(sizeof(CDefendingCombatantInfo)==0x3c);
#ifdef FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
FABLE_STATIC_ASSERT(sizeof(CDefendingCombatantInfo::CProng)==0x1c);
#endif
#endif
