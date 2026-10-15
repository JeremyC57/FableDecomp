// CGameCameraManager::GetViewWidthAtPosition(const C3DVector&) -- width of the camera's horizontal view at the distance of a world position: 2 * distance * tan(HorizontalFOV / 2), the FOV held in turns
// Leaf: GetViewWidthAtPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006977F0, length 106 bytes. Catalog label "CDisplayEngine::GetSeaLevel" is propagated
// from the callee: 0x00A0BEB0 is a shared `fld [ecx+2Ch] ; ret` getter (CMap::DrawGetSeaLevel by ICF), here CCamera::GetHorizontalFOV
// on the manager's Camera at +4; the body sits among the CGameCameraManager methods (0x00697700 .. 0x006978D0). The original name is
// not recorded; the leaf states what the function computes.
// Callees: CCamera::GetHorizontalFOV() @ 0x00A0BEB0. Constants: 0.5f @ 0x0122F59C, (double)6.2831855f @ 0x0124F2B8.
// Layout: engine/CGameCameraManager.h (PDB, fits): Camera +4 (CCamera: WorldPosition +0, HorizontalFOV +0x2c).
// Shape: the FOV is fetched first; the camera position is copied into a frame C3DVector (memberwise user copy constructor: x and y
// forwarded into the x87, z through eax), the delta built component by component (x spilled to the frame), the length is
// sqrt(z*z + y*y + x*x) through the intrinsic, tan is the x87 `fptan` intrinsic on `(fov * 0.5f) * (double)TWO_PI` (two
// multiplies, `fmul dword` then `fmul qword`; an all-float product folds 0.5 * 2pi into one constant), `* 2` is `fadd st,st`.
#include "engine/CGameCameraManager.h"
#include <math.h>
#pragma intrinsic(sqrt, tan)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CCamera {                                   // PDB 56 bytes
    C3DVector WorldPosition;                       // +0x00
    unsigned char _rest[0x2c - 0xc];               // RHSet, locks, FOVFlags
    float     HorizontalFOV;                       // +0x2c
    float GetHorizontalFOV() const;                // 0x00A0BEB0
};

const float TWO_PI = 6.2831855f;

struct CGameCameraManager_Methods : CGameCameraManager {
    float GetViewWidthAtPosition(const C3DVector& position);
};

float CGameCameraManager_Methods::GetViewWidthAtPosition(const C3DVector& position)
{
    const CCamera* camera = reinterpret_cast<const CCamera*>(Camera);
    float fov = camera->GetHorizontalFOV();
    C3DVector cameraPosition(camera->WorldPosition);
    C3DVector delta;
    delta.X = position.X - cameraPosition.X;
    delta.Y = position.Y - cameraPosition.Y;
    delta.Z = position.Z - cameraPosition.Z;
    float distance = (float)sqrt(delta.Z * delta.Z + delta.Y * delta.Y + delta.X * delta.X);
    return (float)(tan((fov * 0.5f) * (double)TWO_PI) * distance) * 2.0f;
}
