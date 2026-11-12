// CCreatureAction_MidAttackSide::Initialise() -- pick the sideways strike animation: "STRIKE_MEDIUM_RIGHT_90" when the normalised direction to the target lies on the right of the creature's facing (positive dot with the facing's right-hand perpendicular), else "STRIKE_MEDIUM_LEFT_90"; then the base Initialise
// Grade: DIFFER(342v344) -- on the shared headers the header fast Normalise fixes the parked `fld st(1) ; fmul [Y]` product, and the new FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ dot fixes the product order and the dead-value pops; one instruction left: retail `fxch st(2)` at 0x008B6BBC brings the normalised Y to the top before `fmul st(2)` (so -facing.X is the dead value popped at the end), ours multiplies with -facing.X on top (swapped caller / product operands, a named perpendicular and the X,Y,Z / default dots all tried)
// Leaf: ?Initialise@CCreatureAction_MidAttackSide@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B6AD0, 344 bytes (+ int3 padding), thiscall, plain `ret` (vtable 0x0127AB6C).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0xa8 first, then PCreature +0x08), CThing::GetPos 0x004C73D0,
//   CTCPhysicsBase::GetFacingVector() const (slot 70, +0x118, hidden result), CCharString 0x0099EBF0 / 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation 0x008574D0, CCombatActionBase::Initialise 0x00857520; the fast normalise's table 0x013CE558.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_YXZ
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_MidAttackSide.h"

void CCreatureAction_MidAttackSide::Initialise()
{
    CThing* target = PTarget.Get();
    C3DVector toTarget = target->GetPos() - PCreature->GetPos();
    C3DVector facing = PCreature->GetPhysicsTC()->GetFacingVector();
    toTarget.Normalise();
    if (toTarget.Dot(C3DVector(facing.Y, -facing.X, 0.0f)) > 0.0f)
        SetCombatAnimation("STRIKE_MEDIUM_RIGHT_90");
    else
        SetCombatAnimation("STRIKE_MEDIUM_LEFT_90");
    CCombatActionBase::Initialise();
}
