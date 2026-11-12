// NInventory::CTCInventoryBase::GetIsEnabled() const -- an inventory screen is enabled by default (returns true)
// Leaf: ?GetIsEnabled@CTCInventoryBase@NInventory@@UBE_NXZ
// Identity: slot 75 of NInventory::CTCInventoryBase's own vtable 0x01253DBC (the body sits at that slot in 9 vtables, all CTCInventoryBase
//   or derived); engine/NInventory_CTCInventoryBase.h FABLE_RETAIL_CTCINVENTORYBASE_VTABLE lists the class's virtuals in retail slot order
//   and names slot 75 GetIsEnabled; the Xbox 360 Test PDB order aligned on the TLC-named slots agrees; Ego_r/FableWin name set: ?GetIsEnabled@CTCInventoryBase@NInventory@@UBE_NXZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTHING
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
bool CTCInventoryBase::GetIsEnabled() const
{
    return true;
}
}
