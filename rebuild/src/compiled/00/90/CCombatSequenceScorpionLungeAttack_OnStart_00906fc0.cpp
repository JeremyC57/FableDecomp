// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `field28` -> `LastFrameUsed`.
// Leaf: ?OnStart@CCombatSequence_ScorpionLungeAttack@@MAEXXZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_ScorpionLungeAttack.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
void CCombatSequence_ScorpionLungeAttack::OnStart()
{
    CCombatSequence_ScorpionLungeAttack* sequence = this;
    sequence->LastFrameUsed = CWorld::GetFrame();
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_ScorpionLungeAttack_OnStart -> CCombatSequence_ScorpionLungeAttack::OnStart, the RTTI-placed PDB member; class model: engine/CCombatSequence_ScorpionLungeAttack.h under FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
