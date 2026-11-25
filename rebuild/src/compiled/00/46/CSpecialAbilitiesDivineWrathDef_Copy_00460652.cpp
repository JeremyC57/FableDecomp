// CSpecialAbilitiesDivineWrathDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesDivineWrathDef: `*this = *(const CSpecialAbilitiesDivineWrathDef*)other;`
// Leaf: ?Copy@CSpecialAbilitiesDivineWrathDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00460652, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesDivineWrathDef::operator= 0x00460657.
// Identity (NF425, 2026-11-25): self ?Copy@CSpecialAbilitiesDivineWrathDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin;
//   target ??4CSpecialAbilitiesDivineWrathDef@@QAEAAV0@ABV0@@Z: bound at 0x00460657 (owner Leaf); Ego_r/FableWin name set; its body
//   opens with a call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CSpecialAbilitiesDivineWrathDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CSpecialAbilitiesDivineWrathDef.h does not declare Copy / operator= in any landed prelude tried (header
//   debt); a local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CSpecialAbilitiesDivineWrathDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CSpecialAbilitiesDivineWrathDef& operator=(const CSpecialAbilitiesDivineWrathDef& other);
};

void CSpecialAbilitiesDivineWrathDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesDivineWrathDef*)other;
}
