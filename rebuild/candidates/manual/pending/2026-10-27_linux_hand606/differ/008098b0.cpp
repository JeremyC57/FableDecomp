// CThingCompare_BestForTargeting2::operator()(const CTargetPoint& a, const CTargetPoint& b) const -- strict-weak "a is the better target": each point's vector score times its thing's multiplier, a wins when it beats b by more than 1e-4
// Grade: DIFFER(183v183) -- 1 instruction swap: the first score's fstp after mov ecx,esi (retail fstp first, after push edx). This pass: valueB named local (worse, 187), compare object reference local, commuted multiplier operands: no change. Lane gap: two-call float comparator fstp/argument scheduling (EXTRA_RULES)
// Earlier grade: DIFFER(183v183) -- wave 2 sec3: PDB spellings applied (operator() const over NPlayerTargeting::CTargetPoint -- the parked QAE / global CTargetPoint leaf was wrong; GetCompareValueForPoint QBE; GetThingCompareMultiplier private ABE const CThing*); residue unchanged: the first score's fstp after mov ecx,esi (retail before). Tried: unnamed score expression, PDB const signatures
// Leaf: ??RCThingCompare_BestForTargeting2@@QBE_NABVCTargetPoint@NPlayerTargeting@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008098B0, length 183 bytes, `ret 8`, bool in al.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThingCompare_BestForTargeting2::GetThingCompareMultiplier
//   @ 0x007F9E70, CVectorCompare_BestForTargeting::GetCompareValueForPoint(const C3DVector&) @ 0x007F9660.
// Global: g_epsilon_0129ba3c (1e-4f, an extern const float, `fcomp dword ptr [0x0129BA3C]`).
// Identity: engine_api.tsv gives operator()(CTargetPoint*, CTargetPoint*); the 20 callers are the STLport sort helpers
//   over the target-point vector (__median 0x0080A140, partition 0x0080A1B0, insertion 0x0080A340, ...).
// Layout: CTargetPoint (Ego_r.pdb, 28 bytes, matches retail): PThing (CIntelligentPointer<CThing>) +0x00, Index +0x08,
//   TargetingPosition +0x0c, FrameRejected +0x18. CThingCompare_BestForTargeting2: Hero +0x00, VectorCompare +0x04,
//   EnemyPreferenceFactor +0x30.
// Shape: both things are fetched before either multiplier (the second thing parks in the dead `a` slot), the
//   multipliers park in the dead argument slots; each targeting position is copied into a frame C3DVector (a
//   memberwise copy constructor: one register per word, a's words loaded first) and handed to the out-of-line
//   point score through `&VectorCompare` formed once; `x > eps` with separate `mov al,1` / `xor al,al` exits.
struct CBaseObject;
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObject* m_pItem;
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetTypedItem() const { return (T*)GetPItem(); }
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace NPlayerTargeting {
class CTargetPoint {
public:
    CIntelligentPointer<CThing> PThing;                 // +0x00
    long Index;                                         // +0x08
    C3DVector TargetingPosition;                        // +0x0c
    long FrameRejected;                                 // +0x18
};
}
using NPlayerTargeting::CTargetPoint;

class CVectorCompare_BestForTargeting {
public:
    unsigned char _body[0x2c];                          // AnchorPos, PreferredLine, the four compare floats, CompareInXYPlane
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660 (PDB QBE)
};

extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

class CThingCompare_BestForTargeting2 {
public:
    CThing* Hero;                                       // +0x00
    CVectorCompare_BestForTargeting VectorCompare;      // +0x04
    float EnemyPreferenceFactor;                        // +0x30
    bool operator()(const CTargetPoint& a, const CTargetPoint& b) const;
private:
    float GetThingCompareMultiplier(const CThing* thing) const;   // 0x007F9E70 (PDB private ABE)
};

bool CThingCompare_BestForTargeting2::operator()(const CTargetPoint& a, const CTargetPoint& b) const
{
    CThing* thingA = a.PThing.GetTypedItem();
    CThing* thingB = b.PThing.GetTypedItem();
    float multiplierA = GetThingCompareMultiplier(thingA);
    float multiplierB = GetThingCompareMultiplier(thingB);
    C3DVector posA(a.TargetingPosition);
    C3DVector posB(b.TargetingPosition);
    float valueA = VectorCompare.GetCompareValueForPoint(posA) * multiplierA;
    if (valueA - VectorCompare.GetCompareValueForPoint(posB) * multiplierB > g_epsilon_0129ba3c)
        return true;
    return false;
}
