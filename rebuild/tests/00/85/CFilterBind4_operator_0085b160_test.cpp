// Behaviour test for 0x0085B160 CFilter_Bind4<IsNot, CombatIsStrikeToStrike, CombatOverlappingSphere,
// CombatCanCollideOrInteractWithThing>::operator()<CThingGameObject> (chunk 402): all four filters must pass, in order,
// stopping at the first failure; the sphere filter needs the hit-locations interface (0x7F) and a bounding sphere
// that overlaps the strike sphere (squared centre distance below the squared radius sum).
#include <cstdio>

class CTCBase {
public:
    virtual ~CTCBase() { }
};
struct CTCInterfaceEntry {
    long     type;
    CTCBase* component;
};
class CThing {
public:
    unsigned int      m_tcPresent[9];
    CTCInterfaceEntry m_entries[4];
    int               m_count;
    bool              StrikeToStrike;
    bool              Collidable;
    CThing() : m_count(0), StrikeToStrike(true), Collidable(true)
    {
        for (int i = 0; i < 9; ++i)
            m_tcPresent[i] = 0;
        m_entries[0].type = 0;
        m_entries[0].component = 0;
    }
    virtual ~CThing() { }
    void AddTC(long type, CTCBase* component)
    {
        m_entries[m_count].type = type;
        m_entries[m_count].component = component;
        ++m_count;
        m_entries[m_count].component = 0;
        m_tcPresent[type >> 5] |= 1u << (type & 31);
    }
    unsigned int HasTC(long type) const { return m_tcPresent[type >> 5] & (1u << (type & 31)); }
    template<class T> bool GetInterface(T** ppOut, long type) const
    {
        if (HasTC(type)) {
            const CTCInterfaceEntry* it = m_entries;
            const CTCInterfaceEntry* last = m_entries + m_count;
            while (it != last && it->type < type)
                ++it;
            *ppOut = (T*)((it == last || type < it->type) ? last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingGameObject : public CThing { };
class C3DVector {
public:
    float X, Y, Z;
};
class CTCHitLocations : public CTCBase {
public:
    C3DVector Centre;
    float     Radius;
    void GetBoundingSphere(C3DVector& centre, float& radius, bool flag) const
    {
        (void)flag;
        centre = Centre;
        radius = Radius;
    }
};
static int gCalls[5];
class CThingFilter_IsNot {
public:
    CThing* Thing;
    bool operator()(const CThing& thing) const { ++gCalls[1]; return &thing != Thing; }
};
namespace {
class CThingFilter_CombatIsStrikeToStrike {
public:
    CThing* ThingStriking;
    bool operator()(const CThing& thing) const { ++gCalls[2]; return thing.StrikeToStrike; }
};
class CThingFilter_CombatCanCollideOrInteractWithThing {
public:
    bool operator()(const CThing& thing) const { ++gCalls[4]; return thing.Collidable; }
};
}
class CThingFilter_CombatOverlappingSphere {
public:
    C3DVector SpherePos;
    float     Radius;
    bool operator()(const CThing& thing) const
    {
        ++gCalls[3];
        const CTCHitLocations* hitLocations;
        if (thing.GetInterface(&hitLocations, 0x7f)) {
            C3DVector centre;
            float radius;
            hitLocations->GetBoundingSphere(centre, radius, true);
            float dx = centre.X - SpherePos.X;
            float dy = centre.Y - SpherePos.Y;
            float dz = centre.Z - SpherePos.Z;
            float r = radius + Radius;
            if (dx * dx + dy * dy + dz * dz < r * r)
                return true;
        }
        return false;
    }
};
template <class F1, class F2, class F3, class F4> class CFilter_Bind4 {
public:
    const F1* PF1;
    const F2* PF2;
    const F3* PF3;
    const F4* PF4;
    template <class T> bool operator()(const T& thing) const
    {
        return (*PF1)(thing) && (*PF2)(thing) && (*PF3)(thing) && (*PF4)(thing);
    }
};
typedef CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere,
                      CThingFilter_CombatCanCollideOrInteractWithThing> TStrikeFilter;

static void Reset() { for (int i = 0; i < 5; ++i) gCalls[i] = 0; }

int main()
{
    int failures = 0;
    CThingGameObject striker, target;
    CTCHitLocations hit;
    hit.Centre.X = 3.0f; hit.Centre.Y = 4.0f; hit.Centre.Z = 0.0f; hit.Radius = 1.0f;     // 5 from the origin
    target.AddTC(0x7f, &hit);
    CThingFilter_IsNot isNot = { &striker };
    CThingFilter_CombatIsStrikeToStrike strike = { &striker };
    CThingFilter_CombatOverlappingSphere sphere;
    sphere.SpherePos.X = 0.0f; sphere.SpherePos.Y = 0.0f; sphere.SpherePos.Z = 0.0f; sphere.Radius = 4.5f;   // 5.5 > 5
    CThingFilter_CombatCanCollideOrInteractWithThing collide;
    TStrikeFilter filter = { &isNot, &strike, &sphere, &collide };

    Reset();
    if (!filter(target) || gCalls[4] != 1) { printf("  overlapping target rejected\n"); ++failures; }
    Reset();
    if (filter(striker) || gCalls[2] != 0) { printf("  the striker itself passed\n"); ++failures; }
    target.StrikeToStrike = false;
    Reset();
    if (filter(target) || gCalls[3] != 0) { printf("  non strike-to-strike target passed\n"); ++failures; }
    target.StrikeToStrike = true;
    sphere.Radius = 3.9f;                                   // 4.9 < 5: no overlap
    Reset();
    if (filter(target) || gCalls[4] != 0) { printf("  distant target passed\n"); ++failures; }
    sphere.Radius = 4.0f;                                   // touching (equal) is not overlapping
    if (filter(target)) { printf("  touching target passed\n"); ++failures; }
    sphere.Radius = 4.5f;
    target.Collidable = false;
    if (filter(target)) { printf("  non-collidable target passed\n"); ++failures; }
    CThingGameObject ghost;                                 // no hit locations
    if (filter(ghost)) { printf("  target without hit locations passed\n"); ++failures; }
    if (failures == 0) {
        printf("0085b160_TEST PASS\n");
        return 0;
    }
    printf("0085b160_TEST FAIL\n");
    return 1;
}