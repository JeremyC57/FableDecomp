// NInventory::CTCInventoryBase::GetComponents() -- reference to the screen's component vector (+0x0C)
// Leaf: ?GetComponents@CTCInventoryBase@NInventory@@UAEAAV?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@XZ
// Identity: slot 82 of NInventory::CTCInventoryBase's own vtable 0x01253DBC (the body sits at that slot in 11 vtables, all CTCInventoryBase
//   or derived); engine/NInventory_CTCInventoryBase.h FABLE_RETAIL_CTCINVENTORYBASE_VTABLE lists the class's virtuals in retail slot order
//   and names slot 82 GetComponents; the Xbox 360 Test PDB order aligned on the TLC-named slots agrees; Ego_r/FableWin name set: ?GetComponents@CTCInventoryBase@NInventory@@UAEAAV?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@XZ.
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
std::vector<NUISystem::CComponent*>& CTCInventoryBase::GetComponents()
{
    return Components;
}
}
