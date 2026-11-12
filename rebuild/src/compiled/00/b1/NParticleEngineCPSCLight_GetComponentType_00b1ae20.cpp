// NParticleEngine::CPSCLight::GetComponentType() const -- returns (EPSCType)2
// Leaf: ?GetComponentType@CPSCLight@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1AE20, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCLight's vtable 0x012A0624 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCLight or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCLight@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCLight {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCLight::GetComponentType() const
{
    return (EPSCType)2;
}
}
