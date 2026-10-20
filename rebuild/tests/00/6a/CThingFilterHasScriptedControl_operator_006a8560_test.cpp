// Behaviour test for 006a8560: body copied from src/006a8560.cpp (CThingFilter_HasScriptedControl::operator()), the callees stubbed.
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
static int g_emptyCalls = 0;
struct CTCScriptedControl {
    unsigned char _pad[0x18];
    bool Locked;
    bool listEmpty;
    bool IsTaskListEmpty() { ++g_emptyCalls; return listEmpty; }
    bool IsActive() { return Locked || !IsTaskListEmpty(); }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool hasScript;
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};
static int g_scriptCalls = 0;
bool __fastcall GFHasScriptThing(CThing* thing) { ++g_scriptCalls; return thing->hasScript; }
struct CThingFilter_HasScriptedControl {
    bool CheckForActive;
    bool operator()(CThing* thing) const;
};
bool CThingFilter_HasScriptedControl::operator()(CThing* thing) const
{
    if (GFHasScriptThing(thing) && thing->HasTC(0x1f)) {
        if (!CheckForActive || thing->GetTC<CTCScriptedControl>(0x1f)->IsActive())
            return true;
    }
    return false;
}
int main()
{
    CThing t; memset(&t, 0, sizeof(t)); CTCScriptedControl sc; memset(&sc, 0, sizeof(sc));
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x1f, (CTCBase*)&sc } };
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    CThingFilter_HasScriptedControl f; f.CheckForActive = false;
    // 1. no script thing: false
    CHECK(f(&t) == false); CHECK(g_scriptCalls == 1);
    // 2. script thing without the component: false
    t.hasScript = true; CHECK(f(&t) == false);
    // 3. component present, not checking activity: true, task list not consulted
    t.m_presentBits[0x1f >> 5] = 1UL << (0x1f & 31); CHECK(f(&t) == true); CHECK(g_emptyCalls == 0);
    // 4. checking activity: locked -> true (list not consulted); unlocked + empty list -> false; unlocked + tasks -> true
    f.CheckForActive = true; sc.Locked = true; CHECK(f(&t) == true); CHECK(g_emptyCalls == 0);
    sc.Locked = false; sc.listEmpty = true; CHECK(f(&t) == false); CHECK(g_emptyCalls == 1);
    sc.listEmpty = false; CHECK(f(&t) == true); CHECK(g_emptyCalls == 2);
    if (fails == 0) printf("006a8560_TEST PASS\n"); else printf("006a8560_TEST FAIL\n");
    return fails;
}