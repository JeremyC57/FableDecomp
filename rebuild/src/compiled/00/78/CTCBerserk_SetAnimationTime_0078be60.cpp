// Naming wave 18: the global stand-in g_someint (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
// Leaf: ?CTCBerserk_SetAnimationTime@@YIXPAVCTCBerserk@@M@Z
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
#include "engine/CTCBerserk.h"  // retyped onto the PDB layout; byte parity re-verified
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
void __fastcall CTCBerserk_SetAnimationTime(CTCBerserk* berserk, float a){ berserk->ReleasedAnimationTimer = (float)CMainGameComponent::GetConstantFPS() * a; }