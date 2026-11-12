// NParticleEngine::CPSCSpline::GetComponentType() const -- returns (EPSCType)2
// Leaf: ?GetComponentType@CPSCSpline@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1B450, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCSpline's vtable 0x012A0934 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCSpline or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCSpline@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCSpline {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCSpline::GetComponentType() const
{
    return (EPSCType)2;
}
}
