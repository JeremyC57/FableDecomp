// NParticleEngine::CPSCRenderSprite::GetComponentSubType() const -- returns (EPSCSubType)1
// Leaf: ?GetComponentSubType@CPSCRenderSprite@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFE470, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCRenderSprite's vtable 0x0129F67C (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCRenderSprite or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCRenderSprite@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCSubType { }; }
namespace NParticleEngine {
class CPSCRenderSprite {
public:
    virtual NParticleEngine::EPSCSubType GetComponentSubType() const;
};
NParticleEngine::EPSCSubType CPSCRenderSprite::GetComponentSubType() const
{
    return (EPSCSubType)1;
}
}
