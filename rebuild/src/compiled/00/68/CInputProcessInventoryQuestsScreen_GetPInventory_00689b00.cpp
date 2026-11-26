// CInputProcessInventoryQuestsScreen::GetPInventory() const -- the screen's inventory component: PInventory (+0x34), returned as its NInventory::CTCInventoryBase
// Leaf: ?GetPInventory@CInputProcessInventoryQuestsScreen@@MBEPAVCTCInventoryBase@NInventory@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00689B00, 4 bytes: `mov eax,[ecx+34h] ; ret`. CInputProcessInventoryQuestsScreen vftable slot 9 (RTTI; NF477 audit: one `.rdata`
// dword, no call); the slot is CInputProcessInventoryBase's pure virtual GetPInventory.
// Identity: Ego_r.pdb + FableWin.pdb ?GetPInventory@CInputProcessInventoryQuestsScreen@@MBEPAVCTCInventoryBase@NInventory@@XZ (protected virtual const).
// Layout: PInventory CTCInventoryQuests* +0x34 (engine/CInputProcessInventoryQuestsScreen.h, Ego_r.pdb); CTCInventoryQuests derives NInventory::CTCInventoryBase at +0 (ghidra_out/rtti_bases.tsv), so
//   the upcast adds nothing (engine/CTCInventoryQuests.h).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the stand-in ?M@C_00689b00@@QAEKXZ (free `__fastcall` on an untyped
//   process / a placeholder class); respelt onto engine/CInputProcessInventoryQuestsScreen.h FABLE_RETAIL_INPUTPROCESS_VTABLE (resolves T3 nf324 "the body uses process
//   typed `const unsigned char*`"). Bytes unchanged.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CTCInventoryQuests.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessInventoryQuestsScreen.h"

NInventory::CTCInventoryBase* CInputProcessInventoryQuestsScreen::GetPInventory() const
{
    return PInventory;
}
