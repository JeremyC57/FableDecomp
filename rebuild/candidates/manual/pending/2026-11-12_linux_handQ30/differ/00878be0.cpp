// NCameraModes::CProjectileWeapon::GetPosFromParams(float phi, float height_param) const -- the projectile-weapon camera position
// Grade: DIFFER(228v244) -- everything matches except the final sum: retail spills pos.X (fstp [esp+28]), loads pos.Z then pos.X, stores (pos+offset).X and .Y to memory and keeps .Z on the FPU before adding ZAxis*height; ours adds pos.X+offset.X straight off the FPU and keeps S.Y/S.Z on the stack. Tried: reference vs by-value pos (by-value needed), C3DVECTOR_ADD_XY_GROUP (worse), /Op (worse). Next: named reach local / `C3DVector result = pos + offset; return result + reach;` / `pos += offset; pos += reach` / `offset + pos + reach`.
// for an orbit angle and height parameter.
// Leaf: ?GetPosFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00878BE0, 244 bytes (`ret 0Ch`).
#include "fable_compiler.h"
#include <math.h>
#pragma intrinsic(cos, sin)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CPHYSICSLEAF_CYLINDER
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_SINGLECUT
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CPhysicsLeaf.h"
#include "engine/CGameCameraManager.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/GFGetInterpolatedValue.h"
namespace NCameraModes {
#pragma pack(push, 1)                           // genuine: Ego_r.pdb CProjectileWeapon XAxis sits at the odd offset +0xcd
class CProjectileWeapon : public CBase {
private:
    C3DVector GetCamPosOffsetXY(float fraction) const;          // 0x00878A80
    C3DVector GetPosFromParams(float phi, float height_param) const;
    long ShotFiredTimer;                                        // +0xa4
    bool Finished;                                              // +0xa8
    C3DVector XAxis;                                            // +0xa9
    C3DVector YAxis;                                            // +0xb5
    C3DVector ZAxis;                                            // +0xc1
};
#pragma pack(pop)

static const float TWO_PI = 6.2831855f;
// The landed 0x00878A80 body (same retail TU, out of line): visible to the compiler, so it knows ecx survives the call.
#pragma auto_inline(off)
C3DVector CProjectileWeapon::GetCamPosOffsetXY(float fraction) const
{
    double angle = fraction * (double)TWO_PI;
    return XAxis * (float)sin(angle) + YAxis * (float)cos(angle);
}
#pragma auto_inline(on)

C3DVector CProjectileWeapon::GetPosFromParams(float phi, float height_param) const
{
    height_param = height_param * 2.0f + 0.5f;
    float t = (height_param - 0.5f) / 2.0f;
    float scale = GFGetInterpolatedValue(1.0f, 1.0f, t);
    C3DVector cam_offset = GetCamPosOffsetXY(phi);
    C3DVector offset = cam_offset * scale;
    C3DVector pos = GetObservedThing()->GetPhysicsTC()->GetPosition();
    return pos + offset + ZAxis * height_param;
}
}
