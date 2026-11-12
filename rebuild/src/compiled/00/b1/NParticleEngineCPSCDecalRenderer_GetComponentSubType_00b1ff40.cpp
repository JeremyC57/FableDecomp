// NParticleEngine::CPSCDecalRenderer::GetComponentSubType() const -- returns (EPSCSubType)9
// Leaf: ?GetComponentSubType@CPSCDecalRenderer@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1FF40, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCDecalRenderer's vtable 0x012A09BC (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCDecalRenderer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCDecalRenderer@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCDecalRenderer {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCDecalRenderer::GetComponentSubType() const
{
    return (EPSCSubType)9;
}
}
