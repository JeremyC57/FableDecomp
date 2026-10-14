#include <cstdio>
#include <cstring>
#include <new>

static int g_baseRuns = 0;
struct CPSCSplineBase { virtual ~CPSCSplineBase() {} unsigned char _body[0x30]; CPSCSplineBase() { ++g_baseRuns; } };
struct CPSCSpline : CPSCSplineBase {
    long m_pointCount; bool m_looped; bool m_dirty; bool m_closed; unsigned char _pad3b; long m_pCurrentPoint;
    CPSCSpline() { m_pCurrentPoint = 0; m_pointCount = 0; m_dirty = false; m_looped = false; m_closed = false; }
};
int main()
{
    unsigned char raw[sizeof(CPSCSpline)]; std::memset(raw, 0xaa, sizeof raw);
    CPSCSpline* s = new (raw) CPSCSpline;
    if (g_baseRuns != 1 || s->m_pointCount != 0 || s->m_looped || s->m_dirty || s->m_closed || s->m_pCurrentPoint != 0) { std::printf("00b1b350_TEST FAIL\n"); return 1; }
    s->~CPSCSpline();
    std::printf("00b1b350_TEST PASS\n");
    return 0;
}
