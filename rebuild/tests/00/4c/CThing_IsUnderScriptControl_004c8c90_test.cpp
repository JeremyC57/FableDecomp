// Behaviour test for 004c8c90: body copied from src/004c8c90.cpp (CThing::IsUnderScriptControl), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

class CTCBase;
static int g_taskRunning = 0; static bool g_taskResult = false;
struct CTCScriptControl {
    unsigned char _pad00[0x18];
    bool          m_underControl;
    bool IsTaskRunning() { ++g_taskRunning; return g_taskResult; }
    bool IsUnderControl() { return m_underControl || !IsTaskRunning(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    void*               __vftable;
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];
    CInterfaceMap       Components;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    bool IsUnderScriptControl();
};
bool CThing::IsUnderScriptControl()
{
    if (HasTC(0x1f)) {
        CTCScriptControl* control = GetTC<CTCScriptControl>(0x1f);
        return control->IsUnderControl();
    }
    return false;
}
int main()
{
    CThing thing; memset(&thing, 0, sizeof(thing));
    CTCScriptControl control; memset(&control, 0, sizeof(control));
    CInterfaceMap::SEntry entries[3] = { { 0x10, (CTCBase*)0x99 }, { 0x1f, (CTCBase*)&control }, { 0x40, (CTCBase*)0x99 } };
    thing.Components.first = entries; thing.Components.last = entries + 3;
    // 1. no script control component: false without touching anything
    CHECK(thing.IsUnderScriptControl() == false && g_taskRunning == 0);
    // 2. component present, under control flag set: true without asking the task
    thing.UsingComponents[0] = 1UL << 0x1f; control.m_underControl = true;
    CHECK(thing.IsUnderScriptControl() == true && g_taskRunning == 0);
    // 3. flag clear: the answer is "no task running"
    control.m_underControl = false; g_taskResult = true;
    CHECK(thing.IsUnderScriptControl() == false && g_taskRunning == 1);
    g_taskResult = false;
    CHECK(thing.IsUnderScriptControl() == true && g_taskRunning == 2);
    if (fails == 0) printf("004c8c90_TEST PASS\n"); else printf("004c8c90_TEST FAIL\n");
    return fails;
}