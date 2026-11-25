// CFlammableDef::Copy(const CDefClassBase* other) -- copy every member of another CFlammableDef: `*this = *(const CFlammableDef*)other;`
// Leaf: ?Copy@CFlammableDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5834, 5 bytes: a thiscall tail call, `jmp` to CFlammableDef::operator= 0x004E5839.
// Identity (NF425, 2026-11-25): self ?Copy@CFlammableDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CFlammableDef@@QAEAAV0@ABV0@@Z: bound at 0x004E5839 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CFlammableDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CFlammableDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CFlammableDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CFlammableDef& operator=(const CFlammableDef& other);
};

void CFlammableDef::Copy(const CDefClassBase* other)
{
    *this = *(const CFlammableDef*)other;
}
