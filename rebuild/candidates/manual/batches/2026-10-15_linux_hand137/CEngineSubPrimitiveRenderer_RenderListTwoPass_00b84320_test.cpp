// Behaviour test for 0x00B84320 CEngineSubPrimitiveRenderer::RenderListTwoPass: flagged subs in pass 1, all subs in pass 2.
#include <cstdio>

struct CSub;
static CSub* g_sub[32]; static void* g_par[32]; static int g_n = 0;
struct CManager { virtual void RenderSubPrimitive(CSub* s, void* p) { g_sub[g_n] = s; g_par[g_n] = p; ++g_n; } };
struct CSub { unsigned char m_flags; CSub* m_pNext; CManager* m_pManager; };
struct SEntry { long m_key; CSub* m_pFirst; };
struct CVec { SEntry* m_pFirst; SEntry* m_pLast; SEntry* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; } };

static void RenderListTwoPass(CVec* list, void* a, void* b)
{
    unsigned long count = list->GetCount();
    for (unsigned long i = 0; i < count; ++i) {
        CSub* first = list->m_pFirst[i].m_pFirst;
        CSub* sub = first;
        do { if (sub->m_flags & 4) sub->m_pManager->RenderSubPrimitive(sub, a); sub = sub->m_pNext; } while (sub);
        sub = first;
        do { sub->m_pManager->RenderSubPrimitive(sub, b); sub = sub->m_pNext; } while (sub);
    }
}

int main()
{
    CManager m; int pa, pb;
    CSub s[3];
    s[0].m_flags = 4; s[0].m_pNext = &s[1]; s[0].m_pManager = &m;
    s[1].m_flags = 0; s[1].m_pNext = 0;     s[1].m_pManager = &m;
    s[2].m_flags = 5; s[2].m_pNext = 0;     s[2].m_pManager = &m;
    SEntry e[2]; e[0].m_pFirst = &s[0]; e[1].m_pFirst = &s[2];
    CVec v; v.m_pFirst = e; v.m_pLast = e + 2; v.m_pEnd = e + 2;
    RenderListTwoPass(&v, &pa, &pb);
    CSub* want[5] = { &s[0], &s[0], &s[1], &s[2], &s[2] };
    void* wantp[5] = { &pa, &pb, &pb, &pa, &pb };
    if (g_n != 5) { std::printf("00b84320_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (g_sub[i] != want[i] || g_par[i] != wantp[i]) { std::printf("00b84320_TEST FAIL\n"); return 1; }
    std::printf("00b84320_TEST PASS\n");
    return 0;
}
