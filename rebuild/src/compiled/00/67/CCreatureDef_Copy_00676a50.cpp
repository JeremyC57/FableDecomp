// CCreatureDef::Copy(const CDefClassBase* other) -- copy every member of another CCreatureDef: `*this = *(const CCreatureDef*)other;`
// Leaf: ?Copy@CCreatureDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00676A50, 5 bytes: a thiscall tail call, `jmp` to CCreatureDef::operator= 0x00675C00.
// Identity (NF425, 2026-11-25): self ?Copy@CCreatureDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CCreatureDef@@QAEAAV0@ABV0@@Z: bound at 0x00675C00 (owner Leaf).
// Respell (NF425): was the stand-in self _CCreatureDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: engine header engine/CCreatureDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CCreatureDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CCreatureDef& operator=(const CCreatureDef& other);
};

void CCreatureDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCreatureDef*)other;
}
