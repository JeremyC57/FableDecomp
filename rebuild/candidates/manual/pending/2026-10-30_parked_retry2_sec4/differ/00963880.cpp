// CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe() -- whether the scorpion king is charging along a line that passes close to this creature (normalised charge direction vs the vector to me)
// Grade: DIFFER(467v463) nd=20 (was 30) -- lever: Normalise in the 008E5D40 form (y*y; += z*z; += x*x; r *= 0.5f as its own statement). Tried: r*=0.5 alone (30), named mantissa (30), sum orders z,x,y (24) / x,y,z (28) / y,x,z (32). Residue: the toMe sum term order (retail fld st(1) vs ours st(0)), the second estimate's and ecx schedule, and the final scale (retail fmul [r]; fstp st(1)). Next: give each Normalise its own sum order by writing toMe's normalise inline at the call.
// Leaf: IsScorpionChargingAtMe
// Retail: Fable.exe (TLC, VC7.1) @ 0x00963880; best of the probes graded under VC7.1 RTM (probe/00963880b.cpp).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = y * y;
        lengthSquared += z * z;
        lengthSquared += x * x;
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
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CPhysicsBody {                           // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;                  // +0x60
    CPhysicsBody* GetPhysicsBody() const { return PhysicsBody; }
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};
#pragma pack(push, 1)
struct CAIStateGroup_VersusScorpionKing : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
    bool GoClockwise;                           // +0x1c
    CBaseIntelligentPointer PArenaCentreThing;  // +0x1d
    bool IsScorpionCharging() const;            // 0x00963810
    bool IsScorpionChargingAtMe();
};
#pragma pack(pop)

bool CAIStateGroup_VersusScorpionKing::IsScorpionChargingAtMe()
{
    if (IS_THING_ALIVE(PEnemy) && IsScorpionCharging()) {
        C3DVector facing = PEnemy.GetPItem()->GetPhysicsBody()->GetFacing();
        CThing* creature = PBrain->PCreature;
        C3DVector toMe = creature->GetPosition() - PEnemy.GetPItem()->GetPosition();
        facing.Normalise();
        toMe.Normalise();
        if (Dot(facing, toMe) > 0.707f)
            return true;
    }
    return false;
}
