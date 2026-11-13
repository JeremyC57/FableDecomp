#ifndef FABLE_ENGINE_C3DKEYFRAME_H
#define FABLE_ENGINE_C3DKEYFRAME_H
#include "engine/CQuaternion.h"
#ifdef FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY
#ifndef FABLE_RETAIL_CQUATERNION_MULASSIGN
#define FABLE_RETAIL_CQUATERNION_MULASSIGN
#endif
#ifndef FABLE_RETAIL_C4DVECTOR_ADDASSIGN
#define FABLE_RETAIL_C4DVECTOR_ADDASSIGN
#endif
#endif
#include "engine/C4DVector.h"
class CMatrix4x4;
// Both original PDBs: complete48-byte value; Rotation0,Position16,Scaling32.
class C3DKeyframe {
public:
 CQuaternion Rotation;
 C4DVector Position;
 C4DVector Scaling;
#ifdef FABLE_RETAIL_C3DKEYFRAME_CTOR
 // Retail opt-in (chunk 2005): FableWin.pdb ??0C3DKeyframe@@QAE@XZ (user default constructor; no stores in retail).
 C3DKeyframe() { }
#endif
 C3DVector GetPosition()const { return Position.ToC3DVector(); }
 void ToMatrix(CMatrix4x4& matrix,bool applyScaling)const;
 void ToMatrix(CMatrix3x4& matrix,bool applyScaling)const;
 void ToMatrixTransposed(CMatrix4x4& matrix,bool applyScaling)const;
#ifdef FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY
 // Retail opt-in (chunk 2005, row 0x00A4D920): FableWin.pdb ?PostApplyRelativeAnimation@C3DKeyframe@@QAEXABV1@@Z (expanded in
 // retail: the out-of-line CQuaternion::operator*= 0x00A88C10, then C4DVector::operator+= ??YC4DVector@@QAEABV0@ABV0@@Z inline).
 void PostApplyRelativeAnimation(const C3DKeyframe& other) { Rotation *= other.Rotation; Position += other.Position; }
#endif
};
#endif
