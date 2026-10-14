#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x006CDA50: exercises the same logic through a copy of the model (compiled separately from the source)
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

struct C2DVector {
    float x, y;
    C2DVector(const C3DVector& v) : x(v.x), y(v.y) {}
    C2DVector(const C2DVector& o) : x(o.x), y(o.y) {}
    C2DVector* Normalise() { float l = x * x + y * y; float s = 1.0f; while (s * s > l * 1.0001f || s * s < l * 0.9999f) s = 0.5f * (s + l / s); x /= s; y /= s; return this; }
};
struct M : CGroundNavigatorExternalInterface {
    C2DVector PeekNavigatorOrientation() const { C2DVector direction(Creature->m_pBody->GetFacing()); direction.Normalise(); return direction; }
};
static bool Run()
{
    CPhysicsBody_Retail body; body.m_facing = C3DVector(3.0f, 4.0f, 9.0f);
    CThingCreatureBase creature; creature.m_pBody = &body;
    M m; m.Creature = &creature;
    C2DVector d = m.PeekNavigatorOrientation();
    return near(d.x, 0.6f) && near(d.y, 0.8f) && near(body.m_facing.x, 3.0f);
}
int main()
{
    if (!Run()) { std::printf("006cda50_TEST FAIL\n"); return 1; }
    std::printf("006cda50_TEST PASS\n");
    return 0;
}