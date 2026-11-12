// NParticleEngine::CPSCAttractor::GetComponentType() const -- returns (EPSCType)2
// Leaf: ?GetComponentType@CPSCAttractor@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18700, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCAttractor's vtable 0x012A04AC (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCAttractor or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCAttractor@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCAttractor {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCAttractor::GetComponentType() const
{
    return (EPSCType)2;
}
}
