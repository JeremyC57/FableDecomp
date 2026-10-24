// Behaviour test for 0x0059525D NUISystem::CFrontEndManager::ClearComponents (model copy)
#include <cstdio>
static int g_deleted = 0;
struct Component { virtual ~Component() { ++g_deleted; } };
struct Stack {
    Component* items[8]; int count;
    bool empty() const { return count == 0; }
    void pop() { --count; }
};
struct MapEntry { int screen; Component* comp; };
struct Map {
    MapEntry entries[8]; int count; int clears;
    MapEntry* begin() { return entries; }
    MapEntry* end() { return entries + count; }
    void clear() { count = 0; ++clears; }
};
struct FrontEndManager {
    Stack m_stackScreens;
    Map m_mapScreens;
    void ClearComponents()
    {
        while (!m_stackScreens.empty()) m_stackScreens.pop();
        for (MapEntry* it = m_mapScreens.begin(); it != m_mapScreens.end(); ++it) delete it->comp;
        m_mapScreens.clear();
    }
};
int main()
{
    FrontEndManager m;
    Component* a = new Component; Component* b = new Component; Component* c = new Component;
    m.m_stackScreens.count = 0;
    m.m_stackScreens.items[m.m_stackScreens.count++] = a; m.m_stackScreens.items[m.m_stackScreens.count++] = b;
    m.m_mapScreens.count = 0; m.m_mapScreens.clears = 0;
    m.m_mapScreens.entries[0].screen = 1; m.m_mapScreens.entries[0].comp = a;
    m.m_mapScreens.entries[1].screen = 2; m.m_mapScreens.entries[1].comp = 0;      // a null value is skipped by delete
    m.m_mapScreens.entries[2].screen = 3; m.m_mapScreens.entries[2].comp = b;
    m.m_mapScreens.entries[3].screen = 4; m.m_mapScreens.entries[3].comp = c;
    m.m_mapScreens.count = 4;
    m.ClearComponents();
    if (!m.m_stackScreens.empty() || g_deleted != 3 || m.m_mapScreens.count != 0 || m.m_mapScreens.clears != 1) { std::printf("0059525d_TEST FAIL\n"); return 1; }
    m.ClearComponents();
    if (g_deleted != 3 || m.m_mapScreens.clears != 2) { std::printf("0059525d_TEST FAIL\n"); return 1; }
    std::printf("0059525d_TEST PASS\n");
    return 0;
}