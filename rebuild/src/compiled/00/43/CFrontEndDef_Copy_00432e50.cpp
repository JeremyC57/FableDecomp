// CFrontEndDef::Copy(const CDefClassBase* other) -- copy every member of another CFrontEndDef: `*this = *(const CFrontEndDef*)other;`
// Leaf: ?Copy@CFrontEndDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00432E50, 5 bytes: a thiscall tail call, `jmp` to CFrontEndDef::operator= 0x00432E60.
// Identity (NF425, 2026-11-25): self ?Copy@CFrontEndDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CFrontEndDef@@QAEAAV0@ABV0@@Z: bound at 0x00432E60 (owner Leaf); FableWin name set; its body opens with a call of the ancestor
//   operator= ??4CResource (0x00431F60): a derived class's assignment.
// Respell (NF425): was the stand-in self ?Copy@CControlsDef@@QAEXPBV1@@Z; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CFrontEndDef.h does not declare Copy / operator= in any landed prelude tried (header debt); a local
//   class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CFrontEndDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CFrontEndDef& operator=(const CFrontEndDef& other);
};

void CFrontEndDef::Copy(const CDefClassBase* other)
{
    *this = *(const CFrontEndDef*)other;
}
