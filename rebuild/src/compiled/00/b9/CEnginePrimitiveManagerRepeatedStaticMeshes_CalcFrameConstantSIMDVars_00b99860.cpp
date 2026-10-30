// CEnginePrimitiveManagerRepeatedStaticMeshes::CalcFrameConstantSIMDVars() -- splat the engine camera's world position and the normals of its four frustum clip planes (left, right, top, bottom) into the four lanes of the SIMD variable block's per-frame camera vectors
// Leaf: ?CalcFrameConstantSIMDVars@CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B99860, 386 bytes, private __fastcall, `ret`.
// Identity (proven, two sources): (1) Ego_r.pdb and FableWin.pdb both name `?CalcFrameConstantSIMDVars@
//   CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXXZ` (private, no arguments); Ego_r donor order CalcFrameConstantSIMDVars
//   0x00A51A70, ~dtor 0x00A51BB0, FrameStart 0x00A51D20 matches retail 0x00B99860 just before FrameStart 0x00B99B50;
//   (2) its only references are FrameStart's two tail jumps (0x00B99B82 / 0x00B99BAB, after choosing the depth- or
//   colour-buffer shadowed shader set), and the stores land exactly on the PDB layout of CSIMDVariableBlock
//   (this->SIMDVariableBlock +0x90; CameraPosX +0xB00 .. CameraClipPlaneBottomZ +0xBE0, float[4] each). Manifest label
//   "engine_copy_global_transform_to_array" is wrong.
// Data: the engine camera global 0x01436EA0 under its dominant landed spelling `?g_pEngineCamera@@3PAVCEngineCamera@@A`
//   (no PDB data symbol; several other spellings are bound, see NOTES.md); its CameraDesc (+0x14) WorldPosition (+0x40)
//   = +0x54 and FrustrumPlanes CPlane[4] at +0x1C0 (engine/CEngineCamera.h).
// Shape: one loop over the four lanes (VC7.1 strength-reduces the lane offset into eax 0xB10..0xB20). The position is
//   read through the global at every store (the float stores may alias it); the four plane normals are copied into
//   C3DVector locals first (X/Y of the left one stay on the x87 stack, the rest go through esi into frame slots), then
//   stored. C3DVector's user memberwise copy constructor is what makes it match: with the implicit copy VC7.1 copies
//   each normal as a block through a `lea` pointer and also unrolls the whole loop four times (1444v386).
// Layout: `this` is engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h and the camera engine/CEngineCamera.h, both
//   included under a layout name (D34 pattern) so the private member and two inline accessors can be declared;
//   CSIMDVariableBlock / CEngineCameraDesc / CPlane / C3DVector have no engine header (local models with the PDB member
//   names; header debt in NOTES.md). The accessor names GetPosition / GetFrustrumPlane are descriptive (inline, emitted
//   nowhere).
#pragma pack(push, 1)
class C3DVector {                                  // retail C3DVector is 1-aligned
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
#pragma pack(pop)
class CPlane {                                     // Ego_r: 16 bytes
public:
    C3DVector Normal;                              // +0x00
    float     Distance;                            // +0x0C
};

struct CSIMDVariableBlock {                        // Ego_r: 0xC00 bytes (allocated 16-aligned by the ctor 0x00B99D90)
    float         NormalX[32];                     // +0x000
    float         NormalY[32];                     // +0x080
    float         NormalZ[32];                     // +0x100
    float         PosX[32];                        // +0x180
    float         PosY[32];                        // +0x200
    float         PosZ[32];                        // +0x280
    unsigned char ObjectMatricies[32 * 16];        // +0x300 C4DVector[32]
    unsigned char ObjectOffsets[32 * 16];          // +0x500 C4DVector[32]
    unsigned char Colour[32 * 16];                 // +0x700 CRGBFloatColour[32]
    unsigned char MainLightColour[32 * 16];        // +0x900 CRGBFloatColour[32]
    float CameraPosX[4];                           // +0xB00
    float CameraPosY[4];                           // +0xB10
    float CameraPosZ[4];                           // +0xB20
    float CameraClipPlaneLeftX[4];                 // +0xB30
    float CameraClipPlaneLeftY[4];                 // +0xB40
    float CameraClipPlaneLeftZ[4];                 // +0xB50
    float CameraClipPlaneRightX[4];                // +0xB60
    float CameraClipPlaneRightY[4];                // +0xB70
    float CameraClipPlaneRightZ[4];                // +0xB80
    float CameraClipPlaneTopX[4];                  // +0xB90
    float CameraClipPlaneTopY[4];                  // +0xBA0
    float CameraClipPlaneTopZ[4];                  // +0xBB0
    float CameraClipPlaneBottomX[4];               // +0xBC0
    float CameraClipPlaneBottomY[4];               // +0xBD0
    float CameraClipPlaneBottomZ[4];               // +0xBE0
    float FadeDistance[4];                         // +0xBF0
};

#define CEngineCamera CEngineCamera_Layout
#include "engine/CEngineCamera.h"
#undef CEngineCamera
struct CEngineCameraDesc {                         // Ego_r: 0x6C bytes; only the prefix up to WorldPosition
    unsigned char Window[0x10];                    // +0x00 C2DBoxF
    unsigned char RotationMatrix[0x30];            // +0x10 CMatrix3x4
    C3DVector     WorldPosition;                   // +0x40
};
class CEngineCamera : public CEngineCamera_Layout {
public:
    const C3DVector& GetPosition() const { return ((const CEngineCameraDesc*)CameraDesc)->WorldPosition; }
    const CPlane& GetFrustrumPlane(long i) const { return ((const CPlane*)FrustrumPlanes)[i]; }
};
extern CEngineCamera* g_pEngineCamera;             // 0x01436EA0

#define CEnginePrimitiveManagerRepeatedStaticMeshes CEnginePrimitiveManagerRepeatedStaticMeshes_Layout
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#undef CEnginePrimitiveManagerRepeatedStaticMeshes
class CEnginePrimitiveManagerRepeatedStaticMeshes : public CEnginePrimitiveManagerRepeatedStaticMeshes_Layout {
    void CalcFrameConstantSIMDVars();              // private (AAE)
};

void CEnginePrimitiveManagerRepeatedStaticMeshes::CalcFrameConstantSIMDVars()
{
    for (long i = 0; i < 4; ++i) {
        SIMDVariableBlock->CameraPosX[i] = g_pEngineCamera->GetPosition().X;
        SIMDVariableBlock->CameraPosY[i] = g_pEngineCamera->GetPosition().Y;
        SIMDVariableBlock->CameraPosZ[i] = g_pEngineCamera->GetPosition().Z;
        C3DVector left = g_pEngineCamera->GetFrustrumPlane(0).Normal;
        C3DVector right = g_pEngineCamera->GetFrustrumPlane(1).Normal;
        C3DVector top = g_pEngineCamera->GetFrustrumPlane(2).Normal;
        C3DVector bottom = g_pEngineCamera->GetFrustrumPlane(3).Normal;
        SIMDVariableBlock->CameraClipPlaneLeftX[i] = left.X;
        SIMDVariableBlock->CameraClipPlaneLeftY[i] = left.Y;
        SIMDVariableBlock->CameraClipPlaneLeftZ[i] = left.Z;
        SIMDVariableBlock->CameraClipPlaneRightX[i] = right.X;
        SIMDVariableBlock->CameraClipPlaneRightY[i] = right.Y;
        SIMDVariableBlock->CameraClipPlaneRightZ[i] = right.Z;
        SIMDVariableBlock->CameraClipPlaneTopX[i] = top.X;
        SIMDVariableBlock->CameraClipPlaneTopY[i] = top.Y;
        SIMDVariableBlock->CameraClipPlaneTopZ[i] = top.Z;
        SIMDVariableBlock->CameraClipPlaneBottomX[i] = bottom.X;
        SIMDVariableBlock->CameraClipPlaneBottomY[i] = bottom.Y;
        SIMDVariableBlock->CameraClipPlaneBottomZ[i] = bottom.Z;
    }
}