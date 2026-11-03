#ifndef FABLE_ENGINE_CATTACKINGCOMBATANTINFO_H
#define FABLE_ENGINE_CATTACKINGCOMBATANTINFO_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/C2DVector.h"
class CThingCreatureBase;class CCombatSequenceBase;
class CAttackingCombatantInfo {
public:CThingCreatureBase* Attacked;long AttackerNumber;long CombatWheelRingIndex;bool PreferredAttackDollySet;
 long PreferredAttackDollyTimer;C2DVector PreferredAttackDollyVector;bool UsingCombatWheel;long CurrentSequence;
 std::vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;
};
FABLE_STATIC_ASSERT(sizeof(CAttackingCombatantInfo)==0x30);
#endif
