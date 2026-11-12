// NUISystem::COk::SetOkCallBack(void __fastcall(void) (*)(void)) -- stores the ok callback (the Ego_r member is a plain void(*)(), the FableWin setter takes a __fastcall pointer)
// Leaf: ?SetOkCallBack@COk@NUISystem@@UAEXP6IXXZ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041CBD7, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 142 of NUISystem::COk's vtable 0x0122FAF4 (the slot holds this body in 1 vtable(s), owner NUISystem::COk); the Xbox 360 Test PDB virtual order (third source) aligned on 64 TLC-named slots of that vtable puts SetOkCallBack(void (*)(void)) at this slot [gap kind GSC]; Ego_r/FableWin name set: ?SetOkCallBack@COk@NUISystem@@UAEXP6IXXZ@Z; no `ret` to check (UNKSIZE).
// Was: NUISystem::COk::vfunc_142 (rebuild/src/compiled/00/41/gapscan_sub0041cbd7_0041cbd7.cpp).
#define FABLE_RETAIL_COK_VSLOTS
#include "engine/NUISystem_COk.h"
namespace NUISystem {
void COk::SetOkCallBack(void (__fastcall* callback)())
{
    OkCallback = (void (*)())callback;
}
}
