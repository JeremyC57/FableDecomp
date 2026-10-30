// CCreatureAction_MidAttackSide::Initialise() -- pick the sideways strike animation: "STRIKE_MEDIUM_RIGHT_90" when the normalised direction to the target lies on the right of the creature's facing (positive dot with the facing's right-hand perpendicular), else "STRIKE_MEDIUM_LEFT_90"; then the base Initialise
// Grade: DIFFER(342v344) nd 25->17 -- lever: the Newton step with r *= 0.5f as its own statement (008E5D40 form) fixes the shr/and order of the rsqrt estimate. Residue: the Y product is fld st(1) ; fmul [Y] (retail fld [Y] ; fmul st(2), r at the stack bottom) and the dead-value pops before fnstsw. Tried the full 008E5D40 sum order (21), X = x * r and r * x forms (17). Next: keep r in memory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B6AD0, length 344 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8 first, PCreature at +8), CThing::GetPosition @ 0x004C73D0,
// the physics component's virtual slot 70 (+0x118, C3DVector GetFacing by hidden pointer), CCharString ctor/dtor @ 0x0099EBF0 /
// 0x0099EAE0, CCombatActionBase::SetAnimationName @ 0x008574D0, CCombatActionBase::Initialise @ 0x00857520.
// Globals: the 128-entry reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC.
// Shape: `toTarget = target->GetPosition() - creature->GetPosition()` (a free inline operator-: the right operand's call runs
// first), the physics through the inline GetPhysics accessor (straight into ecx), the landed inline fast Normalise (the
// write-backs fold into the dot's operands since toTarget dies there), the inline Dot with (facing.Y, -facing.X, 0) keeps the
// `* 0` term; the animation name is an if/else on the dot (`test ah,41h ; jne` = not greater) with the SetAnimationName call merged, each literal converting implicitly.
// Retail CCombatActionBase is 0x114 bytes: PCreature +0x08, PTarget +0xa8.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
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
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* m_pPhysics;                   // +0x60
    CPhysicsBody* GetPhysics() const { return m_pPhysics; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];          // +0x10
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char _padb0[0x114 - 0xb0];         // +0xb0
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct CCreatureAction_MidAttackSide : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_MidAttackSide::Initialise()
{
    CThing* target = PTarget.GetPItem();
    C3DVector toTarget = target->GetPosition() - PCreature.GetPItem()->GetPosition();
    C3DVector facing = PCreature.GetPItem()->GetPhysics()->GetFacing();
    toTarget.Normalise();
    if (Dot(toTarget, C3DVector(facing.Y, -facing.X, 0.0f)) > 0.0f)
        SetAnimationName("STRIKE_MEDIUM_RIGHT_90");
    else
        SetAnimationName("STRIKE_MEDIUM_LEFT_90");
    CCombatActionBase::Initialise();
}
