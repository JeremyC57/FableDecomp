// NSpeechGainManager::CDialogueLayerDef::Copy(CDefClassBase const*) -- copies another dialogue-layer def into this one: the out-of-line assignment, tail-called
// Leaf: ?Copy@CDialogueLayerDef@NSpeechGainManager@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046656A, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 19 of NSpeechGainManager::CDialogueLayerDef's vtable 0x0123543C (the slot holds this body in 1 vtable(s), owner NSpeechGainManager::CDialogueLayerDef); the Xbox 360 Test PDB virtual order (third source) aligned on 23 TLC-named slots of that vtable puts Copy(CDefClassBase const *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?Copy@CDialogueLayerDef@NSpeechGainManager@@UAEXPBVCDefClassBase@@@Z; no `ret` to check (NORET).
// Was: NSpeechGainManager::CDialogueLayerDef::vfunc_19 (rebuild/src/compiled/00/46/gapscan_sub0046656a_0046656a.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_NSPEECHGAINMANAGER_CDIALOGUELAYERDEF_COPY
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/NSpeechGainManager.h"
namespace NSpeechGainManager {
void CDialogueLayerDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CDialogueLayerDef*>(other);
}
}
