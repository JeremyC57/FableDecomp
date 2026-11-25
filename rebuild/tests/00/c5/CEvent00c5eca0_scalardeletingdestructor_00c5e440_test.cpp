// Behaviour model of CSalEvent's scalar deleting destructor (0x00C5E440): delete through the base pointer runs the destructor (a held handle
// closed once) and frees the object; a null handle is not closed.
#include <cstdio>
static int g_closed; static void* g_last;
static void CloseStub(void* h) { ++g_closed; g_last = h; }
class CSalEvent { public: CSalEvent() : m_h(0) {} virtual ~CSalEvent() { if (m_h) { CloseStub(m_h); m_h = 0; } } void* m_h; };
int main()
{
    int h; CSalEvent* p = new CSalEvent; p->m_h = &h; delete p;
    if (g_closed != 1 || g_last != &h) { std::printf("00c5e440_TEST FAIL\n"); return 1; }
    p = new CSalEvent; delete p;
    if (g_closed != 1) { std::printf("00c5e440_TEST FAIL\n"); return 1; }
    std::printf("00c5e440_TEST PASS\n"); return 0;
}
