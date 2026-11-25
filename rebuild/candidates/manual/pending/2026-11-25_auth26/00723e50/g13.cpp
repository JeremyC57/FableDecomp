// CTCPhysicsStandard::SetRHSetAsFlushOnNormal; original xy/right_2d/forward locals and source-line sequence.
// Leaf: ?SetRHSetAsFlushOnNormal@CTCPhysicsStandard@@UAEXABVC3DVector@@@Z
// Flags: (none)
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
// Inferred: retail subtracts the exact single-precision quarter-turn at 0x00723E6D.
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLUSHNORMAL_PASS26
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN
#define FABLE_RETAIL_C3DVECTOR_ANGLE_FORMALS_MAGNITUDE_PASS26
#include "engine/CTCPhysicsStandard.h"
#ifdef _MSC_VER
#pragma intrinsic(sin,cos)
#endif
void CTCPhysicsStandard::SetRHSetAsFlushOnNormal(const C3DVector& normal)
{
    UpdateOldRHSet();
    float xy = GetFacingAngleXY();
    C3DVector right_2d;
    right_2d.InitialiseFromAngles((xy - 0.25f), 0, 1);
    C3DVector forward = right_2d ^ normal;
    forward.Normalise();
    RHSet.SetForward(forward);
    RHSet.SetUp(normal);
}
