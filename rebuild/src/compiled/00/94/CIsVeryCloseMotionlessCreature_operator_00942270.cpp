// CIsVeryCloseMotionlessCreature::operator()(CThing* thing) const -- a creature of category 2 whose kind-2 component shows no movement since the last frame (squared step <= 1e-4 squared) and whose ground distance from MyPos is under 1.4 times the sum of its physics radius and MyRadius
// Leaf: ??RCIsVeryCloseMotionlessCreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x00942270, 210 bytes, `ret 4`.
// Callees: the creature category getter @ 0x00662000 (def +0xe0 -> +0xc4; landed callers name it), the const
//   component-map LowerBound @ 0x004365B0, CThing::GetPosition @ 0x006A4D00 (GOTCHAS chunk 182). Globals: 0.0001f @
//   0x0129BA3C (an extern float, squared), the 1.4f pool constant @ 0x01292874.
// Layout: Ego_r.pdb CIsVeryCloseMotionlessCreature {MyPos +0, MyRadius +0x0c}; retail CThing component bitset +0x20
//   (kind 2 = bit 2), map +0x44, physics body +0x60 (radius +0x34); the kind-2 component keeps this frame's position at
//   +0x0c and the previous one at +0x18.
// Shape: the component comes from an accessor that leaves it unset when absent (retail reads the dead argument slot);
//   the step is `(pos - lastPos).GetLengthSquared()` with the landed C3DVector inline operators; the distance is the
//   square root of the 2D sum; the step test is `!(step > eps*eps)` (`test ah,5 ; jnp`, NaN passes); both tests are
//   nested ifs sharing the `xor al,al` exit.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double x);
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};
extern const float g_minLength_0129ba3c;           // 0x0129BA3C (0.0001f)
struct CTCBase_Retail;
struct STCEntry_Retail {
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTCMotion {                                 // component kind 2
    unsigned char _base[0x0c];
    C3DVector     Position;                        // +0x0c
    C3DVector     LastPosition;                    // +0x18
};
struct CPhysicsBody {
    unsigned char _pad00[0x34];
    float         Radius;                          // +0x34
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x10];
    CPhysicsBody*  m_pBody;                        // +0x60
    long GetCreatureCategory() const;              // 0x00662000
    const C3DVector& GetPosition() const;          // 0x006A4D00
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCMotion* GetMotionIfPresent(const CThing* thing)
{
    CTCMotion* tc;                                 // deliberately unset when the component is absent
    if (thing->HasTC(2))
        tc = (CTCMotion*)thing->m_tcs.Find(2);
    return tc;
}
class CIsVeryCloseMotionlessCreature {
public:
    C3DVector MyPos;                               // +0x00
    float     MyRadius;                            // +0x0c
    bool operator()(CThing* thing) const;
};

bool CIsVeryCloseMotionlessCreature::operator()(CThing* thing) const
{
    if (thing->GetCreatureCategory() == 2) {
        CTCMotion* motion = GetMotionIfPresent(thing);
        if (!((motion->Position - motion->LastPosition).GetLengthSquared() > g_minLength_0129ba3c * g_minLength_0129ba3c)) {
            const C3DVector& pos = thing->GetPosition();
            if (C2DVector(pos.X - MyPos.X, pos.Y - MyPos.Y).GetLength() < (thing->m_pBody->Radius + MyRadius) * 1.4f)
                return true;
        }
    }
    return false;
}