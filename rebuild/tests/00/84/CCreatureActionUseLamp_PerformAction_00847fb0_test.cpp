// Behaviour test for 00847fb0: body copied from src/00847fb0.cpp (CCreatureAction_UseLamp::PerformAction), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};

static int g_setActive = 0; static bool g_activeArg = false; static void* g_activeThis = 0;
struct CTCLight { unsigned char _pad[0x1c]; bool Active; void SetActive(bool active) { ++g_setActive; g_activeArg = active; g_activeThis = this; Active = active; } };
static int g_getPItem = 0;
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { ++g_getPItem; return m_pItem; } };
struct CCreatureActionBase { virtual void v00() {} unsigned char _pad04[0xa8 - 4]; };
struct CCreatureAction_UseLamp : CCreatureActionBase {
    CBaseIntelligentPointer PObjectToUse;
    void PerformAction();
};
void CCreatureAction_UseLamp::PerformAction()
{
    if (PObjectToUse.GetPItem() == 0)
        return;
    if (PObjectToUse.GetPItem()->m_flags91 & 1)
        return;
    CThing* object = PObjectToUse.GetPItem();
    if (object->HasTC(0x27)) {
        CTCLight* light = object->GetTC<CTCLight>(0x27);
        light->SetActive(!light->Active);
    }
}
int main()
{
    CThing lamp; memset(&lamp, 0, sizeof(lamp)); CTCLight light; memset(&light, 0, sizeof(light));
    CInterfaceMap::SEntry entries[2] = { { 0x27, (CTCBase*)&light }, { 0x40, (CTCBase*)0x99 } };
    lamp.m_interfaces.first = entries; lamp.m_interfaces.last = entries + 2;
    CCreatureAction_UseLamp action; action.PObjectToUse.m_pItem = 0;
    // 1. no object: one GetPItem, nothing else
    action.PerformAction(); CHECK(g_getPItem == 1 && g_setActive == 0);
    // 2. dead object
    action.PObjectToUse.m_pItem = &lamp; lamp.m_flags91 = 1; action.PerformAction(); CHECK(g_getPItem == 3 && g_setActive == 0); lamp.m_flags91 = 0;
    // 3. object without a light component
    action.PerformAction(); CHECK(g_getPItem == 6 && g_setActive == 0);
    // 4. light present: toggled off->on, then on->off
    lamp.m_presentBits[0x27 >> 5] = 1UL << (0x27 & 31);
    action.PerformAction(); CHECK(g_setActive == 1 && g_activeArg == true && g_activeThis == &light && light.Active == true);
    action.PerformAction(); CHECK(g_setActive == 2 && g_activeArg == false && light.Active == false);
    if (fails == 0) printf("00847fb0_TEST PASS\n"); else printf("00847fb0_TEST FAIL\n");
    return fails;
}