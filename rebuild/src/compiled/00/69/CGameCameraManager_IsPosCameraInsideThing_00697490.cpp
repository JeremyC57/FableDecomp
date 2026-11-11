// CGameCameraManager::IsPosCameraInsideThing(const CThing& thing, const C3DVector& pos) const -- model the thing as a capsule from its position up past its right eye dummy (1.2x the eye height) with 1.8x its collision radius, and report whether the camera position lies inside it
// Leaf: ?IsPosCameraInsideThing@CGameCameraManager@@QBE_NABVCThing@@ABVC3DVector@@@Z
// Shape: `axis *= (1.0f / height)` -- the redundant parentheses add a no-code PAREN tuple that lets the pos-argument load
//   (`mov eax,[esp+5Ch]`, 0x0069759C) schedule before the last normalised-axis store, as retail; unparenthesised it lands after.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00697490, 473 bytes, `ret 8`.
// Callees: ??0CCharString@@QAE@PBDJ@Z 0x0099EBF0 / ~CCharString 0x0099EAE0, NGraphicAppearance::CTCGraphicAppearance::GetDummyObject
//   0x004C08C0, CThing::GetPos 0x004C73D0; inline CThing::GetTCGraphicAppearance (+0x64) / GetPhysicsTC (+0x60) and
//   CTCPhysicsBase::GetRadius (+0x34); constants 1.2f 0x0125A690, 1.8f 0x0125C288, 1.0f 0x0122DED8 and the const epsilon
//   g_NormaliseEpsilon 0x0129BA3C (0.0001f; landed spelling ?g_NormaliseEpsilon@@3MB).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CGAMECAMERAMANAGER_INSIDETHING
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CGameCameraManager.h"

extern const float g_NormaliseEpsilon;                   // 0x0129BA3C (0.0001f)
// Descriptive (no PDB name found): the open-interval test of the projection.
static inline bool InRange(float value, float low, float high) { return value > low && value < high; }

bool CGameCameraManager::IsPosCameraInsideThing(const CThing& thing, const C3DVector& pos) const
{
    C3DVector eye;
    CRightHandedSet orientation;
    thing.GetTCGraphicAppearance()->GetDummyObject("EYE_SET_R", 0, 0, eye, orientation, 1.0f);
    C3DVector base = thing.GetPos();
    C3DVector top = base + (eye - base) * 1.2f;
    float radius = thing.GetPhysicsTC()->GetRadius() * 1.8f;
    C3DVector axis = top - base;
    float height = axis.GetMagnitude();
    axis *= (1.0f / height);
    C3DVector offset = pos - base;
    double along = offset.Dot(axis);
    if (InRange(along, -g_NormaliseEpsilon, height)) {
        C3DVector perp = offset - axis * (float)along;
        if (perp.GetSquaredMagnitude() < radius * radius)
            return true;
    }
    return false;
}