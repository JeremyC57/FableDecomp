// NParticleEngine::CPSCUpdateNormal::GetComponentType() const -- returns (EPSCType)4
// Leaf: ?GetComponentType@CPSCUpdateNormal@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0F8A0, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCUpdateNormal's vtable 0x0129FC1C (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCUpdateNormal or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 20 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCUpdateNormal@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCUpdateNormal {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCUpdateNormal::GetComponentType() const
{
    return (EPSCType)4;
}
}
