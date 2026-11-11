// NParticleEngine::CPSCEmitterGeneric::GetComponentType() const -- returns (EPSCType)3
// Leaf: ?GetComponentType@CPSCEmitterGeneric@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B15AB0, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCEmitterGeneric's vtable 0x012A002C (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCEmitterGeneric or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCEmitterGeneric@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCEmitterGeneric {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCEmitterGeneric::GetComponentType() const
{
    return (EPSCType)3;
}
}
