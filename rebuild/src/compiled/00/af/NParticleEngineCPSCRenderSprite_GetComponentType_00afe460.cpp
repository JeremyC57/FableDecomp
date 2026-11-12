// NParticleEngine::CPSCRenderSprite::GetComponentType() const -- returns (EPSCType)1
// Leaf: ?GetComponentType@CPSCRenderSprite@NParticleEngine@@UBE?AW4EPSCType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFE460, thiscall, `ret`.
// Identity: slot 4 of NParticleEngine::CPSCRenderSprite's vtable 0x0129F67C (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCRenderSprite or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetComponentType(void) at this slot; Ego_r/FableWin name set: ?GetComponentType@CPSCRenderSprite@NParticleEngine@@UBE?AW4EPSCType@2@XZ; `ret` matches the signature.
namespace NParticleEngine { enum EPSCType { }; }
namespace NParticleEngine {
class CPSCRenderSprite {
public:
    virtual NParticleEngine::EPSCType GetComponentType() const;
};
NParticleEngine::EPSCType CPSCRenderSprite::GetComponentType() const
{
    return (EPSCType)1;
}
}
