// behaviour test for 00c0e970 (standalone copy of the model)
#include <cstdio>
#include <cstring>

struct CEvent { virtual ~CEvent() {} virtual void Post() {} };
static CEvent* g_processed = 0;
struct CEventQueue { void Process(CEvent* e) { g_processed = e; } };
struct CScriptHost : public CEvent { CEventQueue* m_pQueue; virtual void Post() { m_pQueue->Process(this); } };

int main()
{
    CEventQueue q; CScriptHost h; h.m_pQueue = &q; CEvent* e = &h; e->Post();
    if (!(g_processed == &h)) { std::printf("00c0e970_TEST FAIL\n"); return 1; }
    std::printf("00c0e970_TEST PASS\n");
    return 0;
}