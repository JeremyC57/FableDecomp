// CPhysicsMesh::HasCollidedWithTriangleAt -- Sample a swept sphere against a front-facing mesh triangle.
// Leaf: ?HasCollidedWithTriangleAt@CPhysicsMesh@@ABE_NABVC3DTriangle2@@MABVC3DVector@@1MAAV3@2@Z
// Flags: /O2 /Oy /W3
// The shared CPhysicsMesh header owns this inline implementation.
// Both sphere-intersection callers also emit the original out-of-line member.
#define FABLE_RETAIL_CPHYSICSMESH_FULL_SPHERE_PASS30
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
#define FABLE_RETAIL_CPHYSICSMESH_SWEEP_TRIANGLE_PASS30
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF

#define FABLE_RETAIL_C3DVECTOR_CROSS_OPERATOR_PASS29
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
#include "engine/CPhysicsMesh.h"
#include "engine/CSphere.h"

// Fidelity: scaffolding, not retail. Emit the shared private member in its
// standalone object while preserving the original access and calling convention.
namespace {
typedef bool (CPhysicsMesh::*SweepMember)(const C3DTriangle2&, float,
    const C3DVector&, const C3DVector&, float, C3DVector&, C3DVector&) const;

template<SweepMember Method>
#if defined(__GNUC__) || defined(__clang__)
__attribute__((used))
#endif
static SweepMember OriginalSweepMember()
{
    return Method;
}

// VC7.1 checks both private overloads before choosing the requested type.
#if defined(_MSC_VER) && _MSC_VER == 1310 && !defined(__clang__)
template SweepMember OriginalSweepMember<
    static_cast<SweepMember>(&CPhysicsMesh::HasCollidedWithTriangleAt)>();
#else
template SweepMember OriginalSweepMember<&CPhysicsMesh::HasCollidedWithTriangleAt>();
#endif
}