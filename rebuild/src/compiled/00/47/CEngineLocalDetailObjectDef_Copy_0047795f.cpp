// CEngineLocalDetailObjectDef::Copy(const CDefClassBase* other) -- copy every member of another CEngineLocalDetailObjectDef: `*this = *(const CEngineLocalDetailObjectDef*)other;`
// Leaf: ?Copy@CEngineLocalDetailObjectDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047795F, 5 bytes: a thiscall tail call, `jmp` to CEngineLocalDetailObjectDef::operator= 0x00477964.
// Identity (NF425, 2026-11-25): self ?Copy@CEngineLocalDetailObjectDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin;
//   target ??4CEngineLocalDetailObjectDef@@QAEAAV0@ABV0@@Z: bound at 0x00477964 (tracker PDB spelling); Ego_r/FableWin name set; its
//   body opens with a call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CEngineLocalDetailObjectDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CEngineLocalDetailObjectDef.h does not declare Copy / operator= in any landed prelude tried (header
//   debt); a local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CEngineLocalDetailObjectDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CEngineLocalDetailObjectDef& operator=(const CEngineLocalDetailObjectDef& other);
};

void CEngineLocalDetailObjectDef::Copy(const CDefClassBase* other)
{
    *this = *(const CEngineLocalDetailObjectDef*)other;
}
