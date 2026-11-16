// NEntityEvents::CEventHitBase::GetHitParams() const -- the hit event's hit parameters
// Leaf: ?GetHitParams@CEventHitBase@NEntityEvents@@QBEABVCHitParameters@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810F40: mov eax,[ecx+8] ; ret (PHitParams +0x08).
// Name: Ego_r.pdb 0x00807060 + FableWin.pdb 0x025A7ED0 ?GetHitParams@CEventHitBase@NEntityEvents@@QBEABVCHitParameters@@XZ; its one
//   retail caller 0x0094F22C (landed 0x0094F180 IsRunnable) binds this symbol through the same header opt-in. Sits with the other
//   CEventHitBase getters (GetPHitBy 0x00810FC0, GetPWeaponUsed 0x00810FD0). NF33 (2026-11-16): was the free stand-in DrawGetEnvironment.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBASE_GETHITPARAMS
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
namespace NEntityEvents {
const CHitParameters& CEventHitBase::GetHitParams() const
{
    return *PHitParams;
}
}
