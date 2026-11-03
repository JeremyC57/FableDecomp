// CEngineClothSimulationMeshBase::CalculateRunState(const C3DVector& position, float radius) const -- decide whether a cloth simulation sphere should run: out of the simulation radius (SimRadius scaled by the zoom), outside the camera's near/far depth range, or outside any of the four frustum side planes gives run state 1, otherwise 0
// Leaf: ?CalculateRunState@CEngineClothSimulationMeshBase@@IBE?AW4ERunState@CEngineSimulationBase@@ABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80940, length 388 bytes, `ret 8`; `this` is never read. Callers: the landed
//   CalcAndSetRunState overrides 0x00B81180 (static mesh) and 0x00B81230 (animated mesh).
// Name: Ego_r.pdb + FableWin.pdb `?CalculateRunState@CEngineClothSimulationMeshBase@@IBE?AW4ERunState@CEngineSimulationBase@@ABVC3DVector@@M@Z`
//   (protected const, returns CEngineSimulationBase::ERunState); the landed callers bind
//   `?CalculateRunState@CEngineSimulationBase@@QAEJABVC3DVector@@M@Z` (NAME_FIXES.tsv).
// Globals: g_pEngineCamera 0x01436EA0 (CEngineCamera: CameraDesc.WorldPosition +0x54, CameraDesc.NearZPlane +0x6C,
//   CameraDesc.FarZPlane +0x70, LookVector +0x1B4, FrustrumPlanes[4] +0x1C0 -- Ego_r.pdb names, engine/CEngineCamera.h offsets);
//   g_pEngineSimulationManager 0x01436E68 (SimRadius +0x1060, Zoom +0x1068 -- engine/CEngineSimulationManager.h offsets).
// Shape: the radius test is written `!(distance^2 < range^2)` (x87 `test ah,41h ; jne`); the depth and plane tests share one
//   `||` chain so the `return 1` block (with the depth pop) sits at the end of the function as in retail; the planes are tested
//   in the order 2, 3, 0, 1, each a dot product of the camera-relative offset (Z term first) against the radius.
// Enumerators: the ERunState values have no PDB names; ERunState_0 / ERunState_1 are descriptive.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float operator*(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
class CPlane { public:
    C3DVector Normal;                 // +0x00
    float     Distance;               // +0x0C
};
struct CEngineCameraDesc { public:
    unsigned char _pad00[0x40];       // Window, RotationMatrix
    C3DVector     WorldPosition;      // +0x40
    float         HorizontalFOV;      // +0x4C
    float         VerticalFOV;        // +0x50
    bool          Use2DFOV;           // +0x54
    float         NearZPlane;         // +0x58
    float         FarZPlane;          // +0x5C
    float         ZBufferStart;       // +0x60
    float         ZBufferEnd;         // +0x64
    bool          StateValid;         // +0x68
};
#define CEngineCamera CEngineCameraLayout
#include "engine/CEngineCamera.h"
#undef CEngineCamera
class CEngineCamera : public CEngineCameraLayout {
public:
    const CEngineCameraDesc& GetDesc() const { return *reinterpret_cast<const CEngineCameraDesc*>(CameraDesc); }
    const C3DVector& GetLookVector() const { return *reinterpret_cast<const C3DVector*>(&LookVector_X); }
    const CPlane& GetFrustrumPlane(long i) const { return reinterpret_cast<const CPlane*>(FrustrumPlanes)[i]; }
};
#define CEngineSimulationManager CEngineSimulationManagerLayout
#include "engine/CEngineSimulationManager.h"
#undef CEngineSimulationManager
class CEngineSimulationManager : public CEngineSimulationManagerLayout { };
extern CEngineCamera* g_pEngineCamera;                        // 0x01436EA0
extern CEngineSimulationManager* g_pEngineSimulationManager;  // 0x01436E68

class CEngineSimulationBase {
public:
    enum ERunState { ERunState_0, ERunState_1 };
};
class CEngineClothSimulationMeshBase : public CEngineSimulationBase {
protected:
    ERunState CalculateRunState(const C3DVector& position, float radius) const;
};

CEngineSimulationBase::ERunState CEngineClothSimulationMeshBase::CalculateRunState(const C3DVector& position, float radius) const
{
    const CEngineCamera* camera = g_pEngineCamera;
    C3DVector toCamera = camera->GetDesc().WorldPosition - position;
    float simRadius = g_pEngineSimulationManager->SimRadius * g_pEngineSimulationManager->Zoom;
    if (!(toCamera.GetMagnitudeSquared() < simRadius * simRadius))
        return ERunState_1;
    C3DVector offset = position - camera->GetDesc().WorldPosition;
    float depth = offset * camera->GetLookVector();
    if (depth + radius < camera->GetDesc().NearZPlane || depth - radius > camera->GetDesc().FarZPlane
        || offset * camera->GetFrustrumPlane(2).Normal > radius || offset * camera->GetFrustrumPlane(3).Normal > radius
        || offset * camera->GetFrustrumPlane(0).Normal > radius || offset * camera->GetFrustrumPlane(1).Normal > radius)
        return ERunState_1;
    return ERunState_0;
}