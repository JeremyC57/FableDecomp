// NParticleEngine::CPSCEmitterGeneric::GetComponentSubType() const -- returns (EPSCSubType)4
// Leaf: ?GetComponentSubType@CPSCEmitterGeneric@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B15AC0, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCEmitterGeneric's vtable 0x012A002C (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCEmitterGeneric or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCEmitterGeneric@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCEmitterGeneric {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCEmitterGeneric::GetComponentSubType() const
{
    return (EPSCSubType)4;
}
}
