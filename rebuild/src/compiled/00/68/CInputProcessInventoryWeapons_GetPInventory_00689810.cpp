// CInputProcessInventoryWeapons::GetPInventory() const -- the screen's inventory component: PInventory (+0x34), returned as its NInventory::CTCInventoryBase
// Leaf: ?GetPInventory@CInputProcessInventoryWeapons@@MBEPAVCTCInventoryBase@NInventory@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00689810, 4 bytes: `mov eax,[ecx+34h] ; ret`. CInputProcessInventoryWeapons vftable slot 9 (RTTI; NF477 audit: one `.rdata`
// dword, no call); the slot is CInputProcessInventoryBase's pure virtual GetPInventory.
// Identity: Ego_r.pdb + FableWin.pdb ?GetPInventory@CInputProcessInventoryWeapons@@MBEPAVCTCInventoryBase@NInventory@@XZ (protected virtual const).
// Layout: PInventory CTCInventoryWeapons* +0x34 (engine/CInputProcessInventoryWeapons.h, Ego_r.pdb); CTCInventoryWeapons derives NInventory::CTCInventoryBase at +0 (ghidra_out/rtti_bases.tsv), so
//   the upcast adds nothing (engine/CTCInventoryWeapons.h).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the stand-in ?CInputProcessInventoryWeapons_GetPInventory@@YIHPBE@Z (free `__fastcall` on an untyped
//   process / a placeholder class); respelt onto engine/CInputProcessInventoryWeapons.h FABLE_RETAIL_INPUTPROCESS_VTABLE (resolves T3 nf324 "the body uses process
//   typed `const unsigned char*`"). Bytes unchanged.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CTCInventoryWeapons.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessInventoryWeapons.h"

NInventory::CTCInventoryBase* CInputProcessInventoryWeapons::GetPInventory() const
{
    return PInventory;
}
