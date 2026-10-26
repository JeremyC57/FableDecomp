// CEngineClothSimulationMeshBase::CalculateFadeOut(const C3DVector& position) const -- the cloth simulation fade-out factor: 0 inside SimFadeBegin of the zoomed simulation radius from the engine camera, rising linearly to 1 at the radius, clamped
// Leaf: ?CalculateFadeOut@CEngineClothSimulationMeshBase@@IBEMABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B806E0, 151 bytes. __fastcall (this unused), `ret 4`, float in st0.
// Identity: FableWin.pdb `?CalculateFadeOut@CEngineClothSimulationMeshBase@@IBEMABVC3DVector@@@Z` (protected const,
//   float (const C3DVector&)); callers 0x00B813AB (the cloth mesh update) and the two Render overrides 0x00B8155C /
//   0x00B816EC (CEngineClothSimulationAnimatedMesh / CEngineClothSimulation2DAnimatedMesh). The catalog label
//   "Render_CalculateClothVertexDistance" is descriptive.
// Layout: the simulation manager global 0x01436E68 (its MaxIterations +0x20 is the landed GetMaximumIterations
//   0x00B806C0); Ego_r.pdb CEngineSimulationManager has SimRadius / SimFadeBegin / Zoom at +4216/+4220/+4224 behind
//   three 12-byte Dinkumware lists -- retail's STLport lists are 4 bytes, so they sit at +0x1060 / +0x1064 / +0x1068.
//   The engine camera global 0x01436EA0 (engine/CEngineCamera.h; camera position CameraDesc.WorldPosition +0x54).
//   0.0f / 1.0f pooled at 0x0122DEDC / 0x0122DED8.
// Shape: `fade = 0` first (the leading `fld 0`), the camera distance as the temporary `(position - camera)
//   .GetMagnitude()`, the fade begin `SimFadeBegin * Zoom` then `*= SimRadius` (separate statements: one expression is
//   reassociated by VC7.1 to SimFadeBegin*SimRadius*Zoom), then `if (distance > begin) { radius = Zoom * SimRadius;
//   fade = (distance - begin) / (radius - begin); if (fade > 1) fade = 1; }`. Retry pass 3: with <math.h> and a C3DVector with a
//   user default ctor (the landed 0x00BAA4E0 model) the distance store follows the three x87 pops
//   as in retail.
// Layout (supersedes any local-model note above): engine/CEngineSimulationManager.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CEngineSimulationManager CEngineSimulationManagerDonor
#include "engine/CEngineSimulationManager.h"
#undef CEngineSimulationManager
#include "engine/CEngineCamera.h"

#include <math.h>
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

extern CEngineCamera* g_pEngineCamera;                  // 0x01436EA0

inline const C3DVector& PeekCameraPos(const CEngineCamera* camera)
{
    return *(const C3DVector*)(camera->CameraDesc + 0x40);   // CameraDesc.WorldPosition
}

class CEngineSimulationManager : public CEngineSimulationManagerDonor {
                        // retail layout of the fields used here
public:
};

extern CEngineSimulationManager* g_pEngineSimulationManager;   // 0x01436E68

class CEngineClothSimulationMeshBase {
protected:
    float CalculateFadeOut(const C3DVector& position) const;
};

float CEngineClothSimulationMeshBase::CalculateFadeOut(const C3DVector& position) const
{
    float fade = 0.0f;
    float distance = (position - PeekCameraPos(g_pEngineCamera)).GetMagnitude();
    const CEngineSimulationManager* manager = g_pEngineSimulationManager;
    float fadeBegin = manager->SimFadeBegin * manager->Zoom;
    fadeBegin *= manager->SimRadius;
    if (distance > fadeBegin) {
        float radius = manager->Zoom * manager->SimRadius;
        fade = (distance - fadeBegin) / (radius - fadeBegin);
        if (fade > 1.0f)
            fade = 1.0f;
    }
    return fade;
}