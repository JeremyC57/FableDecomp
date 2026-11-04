// NCameraModes::CProjectileWeapon::GetPhiForTargetedThing(const CThing&) const -- the camera phi angle for the thing's targeting position (full height)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008798C0, length 51 bytes.
// Callees: NCameraModes::CBase::GetWorld() const @ 0x0084D050, NTargeting::GetTargetingPositionOnThing @
// 0x007EA2F0 (`__fastcall`, hidden result in ecx), GetPhiForPosition(const C3DVector&) const @ 0x00879680.
// Shape: as 0x00879560 -- the position in a named local, re-addressed with `lea` for the last call.
class CWorld;
class CThing;
class CGameCameraManager;

#include "engine/C3DVector.h"

namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}

namespace NCameraModes {

class CBase { public:
    virtual void v00();
    const CWorld& GetWorld() const;                         // 0x0084D050
};

class CProjectileWeapon : public CBase { public:
    float GetPhiForPosition(const C3DVector& position) const;   // 0x00879680
    float GetPhiForTargetedThing(const CThing& thing) const;
};

float CProjectileWeapon::GetPhiForTargetedThing(const CThing& thing) const
{
    C3DVector position = NTargeting::GetTargetingPositionOnThing(GetWorld(), thing, 1.0f);
    return GetPhiForPosition(position);
}

}