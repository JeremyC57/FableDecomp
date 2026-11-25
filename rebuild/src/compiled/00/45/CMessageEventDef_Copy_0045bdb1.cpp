// CMessageEventDef::Copy(const CDefClassBase* other) -- copy every member of another CMessageEventDef: `*this = *(const CMessageEventDef*)other;`
// Leaf: ?Copy@CMessageEventDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045BDB1, 5 bytes: a thiscall tail call, `jmp` to CMessageEventDef::operator= 0x0045BDB6.
// Identity (NF425, 2026-11-25): self ?Copy@CMessageEventDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CMessageEventDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CMessageEventDef tail-jumps to 0x0045BDB6 (its next body; owner is
//   the address-named stand-in ??4CBase_00431ed0_Derived_0045bdb6@@QAEAAU0@ABU0@@Z); FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CMessageEventDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CMessageEventDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CMessageEventDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CMessageEventDef& operator=(const CMessageEventDef& other);
};

void CMessageEventDef::Copy(const CDefClassBase* other)
{
    *this = *(const CMessageEventDef*)other;
}
