// Behaviour model of CSalKsPin's scalar deleting destructor (0x00C5EC80): delete through the base pointer runs the destructor (a held handle
// closed once) and frees the object; a -1 handle is not closed.
#include <cstdio>
static int g_closed; static void* g_last;
static void CloseStub(void* h) { ++g_closed; g_last = h; }
class CSalFile { public: CSalFile() : m_h((void*)-1) {} virtual ~CSalFile() { if (m_h != (void*)-1) { CloseStub(m_h); m_h = (void*)-1; } } void* m_h; };
class CSalKsPin : public CSalFile { public: virtual ~CSalKsPin(); };
CSalKsPin::~CSalKsPin() {}
int main()
{
    int h; CSalKsPin* p = new CSalKsPin; p->m_h = &h; delete p;
    if (g_closed != 1 || g_last != &h) { std::printf("CLandscapeBackgroundPatch_00c5ec80_TEST FAIL\n"); return 1; }
    p = new CSalKsPin; delete p;
    if (g_closed != 1) { std::printf("CLandscapeBackgroundPatch_00c5ec80_TEST FAIL\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00c5ec80_TEST PASS\n"); return 0;
}
