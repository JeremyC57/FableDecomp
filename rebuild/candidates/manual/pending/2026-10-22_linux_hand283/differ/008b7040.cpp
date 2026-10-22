// CCreatureAction_JumpOutFromCover::Initialise() -- normalise the jump direction; with a live combat target, pick
// Grade: DIFFER(770v770) -- only the target / &PCreature ebp/ebx register swap remains (the second Normalise matches via *this *= FastInvSqrt(lenSq) with operator*=(const float&))
// JUMP_OUT_RIGHT (direction turned -1/4) when the direction points along the target-to-the-left side (the normalised
// creature-to-target vector turned +1/4), else JUMP_OUT_LEFT (turned +1/4), and face the creature along the turned
// direction (atan2 as a 0..1 turn fraction wrapped with fmod); then the base Initialise
// Leaf: ?Initialise@CCreatureAction_JumpOutFromCover
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7040, length 784 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0, C3DVector::RotateAboutZ
//   0x00A14040, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CCombatActionBase::SetAnimationName 0x008574D0 / Initialise
//   0x00857520, _CIfmod 0x00BFEB28, the physics component's slot 66 (+0x108).
// Globals: reciprocal-square-root mantissa table 0x013CE558, 3.0f, 0.5f, 1/(2pi), 0.0f, 1.0f, double 1.0.
// Layout: retail CCombatActionBase is 0x114 bytes (PDB 0x18 larger): JumpDirection at +0x114 (Ego_r.pdb +0x12c);
//   PTarget +0xa8; CThing physics +0x60, flags +0x91 (bit 0 dead). Model: the landed DuckAndCover::Initialise 0x008B6EE0.
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(atan2, fmod)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X;
    float Y;
    float Z;
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void NormaliseD()
    {
        float x = X;
        float y = Y;
        float z = Z;
        *this *= FastInvSqrt(x * x + y * y + z * z);
    }
    __forceinline void NormaliseE()
    {
        *this *= FastInvSqrt(X * X + Y * Y + Z * Z);
    }
    __forceinline void NormaliseB()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        *this *= (3.0f - r * r * lengthSquared) * r * 0.5f;
    }
    __forceinline void NormaliseC()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        *this *= r;
    }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Dot(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
    void RotateAboutZ(float turns);                  // 0x00A14040
};
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetFacingAngle(float fraction);   // slot 66 (+0x108)
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;        // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_JumpOutFromCover : CCombatActionBase {
    C3DVector JumpDirection;                // +0x114
    virtual void Initialise();
};

void CCreatureAction_JumpOutFromCover::Initialise()
{
    JumpDirection.Normalise();
    if (IS_THING_ALIVE(PTarget)) {
        CThing* target = PTarget.GetPItem();
        C3DVector toTarget = target->GetPosition() - GetCreature()->GetPosition();
        toTarget.NormaliseE();
        C3DVector side = toTarget;
        side.RotateAboutZ(0.25f);
        C3DVector direction;
        if (side.Dot(JumpDirection) > 0.0f) {
            direction = JumpDirection;
            direction.RotateAboutZ(-0.25f);
            SetAnimationName("JUMP_OUT_RIGHT");
        } else {
            direction = JumpDirection;
            direction.RotateAboutZ(0.25f);
            SetAnimationName("JUMP_OUT_LEFT");
        }
        CTCPhysicsBase* physics = GetCreature()->m_pPhysics;
        float fraction = (float)atan2(direction.X, direction.Y) * 0.15915493667125702f;
        if (fraction < 0.0f || fraction >= 1.0f) {
            fraction = (float)fmod(fraction, 1.0);
            if (fraction < 0.0f)
                fraction += 1.0f;
        }
        physics->SetFacingAngle(fraction);
    }
    CCombatActionBase::Initialise();
}
