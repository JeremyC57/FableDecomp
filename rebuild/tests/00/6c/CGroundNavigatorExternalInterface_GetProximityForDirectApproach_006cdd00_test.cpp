#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x006CDD00: exercises the same logic through a copy of the model (compiled separately from the source)
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
class C3DVector { public: float x, y, z; C3DVector() {} C3DVector(float a, float b, float c) : x(a), y(b), z(c) {} C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {} };
struct CTCBase_Retail { long m_kind; };
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst; STCEntry_Retail* m_pLast; STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound_004365b0(const long* kind) const   // first entry whose kind is not less than *kind (the map is sorted)
    {
        STCEntry_Retail* it = m_pFirst;
        while (it != m_pLast && it->m_kind < *kind) ++it;
        return it;
    }
};
struct CPhysicsBody_Retail {
    C3DVector m_facing; C3DVector m_position; float m_radius;
    virtual C3DVector GetFacing() { return m_facing; }
};
struct CTCCreatureNavigation_Retail { CTCBase_Retail m_base; unsigned char m_obstacles[0x18]; float m_proximityForDirectApproach; };
struct CDefClassBase_Retail;
class CThingCreatureBase { public:
    unsigned long m_presentBits[9]; CTCMap_Retail m_tcs; CPhysicsBody_Retail* m_pBody; CDefClassBase_Retail* PhysicsTC; unsigned char m_dead; float m_maxSpeed[4];
    virtual float GetMaxSpeed(long mode) { return m_maxSpeed[mode]; }
    template<class T> void GetInterface(T** ppOut, long kind) const
    {
        if (m_presentBits[kind >> 5] & (1UL << (kind & 31))) {
            long type = kind;
            const CTCMap_Retail* map = &m_tcs;
            STCEntry_Retail* it = map->LowerBound_004365b0(&type);
            if (it == map->m_pLast || kind < it->m_kind)
                it = map->m_pLast;
            *ppOut = (T*)it->m_pTC;
        }
    }
};
class CGroundNavigatorExternalInterface { public: void* __vftable; CThingCreatureBase* Creature; float Radius; };
static STCEntry_Retail g_entries[3];
static CTCBase_Retail g_tc3, g_tc9;
static CTCCreatureNavigation_Retail g_nav;
static void Setup(CThingCreatureBase& creature, bool present)
{
    std::memset(&creature, 0, sizeof(creature));
    g_tc3.m_kind = 3; g_tc9.m_kind = 9; g_nav.m_base.m_kind = 7; g_nav.m_proximityForDirectApproach = 2.75f;
    g_entries[0].m_kind = 3; g_entries[0].m_pTC = &g_tc3;
    g_entries[1].m_kind = 7; g_entries[1].m_pTC = &g_nav.m_base;
    g_entries[2].m_kind = 9; g_entries[2].m_pTC = &g_tc9;
    creature.m_tcs.m_pFirst = g_entries; creature.m_tcs.m_pLast = g_entries + 3; creature.m_tcs.m_pEnd = g_entries + 3;
    creature.m_presentBits[0] = (1UL << 3) | (1UL << 9) | (present ? (1UL << 7) : 0);
}

struct M : CGroundNavigatorExternalInterface {
    float GetProximityForDirectApproach() const { CTCCreatureNavigation_Retail* navigation = 0; Creature->GetInterface(&navigation, 7); return navigation ? navigation->m_proximityForDirectApproach : -1.0f; }
};
static bool Run()
{
    CThingCreatureBase creature; M m; m.Creature = &creature;
    Setup(creature, true);
    if (!near(m.GetProximityForDirectApproach(), 2.75f)) return false;
    Setup(creature, false);
    return near(m.GetProximityForDirectApproach(), -1.0f);
}
int main()
{
    if (!Run()) { std::printf("006cdd00_TEST FAIL\n"); return 1; }
    std::printf("006cdd00_TEST PASS\n");
    return 0;
}