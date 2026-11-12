// NParticleEngine::CPSCLight::GetComponentSubType() const -- returns (EPSCSubType)5
// Leaf: ?GetComponentSubType@CPSCLight@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1AE30, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCLight's vtable 0x012A0624 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCLight or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCLight@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCLight {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCLight::GetComponentSubType() const
{
    return (EPSCSubType)5;
}
}
