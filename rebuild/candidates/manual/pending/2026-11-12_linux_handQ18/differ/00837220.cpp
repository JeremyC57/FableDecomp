// CThingTrackNode::UpdateFacingDirection() -- face the node along its track: the direction to the next node (or on from the previous one), turned by Rotate(0, 0.25) and normalised when non-zero; side = (Y, -X, 0) normalised, up = side x direction normalised, and the physics TC's right-handed set becomes (up, direction)
// Grade: DIFFER(ours v retail) -- DIFFER(716v716) -- ported to the shared headers (C3DVector ISNONZERO_ZYX + SQUAREDMAGNITUDE_ZYX, new ROTATE_ANGLES decl, CRightHandedSet CTOR_INLINE, a physics local); three scheduling residues: the third Normalise's `mov edx,17Ch` one tuple late (0x008373EC), the cross product's third component loads side.X before direction.Y (0x00837419; CROSS_XY_GROUP and the swapped spelling compile the same), and the PhysicsTC vtable load `mov eax,[ecx]` hoisted (retail just before the call at 0x008374DD)
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
#define FABLE_RETAIL_C3DVECTOR
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
    C3DVector direction(0.0f, 0.0f, 0.0f);
    CThingTrackNode* next = NextNode;
    if (next)
        direction = next->GetPos() - GetPos();
    else if (PreviousNode)
        direction = GetPos() - PreviousNode->GetPos();
    if (direction.IsNonZero()) {
        direction.Rotate(0.0f, 0.25f);
        direction.Normalise();
        C3DVector side(direction.Y, -direction.X, 0.0f);
        side.Normalise();
        C3DVector up = side ^ direction;
        up.Normalise();
        CTCPhysicsBase* physics = PhysicsTC;
        physics->SetRHSet(CRightHandedSet(up, direction));
    }
}
