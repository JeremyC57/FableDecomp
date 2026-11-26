// CInputProcessInventoryStatsScreen::GetPInventory() const -- the screen's inventory component: PInventory (+0x34), returned as its NInventory::CTCInventoryBase
// Leaf: ?GetPInventory@CInputProcessInventoryStatsScreen@@MBEPAVCTCInventoryBase@NInventory@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00689C30, 4 bytes: `mov eax,[ecx+34h] ; ret`. CInputProcessInventoryStatsScreen vftable slot 9 (RTTI; NF477 audit: one `.rdata`
// dword, no call); the slot is CInputProcessInventoryBase's pure virtual GetPInventory.
// Identity: Ego_r.pdb + FableWin.pdb ?GetPInventory@CInputProcessInventoryStatsScreen@@MBEPAVCTCInventoryBase@NInventory@@XZ (protected virtual const).
// Layout: PInventory CTCInventoryStats* +0x34 (engine/CInputProcessInventoryStatsScreen.h, Ego_r.pdb); CTCInventoryStats derives NInventory::CTCInventoryBase at +0 (ghidra_out/rtti_bases.tsv), so
//   the upcast adds nothing (engine/CTCInventoryStats.h).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the stand-in ?CInputProcessInventoryStatsScreen_GetPInventory@@YIHPBE@Z (free `__fastcall` on an untyped
//   process / a placeholder class); respelt onto engine/CInputProcessInventoryStatsScreen.h FABLE_RETAIL_INPUTPROCESS_VTABLE (resolves T3 nf324 "the body uses process
//   typed `const unsigned char*`"). Bytes unchanged.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYSTATS
#include "engine/CTCInventoryStats.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessInventoryStatsScreen.h"

NInventory::CTCInventoryBase* CInputProcessInventoryStatsScreen::GetPInventory() const
{
    return PInventory;
}
