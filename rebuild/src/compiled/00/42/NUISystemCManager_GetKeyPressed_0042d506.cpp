// NUISystem::CManager::GetKeyPressed() -- returns KeyPressed
// Leaf: ?GetKeyPressed@CManager@NUISystem@@UAEGXZ
// Leaf note: the PDB name ?GetKeyPressed@CManager@NUISystem@@UAE_WXZ in its retail G form (wide text is unsigned short, no /Zc:wchar_t: owner rule 2026-10-29, FINDINGS_LOG); the header declares unsigned short GetKeyPressed().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D506, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 13 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 1 vtable(s), owner NUISystem::CManager); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts GetKeyPressed(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetKeyPressed@CManager@NUISystem@@UAE_WXZ; `ret` matches the signature.
// Was: _unattributed::vfunc_13 (rebuild/src/compiled/00/42/unattributed_vfunc13_0042d506.cpp).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
unsigned short CManager::GetKeyPressed()
{
    return KeyPressed;
}
}
