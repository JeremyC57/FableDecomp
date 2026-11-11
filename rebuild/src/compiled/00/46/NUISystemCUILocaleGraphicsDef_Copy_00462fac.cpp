// NUISystem::CUILocaleGraphicsDef::Copy(const CDefClassBase* def) -- def-system clone hook: assign the source locale-graphics def over this one
// Leaf: ?Copy@CUILocaleGraphicsDef@NUISystem@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00462FAC, 5 bytes (`jmp 0x00462FB1`), thiscall, vtable 0x01235344 slot 19 (cell 0x01235390).
// Identity: CDefClassBase slot 19 = Copy (engine/CDefClassBase.h); Ego_r.pdb ?Copy@CUILocaleGraphicsDef@NUISystem@@UAEXPBVCDefClassBase@@@Z
//   (0x0047CCC5) and FableWin.pdb (0x01926C96). The whole body is the tail jump into the class's copy assignment
//   (??4CUILocaleGraphicsDef@NUISystem@@QAEAAV01@ABV01@@Z, both PDBs), which retail kept out of line right behind it at 0x00462FB1.
// Extent: the catalog row ran on into that operator= (0x00462FB1, 51 bytes); see SPLITS.tsv.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#include "engine/NUISystem_CUILocaleGraphicsDef.h"

void NUISystem::CUILocaleGraphicsDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CUILocaleGraphicsDef*>(def);
}