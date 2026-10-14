#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x006CDAB0: exercises the same logic through a copy of the model (compiled separately from the source)
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
struct C3DVector { float x, y, z; C3DVector() {} C3DVector(float a, float b, float c) : x(a), y(b), z(c) {} C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {} };
struct CTCBase_Retail { long m_kind; };
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst; STCEntry_Retail* m_pLast; STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const long* kind) const   // first entry whose kind is not less than *kind (the map is sorted)
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
struct CThingCreatureBase {
    unsigned long m_presentBits[9]; CTCMap_Retail m_tcs; CPhysicsBody_Retail* m_pBody; CDefClassBase_Retail* PhysicsTC; unsigned char m_dead; float m_maxSpeed[4];
    virtual float GetMaxSpeed(long mode) { return m_maxSpeed[mode]; }
    template<class T> void GetInterface(T** ppOut, long kind) const
    {
        if (m_presentBits[kind >> 5] & (1UL << (kind & 31))) {
            long type = kind;
            const CTCMap_Retail* map = &m_tcs;
            STCEntry_Retail* it = map->LowerBound(&type);
            if (it == map->m_pLast || kind < it->m_kind)
                it = map->m_pLast;
            *ppOut = (T*)it->m_pTC;
        }
    }
};
struct CGroundNavigatorExternalInterface { void* __vftable; CThingCreatureBase* Creature; float Radius; };

struct M : CGroundNavigatorExternalInterface { C3DVector PeekNavigatorPosition3D() const { return Creature->m_pBody->m_position; } };
static bool Run()
{
    CPhysicsBody_Retail body; body.m_position = C3DVector(1.5f, -2.5f, 7.0f);
    CThingCreatureBase creature; creature.m_pBody = &body;
    M m; m.Creature = &creature;
    C3DVector p = m.PeekNavigatorPosition3D();
    return near(p.x, 1.5f) && near(p.y, -2.5f) && near(p.z, 7.0f);
}
int main()
{
    if (!Run()) { std::printf("006cdab0_TEST FAIL\n"); return 1; }
    std::printf("006cdab0_TEST PASS\n");
    return 0;
}