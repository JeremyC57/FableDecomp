// Byte-exact reconstruction of CCombatSequence_ScorpionLungeAttack::IsValid @ 0x00906fa0
// Leaf: ?IsValid@CCombatSequence_ScorpionLungeAttack@@UBE_NXZ
// Naming wave 18: the global stand-in g_globalMultiplier (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).

// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `field28` -> `LastFrameUsed`.
#define FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_ScorpionLungeAttack.h"  // retyped onto the PDB layout; byte parity re-verified
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld { public: static long __fastcall GetFrame(); };      // 0x49d870

bool CCombatSequence_ScorpionLungeAttack::IsValid() const
{
    CCombatSequence_ScorpionLungeAttack* self = const_cast<CCombatSequence_ScorpionLungeAttack*>(this);
    int mult = CMainGameComponent::GetConstantFPS();
    if (CWorld::GetFrame() - self->LastFrameUsed > mult * 10)
        return true;
    return false;
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_ScorpionLungeAttack_IsValid -> CCombatSequence_ScorpionLungeAttack::IsValid, the RTTI-placed PDB member; class model: engine/CCombatSequence_ScorpionLungeAttack.h under FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
