// Behaviour model of CSalFile's scalar deleting destructor (0x00C5EC00): delete through the base pointer runs the destructor (a held handle
// closed once) and frees the object; a -1 handle is not closed.
#include <cstdio>
static int g_closed; static void* g_last;
static void CloseStub(void* h) { ++g_closed; g_last = h; }
class CSalFile { public: CSalFile() : m_h((void*)-1) {} virtual ~CSalFile() { if (m_h != (void*)-1) { CloseStub(m_h); m_h = (void*)-1; } } void* m_h; };
int main()
{
    int h; CSalFile* p = new CSalFile; p->m_h = &h; delete p;
    if (g_closed != 1 || g_last != &h) { std::printf("00c5ec00_TEST FAIL\n"); return 1; }
    p = new CSalFile; delete p;
    if (g_closed != 1) { std::printf("00c5ec00_TEST FAIL\n"); return 1; }
    std::printf("00c5ec00_TEST PASS\n"); return 0;
}
