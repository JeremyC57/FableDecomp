// Behaviour test for 00658fa2: body copied from src/00658fa2.cpp (CTCVillage::GetNextGuardTypeAndIncrement), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return end() - begin(); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
static int g_destroyed = 0;
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct CDefBase : CDefVirtual {
    long m_count;
    void Release() { if (--m_count == 0) Destroy(); }
};
class CVillagePeopleDef : public CDefBase { public:
    unsigned char _pad_0x08[0x8c];
    long Guard;
    long MaleGhost;
    long FemaleGhost;
    long NumGuards;
    long NumGhosts;
    CRetailVector<long> GuardRespawnType;
    CRetailVector<long> GuardRespawnQuantity;
};
struct CDefPointer {
    CVillagePeopleDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    CVillagePeopleDef* operator->() const { return m_p; }
    operator bool() const { return m_p != 0; }
};
static CVillagePeopleDef* g_people = 0; static int g_getSubDef = 0; static long g_countAtGet = -1;
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_00512fe0(CDefPointer& out)
    {
        ++g_getSubDef; g_countAtGet = m_count;
        if (!g_people) return false;
        ++g_people->m_count; out.m_p = g_people; return true;
    }
};
struct CThingDefPointer {
    CDefClassBase* m_p;
    CThingDefPointer(CDefClassBase* p) : m_p(p) { }
    CThingDefPointer(const CThingDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    ~CThingDefPointer() { if (m_p) m_p->Release(); }
    CDefClassBase* operator->() const { return m_p; }
};
class CThing { public:
    unsigned char    _pad_0x00[0x70];
    CThingDefPointer PDef;
    CThing(CDefClassBase* def) : PDef(def) { }
};
class CTCVillage { public:
    void*         __vftable;
    CThing*       ParentThing;
    unsigned char _pad_0x08[0x9c];
    long          NextGuardRespawn;
    long GetNextGuardTypeAndIncrement();
};

long CTCVillage::GetNextGuardTypeAndIncrement()
{
    long respawn = NextGuardRespawn++;
    CDefPointer def;
    {
        CThingDefPointer thingDef(ParentThing->PDef);
        thingDef->GetSubDef_00512fe0(def);
    }
    if (def && def->GuardRespawnType.size() != 0 && def->GuardRespawnType.size() == def->GuardRespawnQuantity.size()) {
        for (unsigned int i = 0; i < def->GuardRespawnQuantity.size(); ++i) {
            respawn -= def->GuardRespawnQuantity[i];
            if (respawn < 0 || i == def->GuardRespawnQuantity.size() - 1)
                return def->GuardRespawnType[i];
        }
    }
    return def->Guard;
}
int main()
{
    static CDefClassBase classDef; static CVillagePeopleDef people; static CThing thing(&classDef);
    classDef.m_count = 1; people.m_count = 1; people.Guard = 500; g_people = &people;
    long types[3] = { 101, 102, 103 }; long quantities[3] = { 2, 1, 3 };
    people.GuardRespawnType._M_start = types; people.GuardRespawnType._M_finish = types + 3;
    people.GuardRespawnQuantity._M_start = quantities; people.GuardRespawnQuantity._M_finish = quantities + 3;
    CTCVillage v; memset(&v, 0, sizeof(v)); v.ParentThing = &thing; v.NextGuardRespawn = 0;
    // respawn 0,1 -> first type; 2 -> second; 3,4,5 -> third; beyond the table -> last entry
    CHECK(v.GetNextGuardTypeAndIncrement() == 101); CHECK(v.NextGuardRespawn == 1); CHECK(g_getSubDef == 1 && g_countAtGet == 2 && classDef.m_count == 1 && people.m_count == 1);
    CHECK(v.GetNextGuardTypeAndIncrement() == 101);
    CHECK(v.GetNextGuardTypeAndIncrement() == 102);
    CHECK(v.GetNextGuardTypeAndIncrement() == 103); CHECK(v.GetNextGuardTypeAndIncrement() == 103); CHECK(v.GetNextGuardTypeAndIncrement() == 103);
    CHECK(v.GetNextGuardTypeAndIncrement() == 103); CHECK(v.NextGuardRespawn == 7);
    // mismatched table sizes: the plain guard
    people.GuardRespawnQuantity._M_finish = quantities + 2; CHECK(v.GetNextGuardTypeAndIncrement() == 500);
    // empty table: the plain guard
    people.GuardRespawnType._M_finish = types; people.GuardRespawnQuantity._M_finish = quantities; CHECK(v.GetNextGuardTypeAndIncrement() == 500);
    CHECK(g_destroyed == 0);
    // defs held only by the lookup are destroyed
    classDef.m_count = 0; people.m_count = 0; CHECK(v.GetNextGuardTypeAndIncrement() == 500); CHECK(g_destroyed == 2);
    if (fails == 0) printf("00658fa2_TEST PASS\n"); else printf("00658fa2_TEST FAIL\n");
    return fails;
}