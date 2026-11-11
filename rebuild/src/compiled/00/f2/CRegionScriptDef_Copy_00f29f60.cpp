// CRegionScriptDef::Copy(const CDefClassBase* def) -- def-system clone hook: assign the source region script def over this one
// Leaf: ?Copy@CRegionScriptDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29F60, 5 bytes (`jmp 0x00F29F70`), thiscall, vtable 0x012FB630 slot 19 (cell 0x012FB67C).
// Identity: CDefClassBase slot 19 = Copy (engine/CDefClassBase.h); FableWin.pdb ?Copy@CRegionScriptDef@@UAEXPBVCDefClassBase@@@Z
//   (0x00F9DD40). The whole body is the tail jump into the class's copy assignment 0x00F29F70 (CParentDefClassBase::operator=
//   0x00431ED0, then the four longs +0x3C..+0x48), which retail kept out of line right behind it.
// Extent: the catalog row ran on through the int3 padding into that operator= (0x00F29F70, 45 bytes); see SPLITS.tsv.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CRegionScriptDef.h"

void CRegionScriptDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CRegionScriptDef*>(def);
}