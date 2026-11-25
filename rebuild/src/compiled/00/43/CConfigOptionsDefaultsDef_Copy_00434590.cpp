// CConfigOptionsDefaultsDef::Copy(const CDefClassBase* other) -- copy every member of another CConfigOptionsDefaultsDef: `*this = *(const CConfigOptionsDefaultsDef*)other;`
// Leaf: ?Copy@CConfigOptionsDefaultsDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00434590, 5 bytes: a thiscall tail call, `jmp` to CConfigOptionsDefaultsDef::operator= 0x004345A0.
// Identity (NF425, 2026-11-25): self ?Copy@CConfigOptionsDefaultsDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CConfigOptionsDefaultsDef@@QAEAAV0@ABV0@@Z: bound at 0x004345A0 (owner Leaf); Ego_r/FableWin name set; its body opens with a
//   call of the ancestor operator= ??4CResource (0x00431F60): a derived class's assignment.
// Respell (NF425): was the stand-in self ?Copy@CControlsDef@@QAEXPBV1@@Z; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CConfigOptionsDefaultsDef.h does not declare Copy / operator= in any landed prelude tried (header debt);
//   a local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CConfigOptionsDefaultsDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CConfigOptionsDefaultsDef& operator=(const CConfigOptionsDefaultsDef& other);
};

void CConfigOptionsDefaultsDef::Copy(const CDefClassBase* other)
{
    *this = *(const CConfigOptionsDefaultsDef*)other;
}
