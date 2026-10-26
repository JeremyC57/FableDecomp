// Behaviour test for 0x006AEBF0 CTCRegionFollower::IsCreatureAllowedInRegionAt (copy of the source logic; spy defs and world map).
#include <cstdio>
static __inline bool DecRefToZero(long& count) { return --count == 0; }
class C3DVector { public: float X, Y, Z; };
static int g_destroyed = 0;
struct CDefPointee {
    long m_refCount;
    CDefPointee() : m_refCount(1) {}
    virtual ~CDefPointee() {}
    virtual void Destroy() { ++g_destroyed; }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) { Object = other.Object; if (Object) { long& count = Object->m_refCount; count = count + 1; } }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); Object = 0; }
            Object = other.Object;
            if (Object) ++Object->m_refCount;
        }
        return *this;
    }
};
class CCreatureDef : public CDefPointee { public: bool AllowedInProtectedTowns; };
static CCreatureDef* g_subDef = 0;
class CDefClassBase : public CDefPointee { public:
    bool GetSubDef(CDefPointer<CCreatureDef>* out)
    {
        if (!g_subDef) return false;
        CDefPointer<CCreatureDef> r; r.Object = g_subDef; ++g_subDef->m_refCount;
        *out = r;
        return true;
    }
};
class CRegionDef : public CDefPointee { public: bool ProtectedTown; };
class CRegion { public: CDefPointer<CRegionDef> PDef; };
static CRegion g_regions[2];
static long g_lastMapQueryX = -1;
class CWorldMap { public:
    virtual ~CWorldMap() {}
    virtual long GetMapNumberAt(const C3DVector& position) const { g_lastMapQueryX = (long)position.X; return position.X > 50.0f ? 7 : 3; }
    long GetRegionNumberMapIsIn(long mapNumber) const { return mapNumber == 7 ? 1 : 0; }
    const CRegion& GetRegion(long region) const { return g_regions[region]; }
};
static CWorldMap g_worldMap;
class CThing {
public:
    CDefPointer<CDefClassBase> PDef;
    CDefPointer<CDefClassBase> GetPDef() const { return PDef; }
};
class CTCRegionFollower { public:
    const CWorldMap& PeekWorldMap() const { return g_worldMap; }
    bool IsCreatureAllowedInRegionAt(const CThing* creature, const C3DVector& position) const;
};
bool CTCRegionFollower::IsCreatureAllowedInRegionAt(const CThing* creature, const C3DVector& position) const
{
    bool hasDef = creature->GetPDef().IsValid();
    if (hasDef) {
        CDefPointer<CCreatureDef> creatureDef;
        bool ok = creature->GetPDef()->GetSubDef(&creatureDef);
        if (ok && !creatureDef->AllowedInProtectedTowns) {
            const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(position)));
            CDefPointer<CRegionDef> regionDef = region.PDef;
            if (regionDef.IsValid() && regionDef->ProtectedTown)
                return false;
        }
    }
    return true;
}
static int fail() { std::printf("006aebf0_TEST FAIL\n"); return 1; }
int main()
{
    CTCRegionFollower follower;
    CThing noDef;
    C3DVector town = { 100.0f, 0.0f, 0.0f }, field = { 10.0f, 0.0f, 0.0f };
    if (!follower.IsCreatureAllowedInRegionAt(&noDef, town)) return fail();       // no def: allowed
    CDefClassBase thingDef; CThing creature; creature.PDef.Object = &thingDef; ++thingDef.m_refCount;
    CCreatureDef creatureDef; creatureDef.AllowedInProtectedTowns = false;
    CRegionDef townDef; townDef.ProtectedTown = true; g_regions[1].PDef.Object = &townDef; ++townDef.m_refCount;
    CRegionDef fieldDef; fieldDef.ProtectedTown = false; g_regions[0].PDef.Object = &fieldDef; ++fieldDef.m_refCount;
    g_subDef = 0;
    if (!follower.IsCreatureAllowedInRegionAt(&creature, town)) return fail();     // no creature sub-def: allowed
    g_subDef = &creatureDef;
    if (follower.IsCreatureAllowedInRegionAt(&creature, town)) return fail();      // protected town, not allowed
    if (g_lastMapQueryX != 100) return fail();
    if (!follower.IsCreatureAllowedInRegionAt(&creature, field)) return fail();    // ordinary region
    creatureDef.AllowedInProtectedTowns = true;
    if (!follower.IsCreatureAllowedInRegionAt(&creature, town)) return fail();     // def allows towns
    creatureDef.AllowedInProtectedTowns = false;
    g_regions[1].PDef.Object = 0;
    if (!follower.IsCreatureAllowedInRegionAt(&creature, town)) return fail();     // region without def
    // reference counts balanced: each def back to its own count, nothing destroyed
    if (thingDef.m_refCount != 2 || creatureDef.m_refCount != 1 || townDef.m_refCount != 2 || fieldDef.m_refCount != 2 || g_destroyed != 0)
        return fail();
    g_regions[0].PDef.Object = 0; creature.PDef.Object = 0;
    std::printf("006aebf0_TEST PASS\n");
    return 0;
}