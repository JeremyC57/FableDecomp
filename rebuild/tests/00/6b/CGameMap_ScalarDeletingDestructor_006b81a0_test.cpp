#include <cstdio>
#include <cstdlib>
// Model of CGameMap's deleting destructor @ 0x006B81A0: free the buffer when set, base destructor, delete when flagged.
static int g_freed, g_base; static void* g_freedPtr;
void MyFree(void* p) { ++g_freed; g_freedPtr = p; }
struct CMapBuffer { void* m_pData; ~CMapBuffer() { if (m_pData != 0) MyFree(m_pData); } };
namespace N_0099a300 { struct CBaseClass { virtual ~CBaseClass() { ++g_base; } }; } using N_0099a300::CBaseClass;
class CGameMap : public CBaseClass { public: CMapBuffer m_cells; virtual ~CGameMap() {} };
int main()
{
    int cell;
    { CGameMap m; m.m_cells.m_pData = &cell; }
    if (g_freed != 1 || g_freedPtr != &cell || g_base != 1) { std::printf("006b81a0_TEST FAIL\n"); return 1; }
    CGameMap* p = new CGameMap; p->m_cells.m_pData = 0; CBaseClass* b = p; delete b;
    if (g_freed != 1 || g_base != 2) { std::printf("006b81a0_TEST FAIL\n"); return 1; }
    std::printf("006b81a0_TEST PASS\n"); return 0;
}