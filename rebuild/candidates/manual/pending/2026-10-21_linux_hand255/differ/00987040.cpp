// Grade: DIFFER(370v374) -- type, distance (squares as named locals), faction and fast-normalise match; the x87 schedule of the scaled-component dot (retail scales x,y,z first and pops r last) and the true/false return block order differ (tried in-place Normalise + Dot both orders, GetNormalised, by-value Dot, explicit locals, if/||/bool-local returns)
// CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::operator()(const CThingAICreature& creature) const -- accept an AI creature of our creature type, strictly inside the squared search distance, allied with our faction and (when the angle is restricted) whose normalised offset lies within the minimum cosine of our side vector
// Leaf: ??RCThingFilter_IsAICreatureMatchingTypeAndFactionNearby
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987040, length 372 bytes (+12 int3), `ret 4`, bool in al.
//   Signature from Ego_r.pdb `??RCThingFilter_IsAICreatureMatchingTypeAndFactionNearby@@QBE_NABVCThingAICreature@@@Z`.
// Callees: CThingCreatureBase::GetCreatureType() const @ 0x00662000, CThing::GetPosition() const @ 0x004C73D0, the const
//   CVectorMap LowerBound copy @ 0x004365B0 (component 0x49 CTCEnemy), CTCEnemy::IsAlliedWithFaction(const CFaction*)
//   const @ 0x0076C7F0. Globals: the reciprocal-square-root mantissa table @ 0x013CE558, 3.0f @ 0x0122DED4,
//   0.5f @ 0x0122F59C (C3DVector's inline fast Normalise, the landed 0x007A6E20 model).
// Layout: Ego_r.pdb {RestrictAngle +0, OurPos +1, MaxSqDist +0x10, SideVec +0x14, OurCreatureType +0x20, PFaction +0x24,
//   MinCosine +0x28}: OurPos at +1, so this TU's C3DVector is byte-aligned (pack(1)).
// Shape: the offset is a named vector normalised in place (VC keeps the scaled components on the x87 stack for the dot);
//   the final test is `return dot > MinCosine` (eax-wide 1/0 shared with the unrestricted `return true`).
extern unsigned long g_InvSqrtMantissaTable[128];       // 0x013CE558

#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
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
#pragma pack(pop)
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CTCBase;
struct CFaction;
struct CTCEnemy {
    bool IsAlliedWithFaction(const CFaction* faction) const;   // 0x0076C7F0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline const T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (const T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    long GetCreatureType() const;                       // 0x00662000
};
struct CThingAICreature : CThingCreatureBase { };

class CThingFilter_IsAICreatureMatchingTypeAndFactionNearby {
public:
    bool      RestrictAngle;                            // +0x00
    C3DVector OurPos;                                   // +0x01
    float     MaxSqDist;                                // +0x10
    C3DVector SideVec;                                  // +0x14
    long      OurCreatureType;                          // +0x20 ECreatureType
    CFaction* PFaction;                                 // +0x24
    float     MinCosine;                                // +0x28
    bool operator()(const CThingAICreature& creature) const;
};

bool CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::operator()(const CThingAICreature& creature) const
{
    if (creature.GetCreatureType() == OurCreatureType) {
        C3DVector offset = creature.GetPosition() - OurPos;
        float x2 = offset.X * offset.X;
        float y2 = offset.Y * offset.Y;
        float z2 = offset.Z * offset.Z;
        if (x2 + y2 + z2 < MaxSqDist && creature.HasTC(0x49)
            && creature.GetTC<CTCEnemy>(0x49)->IsAlliedWithFaction(PFaction)) {
            if (RestrictAngle)
                offset.Normalise();
            return !RestrictAngle || Dot(offset, SideVec) > MinCosine;
        }
    }
    return false;
}
