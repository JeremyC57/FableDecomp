#ifndef FABLE_ENGINE_CENGINECAMERADESC_H
#define FABLE_ENGINE_CENGINECAMERADESC_H
// Retail layout (chunk 843): Ego_r.pdb CEngineCameraDesc (class key U: a struct, ?SetCameraState@CEngineCamera@@QAEXABUCEngineCameraDesc@@_N1@Z;
// struct_layouts_egor.tsv sizeof 0x6C): Window C2DBoxF +0x0, RotationMatrix CMatrix3x4 +0x10, WorldPosition +0x40, HorizontalFOV +0x4C,
// VerticalFOV +0x50, Use2DFOV +0x54, NearZPlane +0x58, FarZPlane +0x5C, ZBufferStart +0x60, ZBufferEnd +0x64, StateValid +0x68. Retail agrees:
// CEngineCamera::SetupGamut 0x00B2FD60 reads Window.TLX / TLY at camera +0x14 / +0x18 and FarZPlane at +0x70 (CameraDesc is camera +0x14);
// the landed RefreshD3DTransforms 0x00B2FC50 reads Near / Far / ZStart / ZEnd at +0x6C..+0x78. A new header: data members only.
#include "rebuild_abi.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
struct CEngineCameraDesc {
    C2DBoxF Window;                                         // +0x00
    CMatrix3x4 RotationMatrix;                              // +0x10
    C3DVector WorldPosition;                                // +0x40
    float HorizontalFOV;                                    // +0x4C
    float VerticalFOV;                                      // +0x50
    bool Use2DFOV;                                          // +0x54
    float NearZPlane;                                       // +0x58
    float FarZPlane;                                        // +0x5C
    float ZBufferStart;                                     // +0x60
    float ZBufferEnd;                                       // +0x64
    bool StateValid;                                        // +0x68
};
FABLE_STATIC_ASSERT(sizeof(CEngineCameraDesc) == 0x6C);
#endif // FABLE_ENGINE_CENGINECAMERADESC_H
