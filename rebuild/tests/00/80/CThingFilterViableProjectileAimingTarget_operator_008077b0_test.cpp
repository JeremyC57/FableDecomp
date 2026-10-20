// Behaviour test for 008077b0: body copied from src/008077b0.cpp (CThingFilter_ViableProjectileAimingTarget::operator()), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CThing;
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CCreatureSubDef : CDefPointeeBase {
    unsigned char _pad08[0xe4 - 0x08];
    bool IsGhost;
};
static CCreatureSubDef* g_subDef = 0; static int g_getSubDef = 0; static long g_countAtGet = -1;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureSubDef>* out);
};
bool CDefClassBase::GetSubDef(CDefPointer<CCreatureSubDef>* out)
{
    ++g_getSubDef; g_countAtGet = m_refCount;
    if (!g_subDef) return false;
    ++g_subDef->m_refCount; out->Object = g_subDef; return true;
}
struct CThingBase {
    unsigned char _pad00[0x10];
    unsigned char m_class;
    unsigned char _pad11[0x70 - 0x11];
    CDefClassBase* PDef;
};
struct CThingFilter_ViableProjectileAimingTarget {
    bool operator()(CThing* thing) const;
};

bool CThingFilter_ViableProjectileAimingTarget::operator()(CThing* pthing) const
{
    CThingBase* thing = (CThingBase*)pthing;
    if (thing->m_class == 0x0d)
        return false;
    CDefClassBase* thingDef = thing->PDef;
    CDefPointer<CCreatureSubDef> def;
    if (thingDef)
        ++thingDef->m_refCount;
    bool got = thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    if (got && def->IsGhost)
        return false;
    return true;
}
int main()
{
    static CThingBase thing; static CDefClassBase classDef; static CCreatureSubDef subDef;
    memset(&thing, 0, sizeof(thing)); thing.PDef = &classDef; classDef.m_refCount = 1; subDef.m_refCount = 1;
    CThingFilter_ViableProjectileAimingTarget f;
    // class 0x0d is rejected without any lookup
    thing.m_class = 0x0d; CHECK(f((CThing*)&thing) == false); CHECK(g_getSubDef == 0);
    // no creature sub-def: accepted
    thing.m_class = 3; g_subDef = 0; CHECK(f((CThing*)&thing) == true); CHECK(g_getSubDef == 1 && g_countAtGet == 2); CHECK(classDef.m_refCount == 1 && g_destroyed == 0);
    // sub-def present, not a ghost: accepted; the sub-def reference taken by the lookup is released
    g_subDef = &subDef; subDef.IsGhost = false; CHECK(f((CThing*)&thing) == true); CHECK(subDef.m_refCount == 1 && g_destroyed == 0);
    // ghost: rejected
    subDef.IsGhost = true; CHECK(f((CThing*)&thing) == false); CHECK(subDef.m_refCount == 1 && g_getSubDef == 3);
    // a class def and sub-def held only by the lookup are destroyed
    classDef.m_refCount = 0; subDef.m_refCount = 0; CHECK(f((CThing*)&thing) == false); CHECK(g_destroyed == 2);
    if (fails == 0) printf("008077b0_TEST PASS\n"); else printf("008077b0_TEST FAIL\n");
    return fails;
}