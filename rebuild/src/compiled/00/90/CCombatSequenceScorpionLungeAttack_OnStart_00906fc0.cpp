// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `field28` -> `LastFrameUsed`.
#include "engine/CCombatSequence_ScorpionLungeAttack.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatSequence_ScorpionLungeAttack;
extern int __fastcall re_call(CCombatSequence_ScorpionLungeAttack*);
void __fastcall OnStart(CCombatSequence_ScorpionLungeAttack* sequence)
{
    sequence->LastFrameUsed = re_call(sequence);
}