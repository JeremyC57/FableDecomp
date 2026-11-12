// CGameCameraManager::GetInterpolatedCamera(float, const CCamera&, const CCamera&) const -- the render camera between two game frames:
// a copy of the current camera whose field of view, world position, up vector and view direction are blended from the old camera
// towards the current one by the interpolation factor.
// Leaf: ?GetInterpolatedCamera@CGameCameraManager@@ABE?AVCCamera@@MABV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006978D0, length 376 bytes; private const __fastcall, `ret 10h` (hidden CCamera result
// pointer + the float + two const CCamera&); `this` is never read. Ego_r.pdb 0x006BE4E0 / FableWin.pdb 0x02202AE0, same mangled name.
// Only caller: CGameCameraManager::GetRenderCamera 0x00697F30, as GetInterpolatedCamera(interpolate, Camera, OldCamera).
// Callees: CCamera copy constructor 0x0049D380 (the local and the result: no NRVO), CCamera::GetFOV 0x00A0BEB0 (twice, the current
// camera first = right-to-left argument evaluation), CCamera::SetFOV(float, float) 0x00A0BED0, CCamera::SetView(forward, up) 0x00A0BE30.
// Inline: GFGetInterpolatedValue(old, new, interp) = (new - old) * interp + old (FableWin.pdb ?GFGetInterpolatedValue@@YIMMMM@Z),
// CCamera::PeekWorldPos / PeekUpDirection / PeekViewDirection / MoveTo, the C3DVector copy constructor.
// Shape: the position goes through a C3DVector that MoveTo copies into the local camera (`fstp tmp ; mov eax,tmp ; mov [cam+N],eax`),
// exactly like the sibling CGameCameraBase::GetRenderCamera 0x006FD5E0. The up and view vectors of both cameras are first copied BY
// VALUE into locals (retail moves old.Z, new.Y and new.Z through integer registers into frame slots and keeps old.X / old.Y on the
// x87 stack), then blended per component into `up` (first) and `forward`, the two SetView arguments. Every GFGetInterpolatedValue
// call is parenthesised as in the sibling row: without the parentheses VC7.1 emits the same instructions but schedules the integer
// copies differently (the store into the camera lands before the `fstp st(0)`, the forward copies after `fstp up.Z`).
// The parameter and local names are descriptive (the PDBs carry no local names); passcam / vecman follow the sibling row.
// Flags: none
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_INTERPOLATEDCAMERA
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCAMERA_SETVIEW
#define FABLE_RETAIL_CCAMERA_UP_DIRECTION
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_CCAMERA_FOV
#include "engine/GFGetInterpolatedValue.h"
#include "engine/CGameCameraManager.h"
CCamera CGameCameraManager::GetInterpolatedCamera(const float interpolate, const CCamera& camera, const CCamera& old_camera) const
{
    CCamera passcam(camera);
    passcam.SetFOV(GFGetInterpolatedValue(old_camera.GetFOV(), camera.GetFOV(), interpolate), 0.0f);
    C3DVector vecman;
    vecman.X = (GFGetInterpolatedValue(old_camera.PeekWorldPos().X, camera.PeekWorldPos().X, interpolate));
    vecman.Y = (GFGetInterpolatedValue(old_camera.PeekWorldPos().Y, camera.PeekWorldPos().Y, interpolate));
    vecman.Z = (GFGetInterpolatedValue(old_camera.PeekWorldPos().Z, camera.PeekWorldPos().Z, interpolate));
    passcam.MoveTo(vecman);
    const C3DVector old_up = old_camera.PeekUpDirection();
    const C3DVector new_up = camera.PeekUpDirection();
    C3DVector up;
    up.X = (GFGetInterpolatedValue(old_up.X, new_up.X, interpolate));
    up.Y = (GFGetInterpolatedValue(old_up.Y, new_up.Y, interpolate));
    up.Z = (GFGetInterpolatedValue(old_up.Z, new_up.Z, interpolate));
    const C3DVector old_forward = old_camera.PeekViewDirection();
    const C3DVector new_forward = camera.PeekViewDirection();
    C3DVector forward;
    forward.X = (GFGetInterpolatedValue(old_forward.X, new_forward.X, interpolate));
    forward.Y = (GFGetInterpolatedValue(old_forward.Y, new_forward.Y, interpolate));
    forward.Z = (GFGetInterpolatedValue(old_forward.Z, new_forward.Z, interpolate));
    passcam.SetView(forward, up);
    return passcam;
}