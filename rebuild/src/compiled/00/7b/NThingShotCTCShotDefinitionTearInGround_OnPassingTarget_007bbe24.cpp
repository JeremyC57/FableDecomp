// NThingShot::CTCShotDefinitionTearInGround::OnPassingTarget() -- once the tear-in-ground shot passes its target it dies two seconds later
// Leaf: ?OnPassingTarget@CTCShotDefinitionTearInGround@NThingShot@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BBE24, 16 bytes, `call ; ret` (unaligned start: the shot-definition TU is
//   size-optimised and packs its functions).
// Identity: NThingShot::CTCShotDefinitionTearInGround vtable 0x0123CF9C slot 41. The class's own virtuals in Ego_r.pdb are
//   InitMovement, UpdateMovement and OnPassingTarget (all `MAEXXZ`); the vtable holds InitMovement at slot 33 (0x007BC8FC)
//   and the landed UpdateMovement at slot 35 (0x007BBDCA), so slot 41 is OnPassingTarget.
// Callees: NThingShot::CTCShotDefinitionBase::SetDieInSeconds(float) @ 0x007BED90 (Ego_r.pdb
//   `?SetDieInSeconds@CTCShotDefinitionBase@NThingShot@@IAEXM@Z`, protected; the landed body names it SetTimeToDie: it
//   stores DieOnFrame = world frame + seconds * frames per second, and this is its only caller).
// Constants: 2.0f at 0x0122DCDC.
// Shape: the float argument is loaded from the pool and stored into its pushed slot (`fld ; push ecx ; fstp [esp]`); the
//   callee takes a stack argument, so the forwarder cannot tail-jump. `optimize("s")` as the TU.
#pragma optimize("s", on)
class CThing;

namespace NThingShot {
class CTCShotDefinitionBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
protected:
    virtual void OnPassingTarget();             // slot 41 (+0xa4)
    void SetDieInSeconds(float seconds);        // 0x007BED90
};

class CTCShotDefinitionTearInGround : public CTCShotDefinitionBase {
protected:
    virtual void OnPassingTarget();
};

void CTCShotDefinitionTearInGround::OnPassingTarget()
{
    SetDieInSeconds(2.0f);
}
}