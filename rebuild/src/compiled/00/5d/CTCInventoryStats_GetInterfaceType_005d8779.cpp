// CTCInventoryStats::GetInterfaceType() const -- the component's interface type, TCI_INVENTORY_STATS (0x14)
// Leaf: ?GetInterfaceType@CTCInventoryStats@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8779, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCInventoryStats's vtable 0x01254ADC (the slot holds this body in 1 vtable(s), owner CTCInventoryStats); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCInventoryStats@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCInventoryStats::vfunc_21 (rebuild/src/compiled/00/5d/CTCInventoryStats_vfunc21_005d8779.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYSTATS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryStats.h"
ETCInterfaceType CTCInventoryStats::GetInterfaceType() const
{
    return StatGetInterfaceType();
}
