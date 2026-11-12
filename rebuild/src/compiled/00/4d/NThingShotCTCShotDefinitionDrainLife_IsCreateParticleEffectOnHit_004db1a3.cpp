// NThingShot::CTCShotDefinitionDrainLife::IsCreateParticleEffectOnHit() const -- returns false
// Leaf: ?IsCreateParticleEffectOnHit@CTCShotDefinitionDrainLife@NThingShot@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB1A3, thiscall, `ret`.
// Identity: slot 43 of NThingShot::CTCShotDefinitionDrainLife's vtable 0x012403F4 (defined here: the slot holds this body in 1 vtable(s), all of NThingShot::CTCShotDefinitionDrainLife or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 25 TLC-named slots of that vtable puts IsCreateParticleEffectOnHit(void) at this slot; Ego_r/FableWin name set: ?IsCreateParticleEffectOnHit@CTCShotDefinitionDrainLife@NThingShot@@MBE_NXZ; `ret` matches the signature.
namespace NThingShot {
class CTCShotDefinitionDrainLife {
public:
protected:
    virtual bool IsCreateParticleEffectOnHit() const;
};
bool CTCShotDefinitionDrainLife::IsCreateParticleEffectOnHit() const
{
    return false;
}
}
