// CEngineCamera::SetupGamut() -- rebuild the view frustum: the camera's world origin (screen 0,0 at linear Z 0), the four far-plane window corners in world space, the world look vector (view +Z through the inverse view rotation, normalised), and the four side planes (normal of two adjacent corner edges from the origin, distance origin . normal)
// Grade: DIFFER(1802v1812) -- md 213: retail keeps corner 0 (Window.TLX, TLY, FarZPlane) wholly in x87 registers and re-stores each other corner's Z (`mov [esp+38h],edx` 0x00B2FF32, 0x00B2FFEA, 0x00B300A0) between the screen and the world transform, ours spills corner 0's Z to [esp+2Ch] and keeps no re-store; the top / right normals sit in swapped frame slots (retail top +0x6C, right +0x60); LookVector's three stores go through esi before `lea ecx` in retail
// Next idea: a screen transform whose Z write VC7.1 cannot prove redundant (the source of retail's re-store), or corner storage that is not one array (retail's slots are one ascending array at esp+24h); then the normal declaration order
// Leaf: ?SetupGamut@CEngineCamera@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2FD60, 1812 bytes, thiscall, plain `ret`.
// Identity (proven; catalog placeholder `_global::CPlane_InitializeFromCorners` corrected): Ego_r.pdb ?SetupGamut@CEngineCamera@@AAEXXZ (0x009D0650)
//   sits between ?RefreshD3DTransforms@CEngineCamera@@QAEXXZ (0x009D0560) and ?ClipSphere@CEngineCamera@@QBE_NABVC3DVector@@MPAK@Z (0x009D0BF0),
//   exactly as retail 0x00B2FD60 sits between the landed RefreshD3DTransforms 0x00B2FC50 and ClipSphere 0x00B30480; FableWin.pdb also names
//   ?SetupGamut@CEngineCamera@@AAEXXZ (0x02DC0AD0). Its only caller is 0x00B3110F inside CEngineCamera::SetCameraState 0x00B30B50 (private
//   AAE member, `this` in ecx, no arguments), and it writes exactly the CEngineCamera members LookVector +0x1B4 and FrustrumPlanes[4] +0x1C0
//   that ClipSphere reads.
// Callees: ?AccurateNormalise@C3DVector@@QAEAAV1@XZ 0x00A14440 (Ego_r.pdb out-of-line; the majority binding), ?Initialise@CPlane@@QAE_NABVC3DVector@@M@Z
//   0x00A42140 (Ego_r.pdb; `push distance ; push &normal`). Constant 0.0f at 0x0122DEDC (the origin's linear Z).
// Shape: corners as one C3DVector[4] (frame 0x8C and retail's ascending esp+24h..+50h slots; four named locals give 0x74 / 0x80);
//   C3DVector::Initialise per corner; the loop over the corners is fully unrolled by VC7.1; TransformViewToWorld sums each row left to
//   right through a matrix reference (header opt-in FABLE_RETAIL_CENGINECAMERA_GAMUT); TransformVector in its parenthesised row form
//   (FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SUMS); the four normals as cross products of the corner edges computed inline (VC7.1 CSEs the
//   edges as retail does); planes 2 (top), 1 (right), 3 (bottom), 0 (left) in retail's call order.
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_GAMUT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SUMS
#include "engine/CEngineCamera.h"

void CEngineCamera::SetupGamut()
{
    C3DVector origin(0.0f, 0.0f, 0.0f);
    C3DVector corners[4];
    corners[0].Initialise(CameraDesc.Window.TLX, CameraDesc.Window.TLY, CameraDesc.FarZPlane);
    corners[1].Initialise(CameraDesc.Window.TLX + WindowWidth, CameraDesc.Window.TLY, CameraDesc.FarZPlane);
    corners[2].Initialise(CameraDesc.Window.TLX + WindowWidth, CameraDesc.Window.TLY + WindowHeight, CameraDesc.FarZPlane);
    corners[3].Initialise(CameraDesc.Window.TLX, CameraDesc.Window.TLY + WindowHeight, CameraDesc.FarZPlane);
    TransformScreenXYLinearZToView(origin);
    TransformViewToWorld(origin);
    for (long i = 0; i < 4; ++i) {
        TransformScreenXYLinearZToView(corners[i]);
        TransformViewToWorld(corners[i]);
    }
    LookVector.X = 0.0f;
    LookVector.Y = 0.0f;
    LookVector.Z = 1.0f;
    InverseViewTransform.TransformVector(LookVector);
    LookVector.AccurateNormalise();
    C3DVector top = (corners[1] - origin) ^ (corners[0] - origin);
    C3DVector right = (corners[2] - origin) ^ (corners[1] - origin);
    C3DVector bottom = (corners[3] - origin) ^ (corners[2] - origin);
    C3DVector left = (corners[0] - origin) ^ (corners[3] - origin);
    top.AccurateNormalise();
    right.AccurateNormalise();
    bottom.AccurateNormalise();
    left.AccurateNormalise();
    FrustrumPlanes[2].Initialise(top, top.Dot(origin));
    FrustrumPlanes[1].Initialise(right, right.Dot(origin));
    FrustrumPlanes[3].Initialise(bottom, bottom.Dot(origin));
    FrustrumPlanes[0].Initialise(left, left.Dot(origin));
}
