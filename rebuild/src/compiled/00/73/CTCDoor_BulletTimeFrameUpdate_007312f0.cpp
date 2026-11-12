// CTCDoor::BulletTimeFrameUpdate() -- the bullet-time frame update is the ordinary frame update (virtual, slot 9)
// Leaf: ?BulletTimeFrameUpdate@CTCDoor@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007312F0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 10 of CTCDoor's vtable 0x01266DE4 (the slot holds this body in 1 vtable(s), owner CTCDoor); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts BulletTimeFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?BulletTimeFrameUpdate@CTCDoor@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCDoor::vfunc_10 (rebuild/src/compiled/00/73/CTCDoor_vfunc10_007312f0.cpp).
#define FABLE_RETAIL_CTCDOOR_VSLOTS3
#define FABLE_RETAIL_CTCDOOR
#define FABLE_RETAIL_CTCDOOR_VSLOTS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCDoor.h"
void CTCDoor::BulletTimeFrameUpdate()
{
    FrameUpdate();
}
