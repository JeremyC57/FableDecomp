// CTCInventoryMagic::CloseInventory() -- close the inventory and release its magic-screen handles.
// Leaf: ?CloseInventory@CTCInventoryMagic@@UAEXXZ
// Retail: 0x0072DC40, 35 bytes, thiscall.
// Flags: none
// Identity: Ego_r00744B30/FableWin02555930; native primary vtable012668EC slot31.
// Calls: qualified NInventory::CTCInventoryBase::CloseInventory005BD597, then vector erase00440D70.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYMAGIC_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "engine/CTCInventoryMagic.h"

void CTCInventoryMagic::CloseInventory()
{
    NInventory::CTCInventoryBase::CloseInventory();
    InventoryMagicPrimitiveHandles.clear();
}