// CCreatureAction_ProjectileCrouch::FrameUpdate() -- the base frame update; then, when turning to face the target, turn the creature's facing towards FacePos (snap when within ~5 degrees, otherwise step 1/72 turn towards the side the target is on) and set the wrapped facing angle
// Grade: DIFFER(546v546) -- Normalise scale keeps r on the x87 stack for X (`fst [r] ; fmul st(2)`, retail `fstp [r] ; fxch ; fmul [r]`), the physics-TC vcall object lands in eax (retail ecx) and the C3DVector copies load X/Y/Z into other registers; layout and flow match
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
    C3DVector toTarget(FacePos.X - position.X, FacePos.Y - position.Y, FacePos.Z - position.Z);
    toTarget.Normalise();
    C3DVector facing = PCreature.GetPItem()->PhysicsTC->GetFacingVector();
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
