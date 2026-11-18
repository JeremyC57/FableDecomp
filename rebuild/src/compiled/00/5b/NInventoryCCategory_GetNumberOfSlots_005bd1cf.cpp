// NInventory::CCategory::GetNumberOfSlots() const -- the category definition's NumberOfSlots, read through a by-value copy of the def pointer
// Leaf: ?GetNumberOfSlots@CCategory@NInventory@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD1CF, length 28 bytes; size-optimised TU. AddRef `inc [def+4]` (null-checked), the
//   NumberOfSlots (+0x40) load, then the Release `dec [def+4] ; jne ; call [vtbl+4]` (not null-checked).
// Callers: NInventory::CTCInventoryBase::GetNumberOfSlotsForCategory 0x005BD328 (NF89; binds this spelling).
// Name fix (agentNF91, q11197): Ego_r.pdb (0x00620102) + FableWin.pdb ?GetNumberOfSlots@CCategory@NInventory@@QBEJXZ (public const);
//   engine/CCategory.h declares it at this address (FABLE_RETAIL_CCATEGORY_NUMBEROFSLOTS, NF89). The old row defined
//   ?GetNumberOfSlots@CCategory_Retail@@QAEJXZ (non-const) on local CCategory_Retail / CDefPointer / CInventoryCategoryDef_Retail models;
//   now the member on the PASS22 NInventory::CCategory with the inline FableWin.pdb GetPDef() (CCATEGORY_GETPDEF) and the quarantine
//   CInventoryCategoryDef (CINVENTORYCATEGORYDEF_PARENTDEF), the 0x005BD054 recipe; FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE gives retail's
//   load-before-Release order.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_NUMBEROFSLOTS
#define FABLE_RETAIL_CCATEGORY_GETPDEF
#define FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/_quarantine/CInventoryCategoryDef.h"
namespace NInventory {
long CCategory::GetNumberOfSlots() const
{
    return GetPDef()->NumberOfSlots;
}
}
