// NParticleEngine::CPSCRenderMesh::Alloc() -- the particle-component factory: a new CPSCRenderMesh (operator new(0x64) 0x00BFEA1A, null-checked,
//   then a tail jump into the out-of-line default constructor 0x00B012E0), returned as its CPSCBase
// Leaf: ?Alloc@CPSCRenderMesh@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7D80, oracle length 24 bytes.
// Retail bytes:  push 0x64 ; call operator new ; test eax,eax ; pop ecx ; je N ; mov ecx,eax ; jmp CPSCRenderMesh::CPSCRenderMesh ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb + FableWin.pdb ?Alloc@CPSCRenderMesh@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall); its retail references are
//   the registry CPSCManager::RegisterPSCs 0x00AF8170 (abs 0x00AF81FF) and DatabaseClone 0x00B05AB0 (call 0x00B05AB4), whose landed rows already
//   bind this spelling here. The constructor is ??0CPSCRenderMesh@NParticleEngine@@QAE@XZ (Ego_r + FableWin), 0x00B012E0 (its only retail
//   reference is this tail jump), whose landed row defines it.
// Naming (NF463; the NF462 r460d / bB2 pattern): was the generated free function `CPSCRenderMesh* CPSCRenderMesh_Alloc()` (cdecl, no arguments:
//   the same code) calling the constructor as the global-namespace ??0CPSCRenderMesh@@QAE@XZ; same bytes.
// Model (ruling r462a): CPSCRenderMesh has no shared header; it is a local class deriving from its RTTI direct base NParticleEngine::CPSCBase
//   (ghidra_out/rtti_bases.tsv: .?AVCPSCRenderMesh@NParticleEngine@@ : CPSCBase@0) on the shared engine/CPSCBase.h, as the bB2 CPSCAttractor rows.
//   Declared: the ctor, Alloc and the ten PDB overrides of CPSCBase's pure virtuals (no vftable is emitted in this TU); own members opaque
//   (sizeof 0x64 = the allocation, Ego_r.pdb 100 bytes). No cast: CPSCBase is at offset 0.
#include "engine/CPSCBase.h"

namespace NParticleEngine {
class CPSCRenderMesh : public CPSCBase {
public:
    CPSCRenderMesh();                                   // 0x00B012E0
    static CPSCBase* FABLE_FASTCALL Alloc();            // 0x00AF7D80
    virtual CPSCBase* DatabaseClone() const;            // 0x00B05AB0
    virtual CCharString GetClassTypeName() const;
    virtual EPSCType GetComponentType() const;
    virtual EPSCSubType GetComponentSubType() const;
    virtual CCharString GetGUIName() const;
    virtual void InterpolateTo(const CPSCBase&,const float);
    virtual void ReadBinary(CDataInputStream&);
    virtual void WriteBinary(CDataOutputStream&) const;
    virtual void DatabaseCopy(const CPSCBase&);
    virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);
private:
    unsigned char _members[0x64 - 0xc];                 // +0x0c own members (opaque)
};
}

namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCRenderMesh::Alloc()
{
    return new CPSCRenderMesh;
}
}
