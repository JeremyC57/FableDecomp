// NParticleEngine::CPSCAttractor::GetComponentSubType() const -- returns (EPSCSubType)3
// Leaf: ?GetComponentSubType@CPSCAttractor@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18710, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCAttractor's vtable 0x012A04AC (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCAttractor or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCAttractor@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCAttractor {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCAttractor::GetComponentSubType() const
{
    return (EPSCSubType)3;
}
}
