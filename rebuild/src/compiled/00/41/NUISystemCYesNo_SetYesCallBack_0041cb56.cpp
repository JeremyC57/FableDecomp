// NUISystem::CYesNo::SetYesCallBack(void __fastcall(void) (*)(void)) -- stores the yes callback (the Ego_r member is a plain void(*)(), the FableWin setter takes a __fastcall pointer)
// Leaf: ?SetYesCallBack@CYesNo@NUISystem@@UAEXP6IXXZ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041CB56, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 142 of NUISystem::CYesNo's vtable 0x0122F884 (the slot holds this body in 1 vtable(s), owner NUISystem::CYesNo); the Xbox 360 Test PDB virtual order (third source) aligned on 64 TLC-named slots of that vtable puts SetYesCallBack(void (*)(void)) at this slot [gap kind GSC]; Ego_r/FableWin name set: ?SetYesCallBack@CYesNo@NUISystem@@UAEXP6IXXZ@Z; no `ret` to check (UNKSIZE).
// Was: NUISystem::CYesNo::vfunc_142 (rebuild/src/compiled/00/41/gapscan_sub0041cb56_0041cb56.cpp).
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_CYESNO_CALLBACKS
#define FABLE_RETAIL_CYESNO_CTOR
#include "engine/NUISystem_CYesNo.h"
namespace NUISystem {
void CYesNo::SetYesCallBack(void (__fastcall* callback)())
{
    YesCallback = (void (*)())callback;
}
}
