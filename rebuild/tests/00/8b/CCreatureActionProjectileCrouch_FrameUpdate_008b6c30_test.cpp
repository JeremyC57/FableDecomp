#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B6C30, length 546 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCombatActionBase::FrameUpdate @ 0x00859D80, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CThing::GetPosition @ 0x004C73D0, C3DVector::RotateAboutZ(float turns) @ 0x00A14040, _CIfmod @ 0x00BFEB28;
//          the physics TC (+0x60) slot 70 (+0x118, the facing vector by hidden pointer) and slot 66 (+0x108, set the
//          XY facing angle).
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f, 0.5f, cos(5 deg) 0.9962f 0x0127A54C, 0.0f,
//          1/(2 pi) 0x01237F08, 1.0f, double 1.0; the step 1/72 turn (0x3C638E39) and the quarter turn (0.25f) immediates.
// Layout: Ego_r.pdb CCreatureAction_ProjectileCrouch TurnToFaceTarget +0x12c / FacePos +0x12d sit at +0x114 / +0x115 in
//   retail (CCombatActionBase is 0x114 bytes in retail, PDB 0x12c); the generated header keeps the PDB offsets.
// Shape: the direction to FacePos is a C3DVector built from the differences and normalised with the inline fast
//   Normalise (landed CTCBattleCharge::UpdateDirectionOverride 0x007A7000 model, squares summed Z, Y, X); the dot
//   products sum Z first; the facing angle is atan2(X, Y) in turns wrapped by the landed WrapAngle.
#include <math.h>
#pragma intrinsic(fmod, atan2)
extern unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558

__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(Z * Z + Y * Y + X * X); }
    float Dot(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
    void RotateAboutZ(float turns);                         // 0x00A14040
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCPhysicsBase {
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void SetFacingAngleXY(float turns);             // slot 66 (+0x108)
    virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacingVector();                    // slot 70 (+0x118)
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                               // 0x00A01B50
};

#pragma pack(push, 1)
struct CCombatActionBase {
    virtual void FrameUpdate();                             // 0x00859D80
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                      // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};
struct CCreatureAction_ProjectileCrouch : CCombatActionBase {
    bool      TurnToFaceTarget;                             // +0x114 (PDB +0x12c)
    C3DVector FacePos;                                      // +0x115 (PDB +0x12d)
    virtual void FrameUpdate();
};
#pragma pack(pop)

void CCreatureAction_ProjectileCrouch::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    if (!TurnToFaceTarget)
        return;
    const C3DVector& position = PCreature.GetPItem()->GetPosition();
    C3DVector toTarget = FacePos - position;
    toTarget.Normalise();
    C3DVector facing = PCreature.GetPItem()->GetPhysicsTC()->GetFacingVector();
    C3DVector desired = facing;
    if (facing.Dot(toTarget) < 0.9962f) {
        C3DVector side = facing;
        side.RotateAboutZ(0.25f);
        if (side.Dot(toTarget) > 0.0f)
            desired.RotateAboutZ(0.013888889f);
        else
            desired.RotateAboutZ(-0.013888889f);
    } else
        desired = toTarget;
    CTCPhysicsBase* physics = PCreature.GetPItem()->PhysicsTC;
    float angle = WrapAngle((float)atan2(desired.X, desired.Y) * 0.15915494f);
    physics->SetFacingAngleXY(angle);
}

// ---- test harness ----
unsigned long g_InvSqrtMantissaTable[128];
static void BuildInvSqrtTable()
{
    for (int i = 0; i < 128; ++i) {
        int e = 126 + (i >> 6);
        double x = ldexp(1.0 + ((i & 63) + 0.5) / 64.0, e - 127);
        double y = 1.0 / sqrt(x);
        int re = (0x17c - e) >> 1;
        double m = y / ldexp(1.0, re - 127) - 1.0;
        long t = (long)(m * 256.0 + 0.5);
        if (t < 0) t = 0;
        if (t > 255) t = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)t;
    }
}
static const double kTwoPi = 6.283185307179586;
void C3DVector::RotateAboutZ(float turns)
{
    double a = turns * kTwoPi, c = cos(a), s = sin(a);
    float x = (float)(X * c - Y * s), y = (float)(X * s + Y * c);
    X = x; Y = y;
}
static int g_baseCalls, g_setCalls;
static float g_setAngle;
static C3DVector g_facing(0.0f, 1.0f, 0.0f);
static C3DVector g_pos(0.0f, 0.0f, 0.0f);
#define DEF10(n) void CTCPhysicsBase::v##n##0() {} void CTCPhysicsBase::v##n##1() {} void CTCPhysicsBase::v##n##2() {} \
    void CTCPhysicsBase::v##n##3() {} void CTCPhysicsBase::v##n##4() {} void CTCPhysicsBase::v##n##5() {} \
    void CTCPhysicsBase::v##n##6() {} void CTCPhysicsBase::v##n##7() {} void CTCPhysicsBase::v##n##8() {} void CTCPhysicsBase::v##n##9() {}
