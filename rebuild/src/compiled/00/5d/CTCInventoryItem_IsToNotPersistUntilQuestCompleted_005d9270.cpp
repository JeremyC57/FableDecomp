// CTCInventoryItem::IsToNotPersistUntilQuestCompleted() const -- the CInventoryItemDef's DoNotPersistUntilQuestCompleted flag, read through an
// add-ref'd copy of the def pointer
// Leaf: ?IsToNotPersistUntilQuestCompleted@CTCInventoryItem@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D9270, thiscall, plain `ret`.
// Identity (NF95, 2026-11-19, queue q11231): Ego_r.pdb 0x0055A1C0 + FableWin.pdb 0x01E01220 ?IsToNotPersistUntilQuestCompleted@CTCInventoryItem@@QBE_NXZ;
//   retail 0x005D9210 / 30 / 50 / 70 are the Ego_r-ordered IsSellable / IsBuyable / IsConfiscatable / IsToNotPersistUntilQuestCompleted run,
//   each reading its CInventoryItemDef byte +0x5C..+0x5F (+0x5F = DoNotPersistUntilQuestCompleted); the sole caller, the landed
//   NInventory::CItem::UpdateItemStats 0x005BEDCB (call at 0x005BEFB6), binds this spelling. Was the made-up free
//   `CTCWeapon_IsAnalogueLoading` (catalog CTCWeapon::IsAnalogueLoading) over row-local models; now on engine/CTCInventoryItem.h +
//   engine/CInventoryItemDef.h.
// Shape: the landed GetInventoryCategory 0x005D91F0: a CDefPointer<const CInventoryItemDef> copy of PDef (+0x0C; add-ref when non-null),
//   the byte read, then the copy's inline release (dec; Delete on zero).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_PDEF
#define FABLE_RETAIL_CTCINVENTORYITEM_ISTONOTPERSISTUNTILQUESTCOMPLETED
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#define FABLE_RETAIL_CLOTHING_DEFS
#include "fable_compiler.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CTCInventoryItem.h"

bool CTCInventoryItem::IsToNotPersistUntilQuestCompleted() const
{
    CDefPointer<const CInventoryItemDef> def(PDef);
    return def->DoNotPersistUntilQuestCompleted;
}
