// NParticleEngine::CPSCSingleSprite::GetComponentType() const -- returns (EPSCType)2
// Leaf: ?GetComponentType@CPSCSingleSprite@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A0F0, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCSingleSprite's vtable 0x0129FB84 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCSingleSprite or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCSingleSprite@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCSingleSprite {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCSingleSprite::GetComponentType() const
{
    return (EPSCType)2;
}
}
