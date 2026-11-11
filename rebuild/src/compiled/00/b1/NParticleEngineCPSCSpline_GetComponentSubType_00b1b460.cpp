// NParticleEngine::CPSCSpline::GetComponentSubType() const -- returns (EPSCSubType)8
// Leaf: ?GetComponentSubType@CPSCSpline@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1B460, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCSpline's vtable 0x012A0934 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCSpline or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCSpline@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCSpline {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCSpline::GetComponentSubType() const
{
    return (EPSCSubType)8;
}
}
