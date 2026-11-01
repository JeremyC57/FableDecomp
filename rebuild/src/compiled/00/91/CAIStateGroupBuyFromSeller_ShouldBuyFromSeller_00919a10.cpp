// Byte-exact reconstruction of CAIStateGroup_BuyFromSeller::ShouldBuyFromSeller @ 0x00919a10
// Naming wave 18: the global stand-in g_div_1375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// __fastcall bool f(Self* this)


// call target at 0x49d870: no-arg getter returning int in eax
#include "engine/CAIStateGroup_BuyFromSeller.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};   // 0x0049D870

// globals
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern int g_cmp_138c694;   // [0x138c694] compare value

bool __fastcall ShouldBuyFromSeller(CAIStateGroup_BuyFromSeller* stateGroup)
{
    int elapsed = CWorld::GetFrame() - stateGroup->FrameLastBought;
    int quot = elapsed / CMainGameComponent::GetConstantFPS();
    if ((unsigned int)quot > (unsigned int)g_cmp_138c694)
        return true;
    return false;
}