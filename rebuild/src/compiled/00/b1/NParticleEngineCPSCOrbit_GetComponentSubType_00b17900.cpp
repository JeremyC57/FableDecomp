// NParticleEngine::CPSCOrbit::GetComponentSubType() const -- returns (EPSCSubType)6
// Leaf: ?GetComponentSubType@CPSCOrbit@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B17900, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCOrbit's vtable 0x012A0374 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCOrbit or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCOrbit@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCOrbit {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCOrbit::GetComponentSubType() const
{
    return (EPSCSubType)6;
}
}
