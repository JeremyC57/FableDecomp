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
 class CProng;
 class CCombatRing {public:float RotationAngle;float DeltaRotation;long Index;float RingThickness;std::vector<CProng> Prongs;};
 class CCombatWheel {public:std::vector<CCombatRing> CombatRings;};
 std::map<unsigned __int64,CThingCreatureBase*> Attackers;CCombatWheel CombatWheel;
 C3DVector LastVictimWheelUpdatePos;long LastWheelUpdateWorldFrame;CIntelligentPointer<CThing> PLastAttacker;
 CDefPointer<const CAttackPatternDef> PAttackPattern;long AttackPatternIndex;long FrameForNextInstruction;
};
FABLE_STATIC_ASSERT(sizeof(CDefendingCombatantInfo)==0x3c);
#endif
