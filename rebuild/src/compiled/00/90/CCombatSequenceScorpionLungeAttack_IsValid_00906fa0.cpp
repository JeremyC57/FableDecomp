// Byte-exact reconstruction of CCombatSequence_ScorpionLungeAttack::IsValid @ 0x00906fa0

// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `field28` -> `LastFrameUsed`.
#include "engine/CCombatSequence_ScorpionLungeAttack.h"  // retyped onto the PDB layout; byte parity re-verified
extern int g_globalMultiplier;                 // ds:0x1375550
class CWorld { public: static long __fastcall GetFrame(); };      // 0x49d870

bool __fastcall IsValid(CCombatSequence_ScorpionLungeAttack* self)
{
    int mult = g_globalMultiplier;
    if (CWorld::GetFrame() - self->LastFrameUsed > mult * 10)
        return true;
    return false;
}