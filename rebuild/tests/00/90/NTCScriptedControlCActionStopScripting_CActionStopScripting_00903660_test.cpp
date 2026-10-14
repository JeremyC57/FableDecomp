#include <cstdio>
#include <cstring>
#include <new>

static int g_baseRuns = 0;
struct CBase { virtual ~CBase() {} CBase() { ++g_baseRuns; } };
struct CActionBase : CBase { long m_actionState; bool m_started; bool m_finished;
    CActionBase() : m_actionState(0), m_started(false), m_finished(false) {} };
struct CActionStopScripting : CActionBase { CActionStopScripting() {} };
int main()
{
    unsigned char raw[sizeof(CActionStopScripting)]; std::memset(raw, 0xaa, sizeof raw);
    CActionStopScripting* a = new (raw) CActionStopScripting;
    if (g_baseRuns != 1 || a->m_actionState != 0 || a->m_started || a->m_finished) { std::printf("00903660_TEST FAIL\n"); return 1; }
    a->~CActionStopScripting();
    std::printf("00903660_TEST PASS\n");
    return 0;
}