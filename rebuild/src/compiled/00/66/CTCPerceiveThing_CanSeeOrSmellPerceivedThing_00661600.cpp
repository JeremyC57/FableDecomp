// CTCPerceiveThing::CanSeeOrSmellPerceivedThing() const -- true when the perceived thing is in line of sight or has been smelt
// Leaf: CanSeeOrSmellPerceivedThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661600, length 23 bytes.
// Callers: CThingCreatureBase::IsPickPocketable @ 0x00665E90, CTCWife::FrameUpdate, CTCGuard::FrameUpdate,
// CAIStateGroup_EnemyCreatureBase::GetEnemy, and as a predicate pointer handed to IsAwareOfThingImplementation
// (0x008645E4, 0x00864DDE). The callers fetch the component next to CTCPerceiveThing's GetPerceivedThing (0x007446A0:
// `add ecx,0Ch ; jmp GetPItem`, PerceivedThing at +0xc), and engine/CTCPerceiveThing.h puts LineOfSight at +0x2d and
// Smelt at +0x31. The manifest label "CTCFadeOutAndIn::IsFading" is propagated from the byte-identical shape.
// Shape: `return LineOfSight || Smelt;` -- full-register 0/1 exits, LineOfSight tested first.
#include "engine/CTCPerceiveThing.h"

struct CTCPerceiveThing_Methods : CTCPerceiveThing {
    bool CanSeeOrSmellPerceivedThing() const;
};

bool CTCPerceiveThing_Methods::CanSeeOrSmellPerceivedThing() const
{
    return LineOfSight || Smelt;
}