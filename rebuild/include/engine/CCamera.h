#ifndef FABLE_ENGINE_CCAMERA_H
#define FABLE_ENGINE_CCAMERA_H
// New header (chunk 718, row 0x00487570 CPlayer::DrawFreeCamDebug). Ego_r.pdb CCamera, sizeof 0x38 (struct_layouts_egor.tsv):
// WorldPosition +0x00, RHSet +0x0C (Up, Forward), HeightLocked +0x24, ViewVecZLocked +0x25, FOVFlags +0x28, HorizontalFOV +0x2C,
// VerticalFOV +0x30, Zoom +0x34; no STL member, so the offsets hold in retail (DrawFreeCamDebug reads CPlayer::CurrentFreeCamera
// +0x40 = WorldPosition and +0x58 = RHSet.Forward). Members: the out-of-line Ego_r.pdb / FableWin.pdb ?GetHorizontalFOV@CCamera@@QBEMXZ
// (retail 0x00A0BE90) and the FableWin.pdb-only inline accessors ?PeekWorldPos@CCamera@@QBEABVC3DVector@@XZ /
// ?PeekViewDirection@CCamera@@QBEABVC3DVector@@XZ (no Ego_r.pdb copy: expanded in the release build).
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
class CCamera {
public:
#ifdef FABLE_RETAIL_CCAMERA_TARGET_CTOR
    CCamera(const C3DVector& world_position,const C3DVector& target_position,float fov,bool is_2d);
#endif
#ifdef FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
    // Retail opt-in (Codex pass 17): original copy ctor/MoveTo/PointAt signatures and debug bodies.
    CCamera(const CCamera&);
    void MoveTo(const C3DVector& p){WorldPosition=p;}
    void PointAt(const C3DVector&);
#endif
    float GetHorizontalFOV() const;                                      // 0x00A0BE90
#ifdef FABLE_RETAIL_CCAMERA_ANGLES
    // Opt-in (chunk 765, rows 0x0069E1B0 / 0x0069E1C0): Ego_r.pdb / FableWin.pdb ?GetAngleXY@CCamera@@QBEMXZ / ?GetAngleYZ@CCamera@@QBEMXZ,
    // out of line in retail at 0x00A0C380 / 0x00A0C3E0 (Ego_r 0x00B4CF00 / 0x00B4CF70). Declarations only.
    float GetAngleXY() const;                                            // 0x00A0C380
    float GetAngleYZ() const;                                            // 0x00A0C3E0
#endif
#ifdef FABLE_RETAIL_CCAMERA_SETVIEW
    // Opt-in (chunk 898, row 0x0084CD10 NCameraModes::SetCameraRoll): Ego_r.pdb ?SetView@CCamera@@QAEXABVC3DVector@@0@Z, out of line in
    // retail at 0x00A0BE30 (the first member of the CCamera TU; called with ecx = the camera, forward then up pushed). Declaration only.
    void SetView(const C3DVector& forward, const C3DVector& up);         // 0x00A0BE30
#endif
#ifdef FABLE_RETAIL_CCAMERA_UP_DIRECTION
    // Retail opt-in (Codex pass18): FableWin PeekUpDirection inline getter and full donor GetProjectileTrailElementFromPoint.
    const C3DVector& PeekUpDirection() const { return RHSet.GetUp(); }
#endif
#ifdef FABLE_RETAIL_CCAMERA_SETANGLEXY
    // Retail opt-in (chunk 1021): Ego_r.pdb ?SetAngleXY@CCamera@@QAEXM@Z (0x00B4D1B0), out of line in retail at 0x00A0C570.
    void SetAngleXY(float angle);                                        // 0x00A0C570
#endif
    const C3DVector& PeekWorldPos() const { return WorldPosition; }
    const C3DVector& PeekViewDirection() const { return RHSet.GetForward(); }
private:
    C3DVector WorldPosition;                                             // +0x00
    CRightHandedSet RHSet;                                               // +0x0C
    bool HeightLocked;                                                   // +0x24
    bool ViewVecZLocked;                                                 // +0x25
    unsigned long FOVFlags;                                              // +0x28
    float HorizontalFOV;                                                 // +0x2C
    float VerticalFOV;                                                   // +0x30
    float Zoom;                                                          // +0x34
};
#endif
