// CContextSensitiveItemDef::Copy(const CDefClassBase* other) -- copy every member of another CContextSensitiveItemDef: `*this = *(const CContextSensitiveItemDef*)other;`
// Leaf: ?Copy@CContextSensitiveItemDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453C1B, 5 bytes: a thiscall tail call, `jmp` to CContextSensitiveItemDef::operator= 0x00453C20.
// Identity (NF425, 2026-11-25): self ?Copy@CContextSensitiveItemDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CContextSensitiveItemDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CContextSensitiveItemDef tail-jumps to 0x00453C20 (its next
//   body; owner is the address-named stand-in ??4CBase_00431f10_Derived_00453c20@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body
//   opens with a call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self ?CContextSensitiveItemDef_Copy@@YIXPAX0@Z; now the real virtual member, so the RTTI-placed
//   name is defined.
// Header: engine header engine/CContextSensitiveItemDef.h does not declare Copy / operator= in any landed prelude tried (header debt);
//   a local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CContextSensitiveItemDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CContextSensitiveItemDef& operator=(const CContextSensitiveItemDef& other);
};

void CContextSensitiveItemDef::Copy(const CDefClassBase* other)
{
    *this = *(const CContextSensitiveItemDef*)other;
}
