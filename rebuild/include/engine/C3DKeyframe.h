#ifndef FABLE_ENGINE_C3DKEYFRAME_H
#define FABLE_ENGINE_C3DKEYFRAME_H
#include "engine/CQuaternion.h"
#include "engine/C4DVector.h"
class CMatrix4x4;
// Both original PDBs: complete48-byte value; Rotation0,Position16,Scaling32.
class C3DKeyframe {
public:
 CQuaternion Rotation;
 C4DVector Position;
 C4DVector Scaling;
 C3DVector GetPosition()const { return Position.ToC3DVector(); }
 void ToMatrix(CMatrix4x4& matrix,bool applyScaling)const;
 void ToMatrix(CMatrix3x4& matrix,bool applyScaling)const;
 void ToMatrixTransposed(CMatrix4x4& matrix,bool applyScaling)const;
};
#endif
