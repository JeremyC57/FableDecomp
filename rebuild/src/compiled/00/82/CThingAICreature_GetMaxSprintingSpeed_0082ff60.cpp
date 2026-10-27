// CThingAICreature::GetMaxSprintingSpeed(const C3DVector&) const -- AI creatures have no sprint speed: use the running speed
// Leaf: ?GetMaxSprintingSpeed@CThingAICreature@@UBEMABVC3DVector@@@Z
// Retail: Fable.exe @ 0x0082FF60, 16 bytes, thiscall `ret 4`, float in st0.
// Identity (proven): vtable slot 82 (+0x148) of CThingAICreature 0x0127293C; CThingPlayerCreature overrides the same slot
// (0x006A9880). The creature speed getters sit in slots 78..86 of both vtables, and their bodies line up with the Ego_r.pdb
// fields: CThingAICreature slots 79/80/81/83 read +0x1B8/+0x1BC/+0x1C0/+0x1C4 = PDB MaxWalkingSpeed / MaxJoggingSpeed /
// MaxRunningSpeed / MaxFlyingSpeed (PDB +0x1E8.. less 0x30), CThingPlayerCreature slots 79..83 read +0x190..+0x1A0 =
// MaxWalking / Jogging / Running / Sprinting / FlyingSpeed (PDB +0x1B8.. less 0x28), slot 78 and 84..86 read the def's
// MaxPushingSpeed / MaxHoverSpeedSlow / Medium / Fast. Slot 82 is therefore GetMaxSprintingSpeed; Ego_r.pdb / FableWin.pdb
// have ?GetMaxSprintingSpeed@CThingAICreature@@UBEMABVC3DVector@@@Z, and CThingAICreature has no sprint field, so it
// forwards to slot 81, GetMaxRunningSpeed.
// Label correction: catalog "CThingAICreature::vfunc_82".
// Callees: virtual slot 81 (+0x144) GetMaxRunningSpeed.
// Shape: the result goes through a named float local, which keeps retail's `call ; ret 4` (a bare `return f(d);` tail-jumps).
// Layout: engine/CThingAICreature.h models the vptr as a data member, so a virtual call cannot go through it: local
// polymorphic model with 81 placeholder slots ahead of the two speed getters (header debt, NOTES.md).
class C3DVector {
public:
    float X, Y, Z;
};

#define V10(n) virtual void _slot##n##0(); virtual void _slot##n##1(); virtual void _slot##n##2(); virtual void _slot##n##3(); \
    virtual void _slot##n##4(); virtual void _slot##n##5(); virtual void _slot##n##6(); virtual void _slot##n##7(); \
    virtual void _slot##n##8(); virtual void _slot##n##9();

class CThingCreatureBase {
public:
    V10(0) V10(1) V10(2) V10(3) V10(4) V10(5) V10(6) V10(7)                 // slots 0..79
    virtual void _slot80();                                                  // slot 80
    virtual float GetMaxRunningSpeed(const C3DVector& direction) const;     // slot 81 (+0x144)
    virtual float GetMaxSprintingSpeed(const C3DVector& direction) const;   // slot 82 (+0x148)
};

class CThingAICreature : public CThingCreatureBase {
public:
    virtual float GetMaxSprintingSpeed(const C3DVector& direction) const;
};

float CThingAICreature::GetMaxSprintingSpeed(const C3DVector& direction) const
{
    float speed = GetMaxRunningSpeed(direction);
    return speed;
}