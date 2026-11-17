// CTCInventoryMagic::CTCInventoryMagic(CThing& thing) -- build both bases, store both vtables, empty the primitive-handle vector
// Retail 0x0072DB90, 66 bytes (+14 int3), ret 4. Called from the factory 0x004D2F46 (CTCInventoryMagic_Create).
// Shape (as the landed sibling CTCInventoryStats::CTCInventoryStats 0x005CD2A6): CTCInventoryBase(thing)
// 0x005BFF07, then NHeroInformationScreens::CBase::CBase 0x0066E734 on the second base at +0x130 (its
// address kept in edi), then the CTCInventoryMagic vtables 0x012668EC (primary) and 0x012668D8 (at
// +0x130), then InventoryMagicPrimitiveHandles (STLport vector<CEnginePrimitiveHandle>, retail +0x134,
// Ego_r.pdb +0x14C) is emptied from one `xor eax,eax` (an inline member constructor: its stores follow
// the vtable stores). This module is speed-optimised (aligned bodies, the argument loaded into eax
// and pushed rather than a pass-through `push [esp+n]`).
// Leaf: ??0CTCInventoryMagic@@QAE@AAVCThing@@@Z

// NF52 (round 7): moved onto engine/CTCInventoryMagic.h (FABLE_RETAIL_CTCINVENTORYMAGIC_PASS22: NInventory::CTCInventoryBase +
// NHeroInformationScreens::CBase, the STLport vector<CEnginePrimitiveHandle>), so the two vtable stores bind the class's real vftables
// ??_7CTCInventoryMagic@@6BCTCInventoryBase@NInventory@@@ (0x012668EC) and ??_7CTCInventoryMagic@@6BCBase@NHeroInformationScreens@@@
// (0x012668D8). The constructor is the PDB's ??0CTCInventoryMagic@@QAE@AAVCThing@@@Z (CThing&; the row spelled CThing*), declared by
// the overlay opt-in FABLE_RETAIL_CTCINVENTORYMAGIC_CTOR; the base constructor is ??0CTCInventoryBase@NInventory@@QAE@AAVCThing@@@Z
// (both PDBs). Were local CTCInventoryBase / NHeroInformationScreens::CBase / CTCInventoryMagic models and a vector stand-in.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_ORIGINAL_MEMBERS_PASS22
#define FABLE_RETAIL_CTCINVENTORYMAGIC_PASS22
#define FABLE_RETAIL_CTCINVENTORYMAGIC_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CDefPointer.h"
#include "engine/CTCInventoryMagic.h"

CTCInventoryMagic::CTCInventoryMagic(CThing& thing)
    : NInventory::CTCInventoryBase(thing)
{
}
