// NThingShot::CTCShotDefinitionDrainLife::IsNegatedHit() const -- returns true
// Leaf: ?IsNegatedHit@CTCShotDefinitionDrainLife@NThingShot@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB1A0, thiscall, `ret`.
// Identity: slot 42 of NThingShot::CTCShotDefinitionDrainLife's vtable 0x012403F4 (defined here: the slot holds this body in 1 vtable(s), all of NThingShot::CTCShotDefinitionDrainLife or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 25 TLC-named slots of that vtable puts IsNegatedHit(void) at this slot; Ego_r/FableWin name set: ?IsNegatedHit@CTCShotDefinitionDrainLife@NThingShot@@MBE_NXZ; `ret` matches the signature.
namespace NThingShot {
class CTCShotDefinitionDrainLife {
public:
protected:
    virtual bool IsNegatedHit() const;
};
bool CTCShotDefinitionDrainLife::IsNegatedHit() const
{
    return true;
}
}
