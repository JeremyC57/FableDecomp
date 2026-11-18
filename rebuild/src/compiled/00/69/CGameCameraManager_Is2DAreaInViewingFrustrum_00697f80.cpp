// CGameCameraManager::Is2DAreaInViewingFrustrum(const C3DVector&, float) const -- check successive points on a circle against the viewing frustum
// Leaf: ?Is2DAreaInViewingFrustrum@CGameCameraManager@@UBE_NABVC3DVector@@M@Z
// Retail: 0x00697F80, 490 bytes, virtual const thiscall.
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_CCAMERA_FOV
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_FRUSTRUM_POS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETPOSITION_INLINE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETOBSERVEDPOS_INLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CBase.h"

extern const double TWO_PI;

bool CGameCameraManager::Is2DAreaInViewingFrustrum(const C3DVector& origin, float radius) const
{
    float fov = Camera.GetFOV();
    C3DVector cam_pos = CurrentMode->GetPosition();
    C3DVector cam_look_at = CurrentMode->GetObservedPos();
    // Construct the horizontal basis for sampling the circle.
    C3DVector line = cam_look_at - cam_pos;
    line.Z = 0.0f;
    line.AccurateNormalise();
    C3DVector perp = line.CrossWithUp();
    perp.AccurateNormalise();
    float d_theta = 0.1f;
    for (float theta = 0.0f; theta <= 1.0f; theta += d_theta) {
        C3DVector r = origin + line * (float)cos(theta * TWO_PI) * radius
                            + perp * (float)sin(theta * TWO_PI) * radius;
        if (CamTools.IsPosInViewingFrustrum(r, cam_look_at, cam_pos, fov))
            return true;
    }
    return false;
}