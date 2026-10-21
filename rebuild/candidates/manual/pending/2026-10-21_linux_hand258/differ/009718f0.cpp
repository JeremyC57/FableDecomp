// CAIStateGroup_JackOfBladesBase::GetIsEnemyInJacksWay(const C3DVector&, const C3DVector&) -- true when the live target thing is nearer (in XY) to the destination than the start is, and lies within 15 degrees of the start-to-destination direction as seen from the destination
// Grade: DIFFER(510v510) -- x87 ordering only: retail squares the path's Y before its X (so the XY sum and the Normalise length reuse them in the other stack order), and pops esi before the second length sum; ours squares X first and pops esi after the table lookup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009718F0, length 508 bytes (+ int3 padding), `ret 8`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTargetThing, three times), CThing::GetPosition @ 0x004C73D0.
// Globals: cos(15 degrees) 0.965923f 0x012975F4, 3.0f 0x0122DED4, 0.5f 0x0122F59C, the reciprocal-square-root mantissa table
// 0x013CE558 (C3DVector's inline fast Normalise, landed 0x007A6E20 / 0x006702E0 models).
// Prototype: FableWin.pdb ?GetIsEnemyInJacksWay@CAIStateGroup_JackOfBladesBase@@IAE_NABVC3DVector@@0@Z.
// Layout: CAIStateGroup_JackOfBladesBase from Ego_r.pdb (PTargetThing +0x24); CThing Dead = bit 0 of +0x91 (retail offset).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquaredXY() const { return X * X + Y * Y; }
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

struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: Dead
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }

struct CAIStateGroup_JackOfBladesBase {
    virtual void v00();
    unsigned char _pad04[0x24 - 4];             // base group, PSwordEmitter, PArenaCentre
    CBaseIntelligentPointer PTargetThing;       // +0x24 CIntelligentPointer<CThingCreatureBase>
protected:
    bool GetIsEnemyInJacksWay(const C3DVector& from, const C3DVector& to);
};

bool CAIStateGroup_JackOfBladesBase::GetIsEnemyInJacksWay(const C3DVector& from, const C3DVector& to)
{
    if (!IsThingAlive(PTargetThing))
        return false;
    C3DVector path = to - from;
    C3DVector fromEnemy = to - PTargetThing.GetPItem()->GetPosition();
    float enemyDistanceSquared = fromEnemy.GetLengthSquaredXY();
    float pathDistanceSquared = path.GetLengthSquaredXY();
    path.Normalise();
    fromEnemy.Normalise();
    if (enemyDistanceSquared < pathDistanceSquared && Dot(fromEnemy, path) > 0.965923f)
        return true;
    return false;
}
