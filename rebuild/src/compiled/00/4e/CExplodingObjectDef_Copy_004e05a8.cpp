// CExplodingObjectDef::Copy(const CDefClassBase* other) -- copy every member of another CExplodingObjectDef: `*this = *(const CExplodingObjectDef*)other;`
// Leaf: ?Copy@CExplodingObjectDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E05A8, 5 bytes: a thiscall tail call, `jmp` to CExplodingObjectDef::operator= 0x004E05AD.
// Identity (NF425, 2026-11-25): self ?Copy@CExplodingObjectDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CExplodingObjectDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CExplodingObjectDef tail-jumps to 0x004E05AD (its next body; owner
//   is the address-named stand-in ??4CBase_00431f10_Derived_004e05ad@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens with a
//   call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CExplodingObjectDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CExplodingObjectDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a
//   local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CExplodingObjectDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CExplodingObjectDef& operator=(const CExplodingObjectDef& other);
};

void CExplodingObjectDef::Copy(const CDefClassBase* other)
{
    *this = *(const CExplodingObjectDef*)other;
}