DEF10(0) DEF10(1) DEF10(2) DEF10(3) DEF10(4) DEF10(5)
void CTCPhysicsBase::v60() {} void CTCPhysicsBase::v61() {} void CTCPhysicsBase::v62() {} void CTCPhysicsBase::v63() {}
void CTCPhysicsBase::v64() {} void CTCPhysicsBase::v65() {} void CTCPhysicsBase::v67() {} void CTCPhysicsBase::v68() {}
void CTCPhysicsBase::v69() {}
void CTCPhysicsBase::SetFacingAngleXY(float turns) { ++g_setCalls; g_setAngle = turns; }
C3DVector CTCPhysicsBase::GetFacingVector() { return g_facing; }
static CTCPhysicsBase g_physics;
static CThing g_thing;
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
const C3DVector& CThing::GetPosition() const { return g_pos; }
void CCombatActionBase::FrameUpdate() { ++g_baseCalls; }
static int g_fail;
static void Check(int ok, const char* what) { if (!ok) { ++g_fail; printf("FAIL %s\n", what); } }
static float Expected(C3DVector d)
{
    float a = (float)(atan2(d.X, d.Y) / kTwoPi);
    if (a < 0.0f) a += 1.0f;
    return a;
}
static int Near(float a, float b) { float d = a - b; return d < 1e-3f && d > -1e-3f; }
int main()
{
    BuildInvSqrtTable();
    g_thing.PhysicsTC = &g_physics;
    static CCreatureAction_ProjectileCrouch action;
    action.PCreature.m_pItem = &g_thing;
    action.TurnToFaceTarget = false;
    action.FrameUpdate();
    Check(g_baseCalls == 1 && g_setCalls == 0, "no turn");
    action.TurnToFaceTarget = true;
    // target straight ahead -> snap to the target direction
    action.FacePos = C3DVector(0.0f, 10.0f, 0.0f);
    action.FrameUpdate();
    Check(g_setCalls == 1 && Near(g_setAngle, 0.0f), "snap ahead");
    // target slightly off (within 5 degrees) -> snap: direction (1, 30, 0)
    g_pos = C3DVector(1.0f, 2.0f, 0.0f);
    action.FacePos = C3DVector(2.0f, 32.0f, 0.0f);
    action.FrameUpdate();
    Check(Near(g_setAngle, Expected(C3DVector(1.0f, 30.0f, 0.0f))), "snap near");
    // target on +X: step 1/72 turn one way
    g_pos = C3DVector(0.0f, 0.0f, 0.0f);
    action.FacePos = C3DVector(10.0f, 0.0f, 0.0f);
    action.FrameUpdate();
    C3DVector side = g_facing; side.RotateAboutZ(0.25f);
    C3DVector stepped = g_facing; stepped.RotateAboutZ(side.X * 1.0f > 0.0f ? 0.013888889f : -0.013888889f);
    Check(Near(g_setAngle, Expected(stepped)), "step towards +X");
    float firstStep = g_setAngle;
    // target on -X: the other way
    action.FacePos = C3DVector(-10.0f, 0.0f, 0.0f);
    action.FrameUpdate();
    C3DVector stepped2 = g_facing; stepped2.RotateAboutZ(side.X * -1.0f > 0.0f ? 0.013888889f : -0.013888889f);
    Check(Near(g_setAngle, Expected(stepped2)), "step towards -X");
    Check(!Near(firstStep, g_setAngle), "steps differ");
    Check(g_baseCalls == 5 && g_setCalls == 4, "call counts");
    printf(g_fail ? "008b6c30_TEST FAIL\n" : "008b6c30_TEST PASS\n");
    return g_fail;
}