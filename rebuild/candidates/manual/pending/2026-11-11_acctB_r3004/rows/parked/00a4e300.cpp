// C3DAnimation2::GetDeltaMovement(CMatrix3x4& delta, float start_time, float end_time) const -- the movement track's relative transform between two times: the start rotation transposed times the end rotation, and the position delta plus the linear movement vector's share, rotated into the start frame (identity without a movement track)
// Grade: DIFFER(1609v1619) -- x87 order / register residue: start ToMatrix keeps E11..E13 on the x87 stack in retail (0x00A4E567 fxch / fstp [esp+8] / fstp [esp+18h]), product rows 2 / 3 term order (retail b1j*a12, b3j*a32, b2j*a22 and b2j*a23, b1j*a13, b3j*a33), translation E42 / E43 term order, frame 0x100 v 0x104; 97 frame-masked diff lines
// Leaf: ?GetDeltaMovement@C3DAnimation2@@UBEXAAVCMatrix3x4@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4E300, 1619 bytes, thiscall, `ret 0Ch`, virtual (vtable 0x0129D560). Identity: Ego_r.pdb /
//   FableWin.pdb ?GetDeltaMovement@C3DAnimation2@@UBEXAAVCMatrix3x4@@MM@Z.
// Callees: C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&, float) const 0x00A4DEA0 (twice, through the inline
//   C3DAnimationSequence forwarder). Everything else is expanded: GetNonLoopingDuration (COMDAT 0x00A4BE10), C3DKeyframe::ToMatrix
//   (COMDAT 0x00A52450) with the forced CQuaternion::ToMatrix, SetMultiplyNoTranslation (COMDAT 0x00A4B820: 3x3 product, translation
//   row cleared -- the three `mov [ecx+2Ch/28h/24h],esi` zero stores), SetTranslateOnly (COMDAT 0x00A4B970), the inline transpose and
//   TransformVector of the start rotation.
// Evidence for the shape: FableWin.pdb emits ?SetMultiplyNoTranslation@CMatrix3x4 (0x032289C0) and ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix3x4@@_N@Z
//   (0x03228BF0) right after ?GetDeltaMovement (0x03228780): their first user. The movement vector is read in place (protected member):
//   the by-value getter always materialises its copy (GetMovementAtTime 0x00A4E1A7).
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2_NONLOOPINGDURATION
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DANIMATIONSTATS_MOVEMENTVECTOR
#define FABLE_RETAIL_C3DANIMATIONSTATS_PROTECTED
#define FABLE_RETAIL_C3DANIMATIONSEQUENCE_GETKEYFRAME
#define FABLE_RETAIL_CMATRIX3X4_MULTIPLYNOTRANSLATION
#define FABLE_RETAIL_C3DKEYFRAME_TOMATRIX_INLINE
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_TRANSPOSE
#define FABLE_RETAIL_CMATRIX3X4_TRANSPOSE_LOWER
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CQUATERNION_TOMATRIX_FORCEINLINE
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimation2.h"

void C3DAnimation2::GetDeltaMovement(CMatrix3x4& delta, float start_time, float end_time) const
{
    if (MovementTrack.Get())
    {
        float duration = GetNonLoopingDuration();
        if (start_time < 0.0f)
            start_time = 0.0f;
        else if (start_time > duration)
            start_time = duration;
        if (end_time < 0.0f)
            end_time = 0.0f;
        else if (end_time > duration)
            end_time = duration;
        C3DKeyframe start_key;
        C3DKeyframe end_key;
        MovementTrack->GetKeyframeInterpolate(start_key, start_time);
        MovementTrack->GetKeyframeInterpolate(end_key, end_time);
        CMatrix3x4 start_matrix;
        CMatrix3x4 end_matrix;
        start_key.ToMatrix(start_matrix, false);
        end_key.ToMatrix(end_matrix, false);
        start_matrix.Transpose();
        delta.SetMultiplyNoTranslation(start_matrix, end_matrix);
        C3DVector movement = end_key.GetPosition() - start_key.GetPosition();
        if (duration > 0.0f)
            movement += MovementVector * ((end_time - start_time) / duration);
        start_matrix.TransformVector(movement);
        delta.SetTranslateOnly(movement);
    }
    else
    {
        delta.SetIdentity();
    }
}
