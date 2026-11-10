#ifndef FABLE_ENGINE_CENGINECAMERADESC_H
#define FABLE_ENGINE_CENGINECAMERADESC_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/C2DBoxF.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
class CEngineCameraDesc {
public:
 C2DBoxF Window;
 CMatrix3x4 RotationMatrix;
 C3DVector WorldPosition;
 float HorizontalFOV,VerticalFOV;
 bool Use2DFOV;
 float NearZPlane,FarZPlane,ZBufferStart,ZBufferEnd;
 bool StateValid;
};
FABLE_STATIC_ASSERT(sizeof(CEngineCameraDesc)==108);
#endif
