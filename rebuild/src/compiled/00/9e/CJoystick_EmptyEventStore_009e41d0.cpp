// CJoystick::EmptyEventStore() -- forget the stored input events (NoStoredEvents = 0)
// Leaf: ?EmptyEventStore@CJoystick@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E41D0: `mov dword ptr [ecx+0D10h], 0 ; ret`.
// Name (NF197, 2026-11-20): Ego_r.pdb / FableWin.pdb ?EmptyEventStore@CJoystick@@IAEXXZ (protected, non-virtual); engine/CJoystick.h
//   (FABLE_RETAIL_CJOYSTICK_VIRTUALS) declares it at this address and puts NoStoredEvents at +0xD10; UpdateEvents 0x00AB6E40 calls it
//   first and binds this spelling. Was the stand-in C009e41d0::M (catalog label _global::SetMemberConst_009e41d0 / CGameEventPackage::Clear).
#define FABLE_RETAIL_CJOYSTICK
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#include "engine/CJoystick.h"

void CJoystick::EmptyEventStore()
{
    NoStoredEvents = 0;
}
