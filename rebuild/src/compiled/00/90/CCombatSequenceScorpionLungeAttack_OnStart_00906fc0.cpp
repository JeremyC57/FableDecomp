// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `field28` -> `LastFrameUsed`.
// Leaf: ?CCombatSequence_ScorpionLungeAttack_OnStart@@YIXPAUCCombatSequence_ScorpionLungeAttack@@@Z
#include "engine/CCombatSequence_ScorpionLungeAttack.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatSequence_ScorpionLungeAttack;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
void __fastcall CCombatSequence_ScorpionLungeAttack_OnStart(CCombatSequence_ScorpionLungeAttack* sequence)
{
    sequence->LastFrameUsed = CWorld::GetFrame();
}