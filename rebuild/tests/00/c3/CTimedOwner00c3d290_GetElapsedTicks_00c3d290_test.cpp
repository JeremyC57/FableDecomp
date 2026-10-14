#include <cstdio>
static __int64 g_now = 0;
static int __stdcall FakeQueryPerformanceCounter(__int64* c) { *c = g_now; return 1; }
struct CTimedObject { unsigned char _pad[0x190]; __int64 m_startTicks; };
struct CTimedOwner { void* m_vptr; CTimedObject* m_pObject;
    __int64 GetElapsedTicks() { CTimedObject* o = m_pObject; if (o) { __int64 now; FakeQueryPerformanceCounter(&now); return now - o->m_startTicks; } return 0; } };
int main()
{
    CTimedObject obj; obj.m_startTicks = 0x100000000i64; CTimedOwner w; w.m_pObject = &obj;
    g_now = 0x100000005i64; if (w.GetElapsedTicks() != 5) { std::printf("00c3d290_TEST FAIL\n"); return 1; }
    g_now = 0x0FFFFFFFFi64; if (w.GetElapsedTicks() != -1) { std::printf("00c3d290_TEST FAIL\n"); return 1; }
    w.m_pObject = 0; if (w.GetElapsedTicks() != 0) { std::printf("00c3d290_TEST FAIL\n"); return 1; }
    std::printf("00c3d290_TEST PASS\n"); return 0;
}