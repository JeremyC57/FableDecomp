// Behaviour test for the NUISystem::CKeyRedefiner scalar deleting destructor @ 0x00558B30.
#include <cstdio>

static int g_order[4]; static int g_n = 0;
struct CComponentRoot { virtual ~CComponentRoot() {} };
struct CGuiControl { virtual ~CGuiControl() {} long m_body[4]; };
struct CHoverable { virtual ~CHoverable() {} };
struct CClickable : CComponentRoot, CGuiControl, CHoverable { virtual ~CClickable() { g_order[g_n++] = 2; } };
struct CKeyList { long m_size; ~CKeyList() { g_order[g_n++] = 1; } };
struct CKeyRedefiner : CClickable {
    CKeyList m_keys;
    static CKeyRedefiner* s_pSelected;
    virtual ~CKeyRedefiner() { if (s_pSelected == this) s_pSelected = 0; }
};
CKeyRedefiner* CKeyRedefiner::s_pSelected = 0;

int main()
{
    CKeyRedefiner* k = new CKeyRedefiner;
    CKeyRedefiner::s_pSelected = k;
    CHoverable* viaThird = k;
    delete viaThird;
    if (CKeyRedefiner::s_pSelected != 0 || g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("00558b30_TEST FAIL\n"); return 1; }
    std::printf("00558b30_TEST PASS\n");
    return 0;
}