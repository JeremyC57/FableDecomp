// 00a364e0 C3DMeshObject::GetDummyObjectPositionStatic
// Leaf: ?GetDummyObjectPositionStatic@C3DMeshObject@@QBEXABVCCharString@@JAAVC3DVector@@11@Z
// Flags: none
#define FABLE_RETAIL_C3DMESHOBJECT_STATIC_DUMMY_PASS26
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST_INLINE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_C3DMESHSTATS_ISANIMATED
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_MESH_DUMMY_INVERSE_STEP_PASS26
#include "fable_assert.h"
#include "engine/CCRC.h"
#include "engine/C3DVector.h"
#include "engine/C3DDummyObject.h"
#include "engine/C3DMeshStats.h"
#include "engine/C3DMeshObject.h"

void C3DMeshObject::GetDummyObjectPositionStatic(const CCharString& name, long n, C3DVector& dummy_pos,
                                                C3DVector& dummy_direction, C3DVector& dummy_up) const
{
    FABLE_RETAIL_ASSERT(!Mesh->IsAnimated());
    const C3DDummyObject* dummy = Mesh->GetDummyObject(name.GetCRC(), n);
    FABLE_RETAIL_ASSERT(dummy);
    dummy_pos = dummy->GetPosition();
    PositionMatrix.TransformPoint(dummy_pos);
    CMatrix3x4 mat = dummy->PeekTransform();
    mat.PostMultiply(PositionMatrix);
    dummy_direction.Initialise(mat.E21, mat.E22, mat.E23);
    dummy_up.Initialise(mat.E31, mat.E32, mat.E33);
    dummy_direction.Normalise();
    dummy_up.Normalise();
}