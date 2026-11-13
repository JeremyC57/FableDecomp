#ifndef FABLE_ENGINE_CENGINECAMERADESC_H
#define FABLE_ENGINE_CENGINECAMERADESC_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/C2DBoxF.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_CENGINECAMERADESC_STRUCT
// Retail opt-in (chunk 2008, row 0x00B665E0): the PDB key is `struct` (Ego_r.pdb ??0CEngineCameraDesc@@QAE@ABU0@@Z 0x009EBB70 = retail
// 0x00B4AF50, ?SetCameraState@CEngineCamera@@QAEXABUCEngineCameraDesc@@_N1@Z; pdb_class_keys.tsv U). The implicit copy constructor is
// non-trivial (C3DVector's user copy) and retail calls its out-of-line COMDAT 0x00B4AF50. Same layout.
struct CEngineCameraDesc {
#else
class CEngineCameraDesc {
#endif
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
