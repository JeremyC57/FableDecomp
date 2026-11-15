// CTCInventory::ResetPrimitiveHandles() -- release the inventory's main primitives and clear their handles.
// Leaf: ?ResetPrimitiveHandles@CTCInventory@@QAEXXZ
// Retail: 0x0058771C, 45 bytes, thiscall.
// Flags: none
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_PASS22_RESETPRIMITIVES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "engine/CTCInventory.h"
#include "stlport/fable_algorithm.h"
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
void CTCInventory::ResetPrimitiveHandles()
{
    std::for_each(InventoryMainPrimitiveHandles.begin(), InventoryMainPrimitiveHandles.end(),
                  std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    InventoryMainPrimitiveHandles.clear();
}