// NParticleEngine::CPSCDecalRenderer::GetComponentType() const -- returns (EPSCType)2
// Leaf: ?GetComponentType@CPSCDecalRenderer@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1FF30, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCDecalRenderer's vtable 0x012A09BC (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCDecalRenderer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCDecalRenderer@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCDecalRenderer {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCDecalRenderer::GetComponentType() const
{
    return (EPSCType)2;
}
}
