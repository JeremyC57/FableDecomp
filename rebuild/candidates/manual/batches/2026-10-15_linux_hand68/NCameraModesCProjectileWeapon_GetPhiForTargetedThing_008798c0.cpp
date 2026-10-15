// NCameraModes::CProjectileWeapon::GetPhiForTargetedThing(const CThing&) const -- the camera phi angle for the thing's targeting position (full height)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008798C0, length 51 bytes.
// Callees: NCameraModes::CBase::GetWorld() const @ 0x0084D050, NTargeting::GetTargetingPositionOnThing @
// 0x007EA2F0 (`__fastcall`, hidden result in ecx), GetPhiForPosition(const C3DVector&) const @ 0x00879680.
// Shape: as 0x00879560 -- the position in a named local, re-addressed with `lea` for the last call.
struct CWorld;
struct CThing;
struct CGameCameraManager;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};

namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}

namespace NCameraModes {

struct CBase {
    virtual void v00();
    const CWorld& GetWorld() const;                         // 0x0084D050
};

struct CProjectileWeapon : CBase {
    float GetPhiForPosition(const C3DVector& position) const;   // 0x00879680
    float GetPhiForTargetedThing(const CThing& thing) const;
};

float CProjectileWeapon::GetPhiForTargetedThing(const CThing& thing) const
{
    C3DVector position = NTargeting::GetTargetingPositionOnThing(GetWorld(), thing, 1.0f);
    return GetPhiForPosition(position);
}

}
