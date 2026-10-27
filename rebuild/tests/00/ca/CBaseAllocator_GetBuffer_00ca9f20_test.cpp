// Behaviour test for 0x00CA9F20 CBaseAllocator::GetBuffer (standalone copy of the logic; free list, lock and wait stubbed)
#include <cstdio>
typedef int HRESULT;
static int g_locked;
struct Lock { Lock() { ++g_locked; } ~Lock() { --g_locked; } };
struct Sample { int m_cRef; };
struct Alloc {
    int m_bCommitted; Sample* m_free[4]; int m_count; int m_waiting; int m_waits; Sample* m_refill;
    Sample* RemoveHead() { return m_count ? m_free[--m_count] : 0; }
    void SetWaiting() { ++m_waiting; }
    void Wait() { ++m_waits; if (m_refill) { m_free[m_count++] = m_refill; m_refill = 0; } }
    HRESULT GetBuffer(Sample** pp, unsigned flags)
    {
        Sample* s;
        *pp = 0;
        for (;;) {
            {
                Lock l;
                if (!m_bCommitted) return (HRESULT)0x80040211L;
                s = RemoveHead();
                if (s == 0) SetWaiting();
            }
            if (s) break;
            if (flags & 4) return (HRESULT)0x8004022EL;
            Wait();
        }
        s->m_cRef = 1;
        *pp = s;
        return 0;
    }
};
int main()
{
    Sample a = { 0 }, b = { 0 };
    Alloc al = { 0, { 0 }, 0, 0, 0, 0 };
    Sample* out = &a;
    if (al.GetBuffer(&out, 0) != (HRESULT)0x80040211L || out != 0) { std::printf("00ca9f20_TEST FAIL\n"); return 1; }
    al.m_bCommitted = 1;
    if (al.GetBuffer(&out, 4) != (HRESULT)0x8004022EL || al.m_waiting != 1 || al.m_waits != 0) { std::printf("00ca9f20_TEST FAIL\n"); return 1; }
    al.m_refill = &b;
    if (al.GetBuffer(&out, 0) != 0 || out != &b || b.m_cRef != 1 || al.m_waits != 1) { std::printf("00ca9f20_TEST FAIL\n"); return 1; }
    al.m_free[al.m_count++] = &a;
    if (al.GetBuffer(&out, 4) != 0 || out != &a || a.m_cRef != 1 || g_locked != 0) { std::printf("00ca9f20_TEST FAIL\n"); return 1; }
    std::printf("00ca9f20_TEST PASS\n");
    return 0;
}