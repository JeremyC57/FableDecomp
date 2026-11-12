// NParticleEngine::CPSCRenderMesh::GetComponentType() const -- returns (EPSCType)1
// Leaf: ?GetComponentType@CPSCRenderMesh@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05A70, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCRenderMesh's vtable 0x0129F96C (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCRenderMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCRenderMesh@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCRenderMesh {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCRenderMesh::GetComponentType() const
{
    return (EPSCType)1;
}
}
