// Naming wave 18: the global stand-in g_spiritInt (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
#include "engine/CCreatureAction_SpiritAttack.h"

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern float g_spiritMul;
extern "C" int __cdecl SpiritHelper();

struct CCreatureAction_SpiritAttack_Methods : CCreatureAction_SpiritAttack {
    virtual void Initialise();
};
void CCreatureAction_SpiritAttack_Methods::Initialise() {
    SpiritHelper();
    CCreatureAction_SpiritAttack* action =
        reinterpret_cast<CCreatureAction_SpiritAttack*>(this);
    action->field10 = (int)((float)CMainGameComponent::GetConstantFPS() * g_spiritMul);
}
