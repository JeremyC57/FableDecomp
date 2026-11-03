#ifndef FABLE_ENGINE_CENGINECAMERADESC_H
#define FABLE_ENGINE_CENGINECAMERADESC_H
// CEngineCameraDesc (chunk 723b): Ego_r.pdb class, 0x6C bytes, no container member (retail = Ego_r). The water passes read
// WorldPosition through CEngineCamera::CameraDesc (+0x14 + 0x40 = +0x54, 0x00B7084C).
#ifndef FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#endif
#ifndef FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
class CEngineCameraDesc {
public:
    C2DBoxF    Window;                                                   // +0x00
    CMatrix3x4 RotationMatrix;                                           // +0x10
    C3DVector  WorldPosition;                                            // +0x40
    float      HorizontalFOV;                                            // +0x4c
    float      VerticalFOV;                                              // +0x50
    bool       Use2DFOV;                                                 // +0x54
    float      NearZPlane;                                               // +0x58
    float      FarZPlane;                                                // +0x5c
    float      ZBufferStart;                                             // +0x60
    float      ZBufferEnd;                                               // +0x64
    bool       StateValid;                                               // +0x68
};
FABLE_STATIC_ASSERT(sizeof(CEngineCameraDesc) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CEngineCameraDesc, WorldPosition) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CEngineCameraDesc, NearZPlane) == 0x58);
#endif // FABLE_ENGINE_CENGINECAMERADESC_H
