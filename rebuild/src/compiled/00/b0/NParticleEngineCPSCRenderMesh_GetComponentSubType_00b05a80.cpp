// NParticleEngine::CPSCRenderMesh::GetComponentSubType() const -- returns (EPSCSubType)0
// Leaf: ?GetComponentSubType@CPSCRenderMesh@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05A80, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCRenderMesh's vtable 0x0129F96C (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCRenderMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCRenderMesh@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCRenderMesh {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCRenderMesh::GetComponentSubType() const
{
    return (EPSCSubType)0;
}
}
