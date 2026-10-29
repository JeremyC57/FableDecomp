#include <cstdio>
#include <cstring>
#include <new>

static int g_baseRuns = 0;
class CBaseClassNonCopyable {
public:  virtual ~CBaseClassNonCopyable() {} CBaseClassNonCopyable() { ++g_baseRuns; } };
struct CActionBase : CBaseClassNonCopyable { long m_actionState; bool m_started; bool m_finished;
    CActionBase() : m_actionState(0), m_started(false), m_finished(false) {} };
int main()
{
    unsigned char raw[sizeof(CActionBase)]; std::memset(raw, 0xaa, sizeof raw);
    CActionBase* a = new (raw) CActionBase;
    if (g_baseRuns != 1 || a->m_actionState != 0 || a->m_started || a->m_finished) { std::printf("00711fa0_TEST FAIL\n"); return 1; }
    a->~CActionBase();
    std::printf("00711fa0_TEST PASS\n");
    return 0;
}