// NParticleEngine::CPSCOrbit::GetComponentType() const -- returns (EPSCType)2
// Leaf: ?GetComponentType@CPSCOrbit@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B178F0, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCOrbit's vtable 0x012A0374 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCOrbit or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCOrbit@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCOrbit {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCOrbit::GetComponentType() const
{
    return (EPSCType)2;
}
}
