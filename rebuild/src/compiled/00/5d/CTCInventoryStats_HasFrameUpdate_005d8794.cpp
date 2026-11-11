// CTCInventoryStats::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCInventoryStats@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8794, thiscall, `ret`.
// Identity: slot 8 of CTCInventoryStats's vtable 0x01254ADC (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryStats or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCInventoryStats@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCInventoryStats.h (opt-ins as in the landed row CTCQuickAccessMenu_ConstructItemCategoryList_00626e40.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCQuickAccessMenu.h"
bool CTCInventoryStats::HasFrameUpdate() const
{
    return true;
}
