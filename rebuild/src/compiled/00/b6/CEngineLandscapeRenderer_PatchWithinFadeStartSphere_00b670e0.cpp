// CEngineLandscapeRenderer::PatchWithinFadeStartSphere(long x, long y) -- is the landscape point (x, y, 0) closer to the engine camera than the foreground fade-start distance
// Leaf: ?PatchWithinFadeStartSphere@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B670E0, 85 bytes. __fastcall, `ret 8`, bool in al.
// Identity: FableWin.pdb `?PatchWithinFadeStartSphere@CEngineLandscapeRenderer@@QAE_NJJ@Z` (bool (long, long)); it sits
//   in the CEngineLandscapeRenderer run (CreateMap 0x00B67140, CalcForegroundFadeFactors 0x00B671E0 -- landed, which
//   names +0x614 the foreground fade start). No direct caller in the image (inlined at its uses). The catalog label
//   "Entity_IsWithinRangeCheck" is descriptive.
// Layout: the camera is the engine's current camera global 0x01436EA0 (engine/CEngineCamera.h; its camera position is
//   CameraDesc.WorldPosition, +0x14 + 0x40 = +0x54). The renderer's PDB layout puts +0x614 inside the embedded mesh
//   builder, which does not fit retail, so the renderer is modelled locally (the landed CalcForegroundFadeFactors model).
// Shape: C3DVector((float)x, (float)y, 0.0f) minus the camera position (the zero-minus-z folds to `fchs`), its length by
//   `(float)sqrt(X*X + Y*Y + Z*Z)` (fsqrt), compared `<` the fade start as an if with two
//   literal returns (`mov al,1` / `xor al,al`; `return a < b` widens to eax).
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

class CEngineLandscapeRenderer {
public:
    unsigned char _pad000[0x614];
    float         m_foregroundFadeStart;                // +0x614
    float         m_foregroundFadeEnd;                  // +0x618
    bool PatchWithinFadeStartSphere(long x, long y);
};

bool CEngineLandscapeRenderer::PatchWithinFadeStartSphere(long x, long y)
{
    C3DVector offset = C3DVector((float)x, (float)y, 0.0f) - PeekCameraPos(g_pEngineCamera);
    if (offset.Length() < m_foregroundFadeStart)
        return true;
    return false;
}