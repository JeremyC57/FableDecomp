// NParticleEngine::CPSCUpdateNormal::GetComponentSubType() const -- returns (EPSCSubType)0xa
// Leaf: ?GetComponentSubType@CPSCUpdateNormal@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0F8B0, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCUpdateNormal's vtable 0x0129FC1C (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCUpdateNormal or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 20 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCUpdateNormal@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCUpdateNormal {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCUpdateNormal::GetComponentSubType() const
{
    return (EPSCSubType)0xa;
}
}
