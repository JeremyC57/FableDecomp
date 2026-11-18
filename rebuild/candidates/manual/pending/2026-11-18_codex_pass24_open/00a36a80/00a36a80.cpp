// 00a36a80 C3DMeshObject::GetDummyObjectPositionAnimated -- Animates a named dummy and transforms its position and direction axes.
// Grade: DIFFER(747v747) -- Equal length. Stack allocation and x87 scheduling after constructing the animated dummy differ.
// Retail: 0x00A36A80, 747 bytes, thiscall.
// Leaf: ?GetDummyObjectPositionAnimated@C3DMeshObject@@QBEXABVCCharString@@JABVC3DAnimationBlendState@@1MAAVC3DAnimationManager@@AAVC3DVector@@33_N@Z
#define FABLE_RETAIL_C3DMESHOBJECT_DUMMY_PAIR_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST_INLINE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_CMOVABLERESOURCE
#include "engine/CCRC.h"
#include "engine/C3DVector.h"
#include "engine/C3DDummyObject.h"
#include "engine/C3DMeshStats.h"
#include "engine/C3DMeshObject.h"

void C3DMeshObject::GetDummyObjectPositionAnimated(const CCharString& name, long n, const C3DAnimationBlendState& anim_state,
    const C3DAnimationBlendState& anim_state_end, float interpolate, C3DAnimationManager& anim_manager, C3DVector& dummy_pos,
    C3DVector& dummy_direction, C3DVector& dummy_up, bool use_morph_scale) const
{
    const C3DDummyObject* dummy = &Mesh->FindDummyObjectsBegin(name.GetCRC())[n].second;
    C3DDummyObject dest;
    Mesh->GetDummyObjectPositionAnimated(*dummy, dest, anim_state, anim_state_end, interpolate, anim_manager, use_morph_scale);
    const CMatrix3x4& mat = dest.PeekTransform();
    dummy_pos = C3DVector(mat.E41, mat.E42, mat.E43);
    PositionMatrix.TransformPoint(dummy_pos);
    C3DVector direction, up;
    direction.X = mat.E22 * PositionMatrix.E21 + mat.E23 * PositionMatrix.E31 + mat.E21 * PositionMatrix.E11;
    direction.Y = mat.E22 * PositionMatrix.E22 + mat.E23 * PositionMatrix.E32 + mat.E21 * PositionMatrix.E12;
    direction.Z = mat.E22 * PositionMatrix.E23 + mat.E23 * PositionMatrix.E33 + mat.E21 * PositionMatrix.E13;
    up.X = mat.E32 * PositionMatrix.E21 + mat.E33 * PositionMatrix.E31 + mat.E31 * PositionMatrix.E11;
    up.Y = mat.E32 * PositionMatrix.E22 + mat.E33 * PositionMatrix.E32 + mat.E31 * PositionMatrix.E12;
    up.Z = mat.E32 * PositionMatrix.E23 + mat.E33 * PositionMatrix.E33 + mat.E31 * PositionMatrix.E13;
    dummy_direction = direction;
    dummy_up = up;
    dummy_direction.Normalise();
    dummy_up.Normalise();
}
