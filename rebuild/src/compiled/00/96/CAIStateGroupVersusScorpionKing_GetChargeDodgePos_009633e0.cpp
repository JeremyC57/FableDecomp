// CAIStateGroup_VersusScorpionKing::GetChargeDodgePos(C3DVector& pos) -- start from the creature's position; while the scorpion is alive, step 10 units sideways (a quarter turn left or right of the scorpion's normalised facing, whichever side the creature is facing towards)
// Leaf: GetChargeDodgePos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009633E0, length 469 bytes (`ret 4`, the position by reference).
// Callees: CThing::GetPosition @ 0x004C73D0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3 on PEnemy), the physics
//          body's virtual GetFacing (slot 70, +0x118, C3DVector by hidden pointer, on thing+0x60; twice),
//          C3DVector::RotateXY(float turns) @ 0x00A14040 (-0.25 / +0.25).
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC,
//          10.0f 0x01296374.
// Layout: engine/CAIStateGroup_VersusScorpionKing.h (packed: GoClockwise +0x1c, PArenaCentreThing +0x1d) with the flattened
//          base modelled as the landed CAIStateGroupBase and PEnemy (+0x14); CThing physics body +0x60, dead bit +0x91.
// Model: the landed CTCAssassinRush::SetupFacingDirection 0x007A6E20 inline fast Normalise (exponent (380 - e) / 2, table
//          mantissa, one Newton step) and Dot.
// Shape: C3DVector needs a DECLARED memberwise copy ctor (the two rotated copies of the facing then take x straight from
//          the x87 stack and the estimate's integer ops schedule like retail; with the implicit copy every word goes
//          through memory); the side choice is an if/else of `pos += side * 10.0f` (VC merges the z tail).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
class C3DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float k) const { return C3DVector(X * k, Y * k, Z * k); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    void RotateXY(float turns);                 // 0x00A14040
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
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CPhysicsBody {                           // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
class CThing { public:
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
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
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
    void GetChargeDodgePos(C3DVector& pos);
};
#pragma pack(pop)

void CAIStateGroup_VersusScorpionKing::GetChargeDodgePos(C3DVector& pos)
{
    pos = PBrain->PCreature->GetPosition();
    if (IS_THING_ALIVE(PEnemy)) {
        C3DVector facing = PEnemy.GetPItem()->GetPhysicsBody()->GetFacing();
        facing.Normalise();
        C3DVector left = facing;
        left.RotateXY(-0.25f);
        C3DVector right = facing;
        right.RotateXY(0.25f);
        if (Dot(left, PBrain->PCreature->GetPhysicsBody()->GetFacing()) > 0.0f)
            pos += left * 10.0f;
        else
            pos += right * 10.0f;
    }
}