// CExperienceDef::Copy(const CDefClassBase* other) -- copy every member of another CExperienceDef: `*this = *(const CExperienceDef*)other;`
// Leaf: ?Copy@CExperienceDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E4D83, 5 bytes: a thiscall tail call, `jmp` to CExperienceDef::operator= 0x004E4D88.
// Identity (NF425, 2026-11-25): self ?Copy@CExperienceDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CExperienceDef@@QAEAAV0@ABV0@@Z: bound at 0x004E4D88 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CExperienceDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CExperienceDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CExperienceDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CExperienceDef& operator=(const CExperienceDef& other);
};

void CExperienceDef::Copy(const CDefClassBase* other)
{
    *this = *(const CExperienceDef*)other;
}
