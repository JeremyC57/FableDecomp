// CEngineClothSimulationMeshBase::CalculateFadeOut(const C3DVector& position) const -- the cloth simulation fade-out factor: 0 inside SimFadeBegin of the zoomed simulation radius from the engine camera, rising linearly to 1 at the radius, clamped
// Grade: DIFFER(151v151) -- same length; x87 only: retail stores the distance (`fst [esp]`) after the three stack pops (ours right after fsqrt) and multiplies SimFadeBegin*Zoom*SimRadius / Zoom*SimRadius (ours SimFadeBegin*SimRadius*Zoom / SimRadius*Zoom whatever the source order); 4 grades
// Leaf: ?CalculateFadeOut@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B806E0, 151 bytes. __thiscall (this unused), `ret 4`, float in st0.
// Identity: FableWin.pdb `?CalculateFadeOut@CEngineClothSimulationMeshBase@@IBEMABVC3DVector@@@Z` (protected const,
//   float (const C3DVector&)); callers 0x00B813AB (the cloth mesh update) and the two Render overrides 0x00B8155C /
//   0x00B816EC (CEngineClothSimulationAnimatedMesh / CEngineClothSimulation2DAnimatedMesh). The catalog label
//   "Render_CalculateClothVertexDistance" is descriptive.
// Layout: the simulation manager global 0x01436E68 (its MaxIterations +0x20 is the landed GetMaximumIterations
//   0x00B806C0); Ego_r.pdb CEngineSimulationManager has SimRadius / SimFadeBegin / Zoom at +4216/+4220/+4224 behind
//   three 12-byte Dinkumware lists -- retail's STLport lists are 4 bytes, so they sit at +0x1060 / +0x1064 / +0x1068.
//   The engine camera global 0x01436EA0 (engine/CEngineCamera.h; camera position CameraDesc.WorldPosition +0x54).
//   0.0f / 1.0f pooled at 0x0122DEDC / 0x0122DED8.
// Shape: `fade = 0` first (the leading `fld 0`), the camera distance as `(position - camera).Length()`, the fade begin
//   `SimFadeBegin * Zoom * SimRadius`, then `if (distance > begin) { fade = (distance - begin) / (Zoom * SimRadius -
//   begin); if (fade > 1) fade = 1; }`.
#include "engine/CEngineCamera.h"

extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Length() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

extern CEngineCamera* g_pEngineCamera;                  // 0x01436EA0

inline const C3DVector& PeekCameraPos(const CEngineCamera* camera)
{
    return *(const C3DVector*)(camera->CameraDesc + 0x40);   // CameraDesc.WorldPosition
}

class CEngineSimulationManager {                        // retail layout of the fields used here
public:
    unsigned char _pad000[0x1060];
    float         SimRadius;                            // +0x1060
    float         SimFadeBegin;                         // +0x1064
    float         Zoom;                                 // +0x1068
};

extern CEngineSimulationManager* g_pEngineSimulationManager;   // 0x01436E68

class CEngineClothSimulationMeshBase {
protected:
    float CalculateFadeOut(const C3DVector& position) const;
};

float CEngineClothSimulationMeshBase::CalculateFadeOut(const C3DVector& position) const
{
    float fade = 0.0f;
    C3DVector offset = position - PeekCameraPos(g_pEngineCamera);
    float distance = offset.Length();
    const CEngineSimulationManager* manager = g_pEngineSimulationManager;
    float fadeBegin = manager->SimFadeBegin * manager->SimRadius * manager->Zoom;
    if (distance > fadeBegin) {
        fade = (distance - fadeBegin) / (manager->SimRadius * manager->Zoom - fadeBegin);
        if (fade > 1.0f)
            fade = 1.0f;
    }
    return fade;
}
