// CCarryableDef::Copy(const CDefClassBase* other) -- copy every member of another CCarryableDef: `*this = *(const CCarryableDef*)other;`
// Leaf: ?Copy@CCarryableDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DFEB3, 5 bytes: a thiscall tail call, `jmp` to CCarryableDef::operator= 0x004DFEB8.
// Identity (NF425, 2026-11-25): self ?Copy@CCarryableDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CCarryableDef@@QAEAAV0@ABV0@@Z: bound at 0x004DFEB8 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CCarryableDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCarryableDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CCarryableDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CCarryableDef& operator=(const CCarryableDef& other);
};

void CCarryableDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCarryableDef*)other;
}
