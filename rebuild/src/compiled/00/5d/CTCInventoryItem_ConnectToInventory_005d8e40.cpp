// CTCInventoryItem::ConnectToInventory(NInventory::CTCInventoryBase& inventory) -- remember the inventory the item belongs to (PInventory +0x10)
// Leaf: ?ConnectToInventory@CTCInventoryItem@@QAEXAAVCTCInventoryBase@NInventory@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D8E40, 16 bytes with padding: `mov eax,[esp+4] ; mov [ecx+10h],eax ; ret 4`.
// Name-fix 2026-11-12 (was the stand-in free __fastcall AddExclusiveObserver(CObservable*, void*, CObserver*); catalog
//   _global::SetMember_dword0x10_005d8e40, manifest NUISystem::CObservable::AddExclusiveObserver): Ego_r.pdb 00559da0 + FableWin.pdb
//   01e00730 ?ConnectToInventory@CTCInventoryItem@@QAEXAAVCTCInventoryBase@NInventory@@@Z (Ego_r donor directly before
//   ReturnToInventory, as retail 0x005D8E40 / 0x005D8E50); Ego_r.pdb CTCInventoryItem.PInventory is at +16 (0x10). Declared in
//   engine/CTCInventoryItem.h (FABLE_RETAIL_CTCINVENTORYITEM_CONNECTTOINVENTORY), and the landed caller
//   NInventory::CTCInventoryBase::CreateInventoryObject 0x005BE8F3 (passes `*this`) binds this address under that symbol.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CONNECTTOINVENTORY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryItem.h"

void CTCInventoryItem::ConnectToInventory(NInventory::CTCInventoryBase& inventory)
{
    PInventory = &inventory;
}
