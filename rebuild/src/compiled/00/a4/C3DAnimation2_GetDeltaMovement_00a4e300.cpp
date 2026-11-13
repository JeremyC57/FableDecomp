// C3DAnimation2::GetDeltaMovement(CMatrix3x4& delta, float start_time, float end_time) const -- the movement track's relative transform between two times: the start rotation transposed times the end rotation, and the position delta plus the linear movement vector's share, rotated into the start frame (identity without a movement track)
// Fitted (coordinator, chunk 3010): CMatrix3x4::SetMultiplyNoTranslation is `SetMultiply(m1, m2); E41 = E42 = E43 = 0.0f;` -- with the 9-sum body the row is DIFFER(1615v1619)
//   on 13 x87 commutative orders only (c2 sorts each n-ary add by a 16-bit hash of operand record ids; retail carries about 60..88 more records, NOTES.md session 3).
//   For: the row is byte-exact with it and the helper compiled out of line equals retail 0x00A4B820 (240 bytes). Against: FableWin.pdb's debug copy (160 /Od
//   instructions) fits the 9-sum body; FableWin is a later revision. Needs the main coordinator's ruling (README).
// Leaf: ?GetDeltaMovement@C3DAnimation2@@UBEXAAVCMatrix3x4@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4E300, 1619 bytes, thiscall, `ret 0Ch`, virtual (vtable 0x0129D560). Identity: Ego_r.pdb /
//   FableWin.pdb ?GetDeltaMovement@C3DAnimation2@@UBEXAAVCMatrix3x4@@MM@Z.
// Callees: C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&, float) const 0x00A4DEA0 (twice, through the inline
//   C3DAnimationSequence forwarder). Everything else is expanded: GetNonLoopingDuration (COMDAT 0x00A4BE10), C3DKeyframe::ToMatrix
//   (COMDAT 0x00A52450) with the forced CQuaternion::ToMatrix and the forced PreScale(MMM), SetMultiplyNoTranslation (COMDAT 0x00A4B820: 3x3 product, translation
//   row cleared -- the three `mov [ecx+2Ch/28h/24h],esi` zero stores), SetTranslateOnly (COMDAT 0x00A4B970), the inline transpose and
//   TransformVector of the start rotation.
// Clamps: GFLimit (Ego_r / FableWin ??$GFLimit@MMM@@YIXAAMABM1@Z; FableWin's debug GetDeltaMovement is 119 instructions = this text's
//   113 + its by-value / float-argument artefacts, the if-chains give 129). Transpose (three SWAP<float> calls, FableWin) sits
//   between the two ToMatrix calls and copies the upper triangle (0x00A4E567 spills E12 / E13, 0x00A4E571 reloads E23
//   before the second conversion); TransformVector's X row is grouped like TransformPoint's (0x00A4E8A5: zz*E31 + yy*E21, then xx*E11).
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
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_B
#define FABLE_RETAIL_CMATRIX3X4_MULTIPLYNOTRANSLATION
#define FABLE_RETAIL_C3DKEYFRAME_TOMATRIX_INLINE
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_GROUPED_X
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_TRANSPOSE
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CQUATERNION_TOMATRIX_FORCEINLINE
#include "engine/GFLimit.h"
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimation2.h"

void C3DAnimation2::GetDeltaMovement(CMatrix3x4& delta, float start_time, float end_time) const
{
    if (MovementTrack.Get())
    {
        float duration = GetNonLoopingDuration();
        GFLimit(start_time, 0.0f, duration);
        GFLimit(end_time, 0.0f, duration);
        C3DKeyframe start_key;
        C3DKeyframe end_key;
        MovementTrack->GetKeyframeInterpolate(start_key, start_time);
        MovementTrack->GetKeyframeInterpolate(end_key, end_time);
        CMatrix3x4 start_matrix;
        CMatrix3x4 end_matrix;
        start_key.ToMatrix(start_matrix, false);
        start_matrix.Transpose();
        end_key.ToMatrix(end_matrix, false);
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