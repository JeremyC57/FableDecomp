// NCameraModes::CProjectileWeapon::GetPosFromParams(float phi, float height_param) const -- the projectile-weapon camera position
// Shape: height_param is rescaled in place (h*2 + 0.5); the orbit distance is GFGetInterpolatedValue(1.0f, 1.0f, (h - 0.5f) / 2.0f)
//   (retail keeps `* 0.0 + 1.0`); the XY offset is the landed GetCamPosOffsetXY(phi) scaled by it; the observed thing's physics
//   position is copied by value, then DIVIDED BY 1.0f in the return expression (C3DVector::operator/(float), PDB ??KC3DVector@@QBE?BV0@M@Z:
//   reciprocal 1.0f/1.0f folds late, so no instruction is left but the quotient stays a separate value: retail homes pos.X at
//   [esp+28h] (0x00878C90), reloads it after pos.Z (0x00878C94/98) and stores (pos + offset).X/.Y at [esp+1Ch]/[esp+20h] before adding
//   ZAxis * height_param). No plain copy spelling keeps that stage (VC7.1 propagates every copy); the unit divisor is the only
//   spelling found, so the constant's source name is unknown.
// Inferred: the unit divisor (owner rule 2026-11-12: an inferred statement may land when the row behaves exactly as retail --
//   dividing by 1.0f cannot change the position). Graded alternatives (coordinator, 2026-11-12): plain `pos`, `pos * 1.0f`,
//   `pos * K` (const float K = 1.0f) and `pos * (float)(1.0 / 1.0)` all give 228v244 (a float multiply by one is removed at once);
//   per-component `(float)` / `(float)(double)` casts give 234v244. Three spellings MATCH: `pos / 1.0f`, `pos / K` with a named
//   `const float K = 1.0f`, and per-component `(float)(pos.X * 1.0)` (a double product converted back to float). The value must pass
//   through arithmetic that only folds late (operator/'s `float reciprocal = 1.0f / v`) and be stored as float. Lionhead most likely
//   divided by a named unit-scale constant that is 1.0f in this build; its name is unknown, so the literal is kept.
// Callees: GetCamPosOffsetXY 0x00878A80 (landed; its real body is out of line in this TU so c2 knows ecx survives),
//   NCameraModes::CBase::GetObservedThing 0x0084F1B0.
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
    return pos / 1.0f + offset + ZAxis * height_param;
}
}