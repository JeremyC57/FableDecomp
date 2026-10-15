// Behaviour test for the NUISystem::CRedefinerList scalar deleting destructor @ 0x00557360.
#include <cstdio>

static int g_scrollDtors = 0;
struct CListRoot { virtual ~CListRoot() {} };
struct CGuiControl { virtual ~CGuiControl() {} long m_body[4]; };
struct CHoverable { virtual ~CHoverable() {} };
struct CScrollableList : CListRoot, CGuiControl, CHoverable { virtual ~CScrollableList() { ++g_scrollDtors; } };
struct CRedefinerList : CScrollableList {
    static CRedefinerList* s_pActiveList;
    virtual ~CRedefinerList() { if (s_pActiveList == this) s_pActiveList = 0; }
};
CRedefinerList* CRedefinerList::s_pActiveList = 0;

int main()
{
    CRedefinerList* a = new CRedefinerList;
    CRedefinerList* b = new CRedefinerList;
    CRedefinerList::s_pActiveList = a;
    delete b;                                        // not the active list: the global stays
    if (CRedefinerList::s_pActiveList != a || g_scrollDtors != 1) { std::printf("00557360_TEST FAIL\n"); return 1; }
    CGuiControl* viaBase = a;                        // deleting through the second base (the adjustor thunk path)
    delete viaBase;
    if (CRedefinerList::s_pActiveList != 0 || g_scrollDtors != 2) { std::printf("00557360_TEST FAIL\n"); return 1; }
    std::printf("00557360_TEST PASS\n");
    return 0;
}