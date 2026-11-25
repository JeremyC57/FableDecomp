// CEngineLocalDetailLayerDef::Copy(const CDefClassBase* other) -- copy every member of another CEngineLocalDetailLayerDef: `*this = *(const CEngineLocalDetailLayerDef*)other;`
// Leaf: ?Copy@CEngineLocalDetailLayerDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00477C65, 5 bytes: a thiscall tail call, `jmp` to CEngineLocalDetailLayerDef::operator= 0x00477A8A.
// Identity (NF425, 2026-11-25): self ?Copy@CEngineLocalDetailLayerDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin;
//   target ??4CEngineLocalDetailLayerDef@@QAEAAV0@ABV0@@Z: bound at 0x00477A8A (owner Leaf); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CEngineLocalDetailLayerDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CEngineLocalDetailLayerDef.h does not declare Copy / operator= in any landed prelude tried (header
//   debt); a local class over its RTTI direct base CParentDefClassBase engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"

class CEngineLocalDetailLayerDef : public CParentDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CEngineLocalDetailLayerDef& operator=(const CEngineLocalDetailLayerDef& other);
};

void CEngineLocalDetailLayerDef::Copy(const CDefClassBase* other)
{
    *this = *(const CEngineLocalDetailLayerDef*)other;
}
