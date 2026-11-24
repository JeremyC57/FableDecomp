// Leaf: ?Copy@CMultiStaticMeshDef@@UAEXPBVCDefClassBase@@@Z
// CMultiStaticMeshDef::Copy(const CDefClassBase* other) -- copy another CMultiStaticMeshDef: the CDefClassBase part through its out-of-line operator=, then the Meshes vector
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EB6C8, thiscall, `ret 4`: `call 0x00431F10` (CDefClassBase::operator=, this = ecx, other pushed), then
//   `lea ecx,[esi+28h] ; push other+28h ; call 0x004EB6E7` (vector<CMultiStaticMeshEntryDef>::operator=, out of line); the derived assignment is inline.
// Identity: the RTTI vftable placement of 0x004EB6C8 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets; the member
//   at +0x28 is Ego_r.pdb CMultiStaticMeshDef::Meshes, a vector<CMultiStaticMeshEntryDef> (rebuild/include/engine/RECONCILE.tsv; STLport 4.6: 0xC bytes).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, ??4CDefClassBase@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; engine/CDefClassBase.h);
//   vector<CMultiStaticMeshEntryDef>::operator= 0x004EB6E7, ??4?$vector@VCMultiStaticMeshEntryDef@@V?$allocator@VCMultiStaticMeshEntryDef@@@std@@@std@@QAEAAV01@ABV01@@Z
//   (the landed row 0x004EB6E7's own symbol).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in (bare name `Copy`, made-up callee SubContainer::Copy) onto the PDB member.
//   No engine/CMultiStaticMeshDef.h (only _quarantine/): the class is a local model on the shared CSubDefClassBase; the vector is the shared
//   STLport model (stlport/fable_containers.h, FABLE_RETAIL_STLP_VECTOR_ASSIGN: operator= declared out of line, its one definition is 0x004EB6E7).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"

class CMultiStaticMeshEntryDef;

class CMultiStaticMeshDef : public CSubDefClassBase {                  // no engine header (RTTI .?AVCMultiStaticMeshDef@@ : CSubDefClassBase)
public:
    virtual void Copy(const CDefClassBase* other);                      // slot 19 0x004EB6C8
    std::vector<CMultiStaticMeshEntryDef> Meshes;                       // +0x28
};

void CMultiStaticMeshDef::Copy(const CDefClassBase* other)
{
    CDefClassBase::operator=(*other);
    Meshes = ((const CMultiStaticMeshDef*)other)->Meshes;
}
