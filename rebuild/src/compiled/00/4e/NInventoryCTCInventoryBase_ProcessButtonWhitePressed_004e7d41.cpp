// NInventory::CTCInventoryBase::ProcessButtonWhitePressed() -- no White-button action by default (empty, `ret`)
// Leaf: ?ProcessButtonWhitePressed@CTCInventoryBase@NInventory@@UAEXXZ
// Identity: slot 66 of NInventory::CTCInventoryBase's own vtable 0x01253DBC (the body sits at that slot in 11 vtables, all CTCInventoryBase
//   or derived); engine/NInventory_CTCInventoryBase.h FABLE_RETAIL_CTCINVENTORYBASE_VTABLE lists the class's virtuals in retail slot order
//   and names slot 66 ProcessButtonWhitePressed; the Xbox 360 Test PDB order aligned on the TLC-named slots agrees; Ego_r/FableWin name set: ?ProcessButtonWhitePressed@CTCInventoryBase@NInventory@@UAEXXZ.
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
void CTCInventoryBase::ProcessButtonWhitePressed()
{
}
}
