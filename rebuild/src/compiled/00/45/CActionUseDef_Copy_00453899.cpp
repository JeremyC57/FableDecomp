// CActionUseDef::Copy(const CDefClassBase* other) -- copy every member of another CActionUseDef: `*this = *(const CActionUseDef*)other;`
// Leaf: ?Copy@CActionUseDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453899, 5 bytes: a thiscall tail call, `jmp` to CActionUseDef::operator= 0x0045389E.
// Identity (NF425, 2026-11-25): self ?Copy@CActionUseDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CActionUseDef@@QAEAAV0@ABV0@@Z: the RTTI-proven Copy of CActionUseDef tail-jumps to 0x0045389E (its next body; owner is the
//   address-named stand-in ??4CBase_00431f10_Derived_0045389e@@QAEAAU0@ABU0@@Z); Ego_r/FableWin name set; its body opens with a call of
//   the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self ?CActionUseDef_Copy@@YIXPAVCActionUseDef@@PAXPBV1@@Z; now the real virtual member, so the
//   RTTI-placed name is defined.
// Header: engine header engine/CActionUseDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CActionUseDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CActionUseDef& operator=(const CActionUseDef& other);
};

void CActionUseDef::Copy(const CDefClassBase* other)
{
    *this = *(const CActionUseDef*)other;
}
