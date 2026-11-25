// CMultiStaticMeshEntryDef::Copy(const CDefClassBase* other) -- copy every member of another CMultiStaticMeshEntryDef: `*this = *(const CMultiStaticMeshEntryDef*)other;`
// Leaf: ?Copy@CMultiStaticMeshEntryDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EB907, 5 bytes: a thiscall tail call, `jmp` to CMultiStaticMeshEntryDef::operator= 0x004EB831.
// Identity (NF425, 2026-11-25): self ?Copy@CMultiStaticMeshEntryDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CMultiStaticMeshEntryDef@@QAEAAV0@ABV0@@Z: bound at 0x004EB831 (tracker PDB spelling); Ego_r/FableWin name set; its body opens
//   with a call of the ancestor operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CMultiStaticMeshEntryDef_Copy; now the real virtual member, so the RTTI-placed name is
//   defined.
// Header: engine header engine/CMultiStaticMeshEntryDef.h does not declare Copy / operator= in any landed prelude tried (header debt);
//   a local class over its RTTI direct base CSubDefClassBase engine/CSubDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"

class CMultiStaticMeshEntryDef : public CSubDefClassBase
{
public:
    virtual void Copy(const CDefClassBase* other);
    CMultiStaticMeshEntryDef& operator=(const CMultiStaticMeshEntryDef& other);
};

void CMultiStaticMeshEntryDef::Copy(const CDefClassBase* other)
{
    *this = *(const CMultiStaticMeshEntryDef*)other;
}
