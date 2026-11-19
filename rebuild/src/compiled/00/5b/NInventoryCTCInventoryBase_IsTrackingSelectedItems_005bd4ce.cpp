// NInventory::CTCInventoryBase::IsTrackingSelectedItems() const -- the CInventoryDef's TrackSelectedItems flag, read through an add-ref'd
// copy of the def pointer
// Leaf: ?IsTrackingSelectedItems@CTCInventoryBase@NInventory@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD4CE, thiscall, plain `ret`; size TU.
// Identity (NF95, 2026-11-19, queue q11235): Ego_r.pdb 0x00620377 + FableWin.pdb 0x0236A8C7 ?IsTrackingSelectedItems@CTCInventoryBase@NInventory@@QBE_NXZ
//   (public const; the PASS22 complete-methods block of engine/NInventory_CTCInventoryBase.h declares it); the body reads CInventoryDef
//   +0x58 TrackSelectedItems through PDef (+0x68 retail); the sole caller, the landed CCategory::SelectHighlightedItem 0x005BD9B2 (call at
//   0x005BD9CB), binds this spelling. Was the made-up free `CTCWeapon_IsAnalogueLoading` (catalog _global::CIVCountedPointer_Assign_005bd4ce)
//   over row-local models; now on the shared headers.
// Shape: CTCInventoryBase::GetPDef() is inline in retail (a CDefPointer copy: add-ref when non-null), the byte read, then the copy's inline
//   release (dec; Delete on zero).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_PDEF
#define FABLE_RETAIL_CTCINVENTORYBASE_GETPDEF_INLINE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#include "engine/CInventoryDef.h"
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
bool CTCInventoryBase::IsTrackingSelectedItems() const
{
    return GetPDef()->TrackSelectedItems;
}
}
