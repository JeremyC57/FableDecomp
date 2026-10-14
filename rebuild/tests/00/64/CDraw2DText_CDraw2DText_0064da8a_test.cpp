#include <cstdio>
static int g_baseCtors = 0, g_listCtors = 0; static void* g_lastGui = 0; static void* g_lastDraws = 0;
struct CPlayerGui;
struct CRetailAllocator { CRetailAllocator() {} };
struct CDrawBase { virtual void v00() {} unsigned char _pad_0x04[0x24]; CDrawBase(CPlayerGui* gui, void* draws) { ++g_baseCtors; g_lastGui = gui; g_lastDraws = draws; } };
struct CRetailList { void* head; CRetailList(const CRetailAllocator& al = CRetailAllocator()) : head(&head) { ++g_listCtors; } };
struct CDraw2DText : CDrawBase { virtual void v00() {} CRetailList m_entries; CDraw2DText(CPlayerGui* gui, void* draws) : CDrawBase(gui, draws), m_entries() {} };

int main()
{
    int gui, draws; CDraw2DText d((CPlayerGui*)&gui, &draws);
    if (g_baseCtors != 1 || g_listCtors != 1 || g_lastGui != &gui || g_lastDraws != &draws || d.m_entries.head != &d.m_entries.head) { std::printf("0064da8a_TEST FAIL\n"); return 1; }
    if ((unsigned char*)&d.m_entries - (unsigned char*)&d != 0x28) { std::printf("0064da8a_TEST FAIL\n"); return 1; }
    std::printf("0064da8a_TEST PASS\n");
    return 0;
}