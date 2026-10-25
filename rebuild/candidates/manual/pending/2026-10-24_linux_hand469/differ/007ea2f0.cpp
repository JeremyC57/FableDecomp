// NTargeting::GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float interpolate) -- the thing's first graphic-appearance targeting helper point (four helper names tried in order), else its interpolated physics position raised by a quarter unit; the interpolate is rescaled when bullet time applies to the thing
// Grade: DIFFER(351v411) -- return-block layout: retail keeps a separate copy-to-result + epilogue for the 2nd and 3rd helper hits (differently scheduled dword moves), the 1st/4th hits jump to the shared block the fallback stores into (pos written to its stack slot, then copied); ours merges all four hits into one copy block and forwards the fallback straight into the hidden result (thing ebp, result ebx/interp edi vs retail result edi/interp ebx). The || single-return form (g2) reproduces the fallback temps exactly but merges in registers (339 bytes)
// Leaf: GetTargetingPositionOnThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EA2F0, 411 bytes, `__fastcall` (hidden C3DVector result in ecx, world in edx), `ret 8`.
// Caller: CTCTargeted::GetTargetPoint @ 0x007EA490 (landed; manifest label "Creature::GetTargetFirePoint" is wrong).
// Callees: CThing::IsAffectedByBulletTime @ 0x004C9920, CBulletTime::ConvertInterpolateToBulletTimeInterpolate @ 0x004C6010,
//   CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector*, float) @ 0x004C0560 (x4, index 0,
//   flags 0x20) with the global helper names @ 0x013BAC50, 0x013BAC4C, 0x013BACDC, 0x013BAC48. Constant 0.25f @ 0x0123078C.
// Layout: CWorld bullet time +0x68 (CBulletTime controller +0x04, active byte +0x0C); CThing physics +0x60, graphic appearance +0x64;
//   Ego_r.pdb CTCPhysicsBase Position +0x0C, OldPosition +0x18.
// Shape: one named local C3DVector returned through the implicit copy (dword moves); the interpolate is written back to its
//   own stack slot; the fallback is OldPosition + (Position - OldPosition) * t through by-value operators, then rebuilt with Z + 0.25.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CCharString { void* m_pData; };
extern CCharString g_TargetHelperName;          // 0x013BAC50
extern CCharString g_CameraStickHelperName;     // 0x013BAC4C
extern CCharString g_CCharString_013bacdc;      // 0x013BACDC
extern CCharString g_CCharString_013bac48;      // 0x013BAC48 "focus_eye_01"
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* out, float scale);   // 0x004C0560
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0C
    C3DVector OldPosition;                      // +0x18
    C3DVector GetInterpolatedPosition(float t) const { return (Position - OldPosition) * t + OldPosition; }
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PPhysics;                   // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64 (PDB +0x74)
    bool IsAffectedByBulletTime() const;        // 0x004C9920
};
struct CBulletTime {
    unsigned char _pad0[4];
    void* m_pController;                        // +0x04
    unsigned char _pad1[4];
    bool m_active;                              // +0x0C
    float ConvertInterpolateToBulletTimeInterpolate(float interpolate);   // 0x004C6010
    bool IsActive() const { return m_active || m_pController != 0; }
};
struct CWorld {
    unsigned char _pad[0x68];
    CBulletTime* m_pBulletTime;                 // +0x68
};
namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float interpolate);
}

C3DVector __fastcall NTargeting::GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float interpolate)
{
    if (world.m_pBulletTime->IsActive() && thing.IsAffectedByBulletTime())
        interpolate = world.m_pBulletTime->ConvertInterpolateToBulletTimeInterpolate(interpolate);
    C3DVector pos;
    CTCGraphicAppearance* appearance = thing.PTCGraphicAppearance;
    if (appearance != 0) {
        if (appearance->GetHelperPoint(g_TargetHelperName, 0, 0x20, &pos, interpolate))
            return pos;
        if (appearance->GetHelperPoint(g_CameraStickHelperName, 0, 0x20, &pos, interpolate))
            return pos;
        if (appearance->GetHelperPoint(g_CCharString_013bacdc, 0, 0x20, &pos, interpolate))
            return pos;
        if (appearance->GetHelperPoint(g_CCharString_013bac48, 0, 0x20, &pos, interpolate))
            return pos;
    }
    pos = thing.PPhysics->GetInterpolatedPosition(interpolate) + C3DVector(0.0f, 0.0f, 0.25f);
    return pos;
}
