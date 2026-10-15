#include <cstdio>
// Behaviour model of 0x00CAA410: the destructor stops the second interface, releases the entry queue, then the base destructor runs.
static char g_order[8]; static int g_n = 0;
struct IStreamCallback { virtual ~IStreamCallback() {} virtual long Stop() = 0; };
struct CBase { virtual ~CBase() { g_order[g_n++] = 'B'; } };
struct CObject : CBase, IStreamCallback {
    int m_entries;
    virtual long Stop() { g_order[g_n++] = 'S'; return 0; }
    void DeallocateScriptEntries() { m_entries = 0; g_order[g_n++] = 'D'; }
    virtual ~CObject() { Stop(); DeallocateScriptEntries(); }
};
int main()
{
    CObject* o = new CObject; o->m_entries = 3;
    CBase* b = o; delete b;
    if (g_n != 3 || g_order[0] != 'S' || g_order[1] != 'D' || g_order[2] != 'B') { std::printf("00caa410_TEST FAIL\n"); return 1; }
    std::printf("00caa410_TEST PASS\n");
    return 0;
}