// NCameraModes::CProjectileWeapon::GetPosFromParams(float angle, float extension) const -- camera position for the projectile-weapon mode: the observed thing's position plus the XY offset for the angle (scaled) plus the mode's Z axis times the remapped extension
// Grade: DIFFER(228v244) -- dl=11 (try n, retail headers): fixed the unfolded lerp (named t), the rotated local, and the `this` reload (same-TU GetCamPosOffsetXY 0x00878A80 body); left: retail spills position.X and the position+offset X/Y sums to the frame before adding the Z-axis term (ours keeps them on the FPU); named reach/sum, += and reordered sums tried
// Leaf: ?GetPosFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00878BE0, length 244 bytes, `ret 0xc` (C3DVector by hidden pointer).
// Identity: Ego_r.pdb ?GetPosFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z (private const).
// Callees: GetCamPosOffsetXY(float) const 0x00878A80, NCameraModes::CBase::GetObservedThing() const 0x0084F1B0 (Ego_r.pdb
//   ?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ). Floats 0.5f 0x012777F8, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb CProjectileWeapon XAxis/YAxis/ZAxis +0xCD/+0xD9/+0xE5 (packed) = retail +0xA9/+0xB5/+0xC1 (the landed
//   ctor 0x00879620); no engine header for the class (engine/NCameraModes_CBase.h is the slot model only): local model.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/NCameraModes_CBase.h"
// FableWin.pdb ?GFGetInterpolatedValue@@YIMMMM@Z (debug, out of line), inline in retail: start + (end - start) * t.
inline float __fastcall GFGetInterpolatedValue(float start, float end, float t) { return start + (end - start) * t; }

namespace NCameraModes {
#pragma pack(push, 1)
class CProjectileWeapon : public CBase {
public:
    const CThingGameObject* GetObservedThing() const;   // 0x0084F1B0 (CBase member in Ego_r; declared here)
private:
    C3DVector GetCamPosOffsetXY(float angle) const;     // 0x00878A80
    C3DVector GetPosFromParams(float angle, float extension) const;
    unsigned char _pad04[0xa9 - 4];
    C3DVector XAxis;                                    // +0xa9
    C3DVector YAxis;                                    // +0xb5
    C3DVector ZAxis;                                    // +0xc1
};
#pragma pack(pop)
}

// GetCamPosOffsetXY 0x00878A80 (the row before this one in retail's TU; defined here so VC7.1 sees it leaves ecx alone and
// does not reload `this` for the GetObservedThing call): the XY axes rotated by angle turns (2 pi as a double, 0x0124F2B8).
C3DVector NCameraModes::CProjectileWeapon::GetCamPosOffsetXY(float angle) const
{
    double radians = angle * 6.2831854820251465;
    return XAxis * (float)sin(radians) + YAxis * (float)cos(radians);
}

C3DVector NCameraModes::CProjectileWeapon::GetPosFromParams(float angle, float extension) const
{
    extension = extension + extension + 0.5f;
    float t = (extension - 0.5f) * 0.5f;
    float scale = GFGetInterpolatedValue(1.0f, 1.0f, t);
    C3DVector rotated = GetCamPosOffsetXY(angle);
    C3DVector offset = rotated * scale;
    C3DVector position = GetObservedThing()->GetPhysicsTC()->GetPosition();
    return position + offset + ZAxis * extension;
}
