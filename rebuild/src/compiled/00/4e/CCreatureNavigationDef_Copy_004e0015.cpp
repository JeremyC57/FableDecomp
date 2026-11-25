// CCreatureNavigationDef::Copy(const CDefClassBase* other) -- copy every member of another CCreatureNavigationDef: `*this = *(const CCreatureNavigationDef*)other;`
// Leaf: ?Copy@CCreatureNavigationDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0015, 5 bytes: a thiscall tail call, `jmp` to CCreatureNavigationDef::operator= 0x004E001A.
// Identity (NF425, 2026-11-25): self ?Copy@CCreatureNavigationDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CCreatureNavigationDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CCreatureNavigationDef tail-jumps to 0x004E001A (its next body;
//   owner is the address-named stand-in ??4CBase_00431f10_Derived_004e001a@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CCreatureNavigationDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureNavigationDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CCreatureNavigationDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CCreatureNavigationDef& operator=(const CCreatureNavigationDef& other);
};

void CCreatureNavigationDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCreatureNavigationDef*)other;
}
