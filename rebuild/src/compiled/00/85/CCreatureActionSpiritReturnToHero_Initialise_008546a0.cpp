// Byte-exact reconstruction of CCreatureAction_SpiritReturnToHero::Initialise @ 0x008546a0
// Naming wave 18: the global stand-in g_spiritValue (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
#include "engine/CCreatureAction_SpiritReturnToHero.h"  // retyped onto the PDB layout; byte parity re-verified
class CCreatureActionBase { public: };

// Base class Initialise (call 0x694d10), this-call void
void __fastcall CCreatureActionBase_Initialise(CCreatureAction_SpiritReturnToHero* action);

// Global integer loaded via fild [0x1375550]
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

void __fastcall Initialise(CCreatureAction_SpiritReturnToHero* action)
{
    CCreatureActionBase_Initialise(action);
    action->field_14 = 0;
    action->field_10 = (long)(float)CMainGameComponent::GetConstantFPS();
}