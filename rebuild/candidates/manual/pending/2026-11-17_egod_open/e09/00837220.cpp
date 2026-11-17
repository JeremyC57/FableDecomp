// CThingTrackNode::UpdateFacingDirection() -- face the node along its track: the direction to the next node (or on from the previous one), turned by Rotate(0, 0.25) and normalised when non-zero; side = (Y, -X, 0) normalised, up = side x direction normalised, and the physics TC's right-handed set becomes (up, direction)
// Grade: DIFFER(716v716) -- two scheduling residues left (the draft's third, the PhysicsTC vtable hoist, closed by CThing::GetPhysicsTC()): the third Normalise's `mov edx,17Ch` one tuple late (retail 0x008373EC before `fmul [0x0122DEDC]`), and the cross product's third component operand order (retail 0x00837419 `fld [esp+18h]` forward.Y then `fmul [esp+2Ch]` right.X; ours the reverse).
// FITTED: GetNextNode() / GetPreviousNode() / GetPhysicsTC() accessors (FableWin-emitted inline accessors; the GetNextNode result is kept in ebx across the GetPos call as retail 0x00837227 does, a raw NextNode read is reloaded).
// Leaf: ?UpdateFacingDirection@CThingTrackNode@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00837220, 716 bytes, thiscall, `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateFacingDirection@CThingTrackNode@@QAEXXZ (the catalog's matched name).
// Callees: CThing::GetPos() const 0x006A4D00 (FableWin ?GetPos@CThing@@QBEABVC3DVector@@XZ), C3DVector::Rotate(float, float)
//   0x00A14260 (landed); CTCPhysicsBase::SetRHSet slot 75 (+0x12c). Expanded: IsNonZero (Z, Y, X; DELTA_NOTIONAL_ZERO 0x0129BA3C
//   squared at run time), three Normalise (GFFastOneOverSqrt: table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C), the cross
//   product, the CRightHandedSet(up, forward) ctor.
// Layout: engine/CThingTrackNode.h (NextNode +0xd0, PreviousNode +0xd4), CThing::PhysicsTC +0x60.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CTHINGTRACKNODE_UPDATEFACING
#define FABLE_RETAIL_CTHINGTRACKNODE_PEEK_NEXT
#define FABLE_RETAIL_CTHINGTRACKNODE_GET_NODES
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingTrackNode.h"

void CThingTrackNode::UpdateFacingDirection()
{
    C3DVector forward(0.0f, 0.0f, 0.0f);
    if (GetNextNode())
        forward = GetNextNode()->GetPos() - GetPos();
    else if (GetPreviousNode())
        forward = GetPos() - GetPreviousNode()->GetPos();
    if (forward.IsNonZero())
    {
        forward.Rotate(0.0f, 0.25f);
        forward.Normalise();
        C3DVector right(forward.Y, -forward.X, 0.0f);
        right.Normalise();
        C3DVector up = right ^ forward;
        up.Normalise();
        GetPhysicsTC()->SetRHSet(CRightHandedSet(up, forward));
    }
}
