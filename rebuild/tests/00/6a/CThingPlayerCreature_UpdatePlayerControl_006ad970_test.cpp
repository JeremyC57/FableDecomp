#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCScriptedControl {
    bool m_forceUpdate; bool m_running; int m_updates;
    bool IsScriptTaskRunning() { return m_running; }
    void Update() { ++m_updates; }
};
struct CThingPlayerCreature : CThing {
    int m_moves;
    void ControlledMove() { ++m_moves; }
    void UpdatePlayerControl();
};
void CThingPlayerCreature::UpdatePlayerControl()
{
    CTCScriptedControl* control;
    if (GetInterface(&control, 0x1f))
    {
        if (control->m_forceUpdate || !control->IsScriptTaskRunning())
            control->Update();
    }
    ControlledMove();
}
int main()
{
    int bad = 0, i;
    CTCScriptedControl tc; tc.m_forceUpdate = false; tc.m_running = true; tc.m_updates = 0;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 3; entries[0].component = 0;
    entries[1].type = 0x1f; entries[1].component = (CTCBase*)&tc;
    entries[2].type = 0x40; entries[2].component = 0;
    CThingPlayerCreature c; c.m_moves = 0;
    for (i = 0; i < 9; ++i) c.m_presentBits[i] = 0;
    c.m_interfaces.first = entries; c.m_interfaces.last = entries + 3;
    c.UpdatePlayerControl();
    if (c.m_moves != 1 || tc.m_updates != 0) bad = 1;
    c.m_presentBits[0] = 0x80000000UL;
    c.UpdatePlayerControl();
    if (c.m_moves != 2 || tc.m_updates != 0) bad = 2;          /* running, not forced */
    tc.m_running = false;
    c.UpdatePlayerControl();
    if (c.m_moves != 3 || tc.m_updates != 1) bad = 3;
    tc.m_running = true; tc.m_forceUpdate = true;
    c.UpdatePlayerControl();
    if (c.m_moves != 4 || tc.m_updates != 2) bad = 4;
    if (bad) { std::printf("006ad970_TEST FAIL\n"); return 1; }
    std::printf("006ad970_TEST PASS\n");
    return 0;
}