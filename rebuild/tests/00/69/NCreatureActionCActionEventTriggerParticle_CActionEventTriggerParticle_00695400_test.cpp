#include <cstdio>
static int g_strings = 0;
struct CBase { virtual void v00() { } CBase() { } };
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
namespace NCreatureAction {
struct CActionEventBase : CBase { bool m_bTriggered; long m_a; long m_b; long m_c; CActionEventBase(long a, long b, long c) : CBase(), m_bTriggered(false), m_a(a), m_b(b), m_c(c) { } };
struct CActionEventTriggerParticle : CActionEventBase { long m_d; CCharString m_name; CCharString m_param; bool m_flag0; bool m_flag;
    CActionEventTriggerParticle(long a, long b, long c) : CActionEventBase(a, b, c), m_name(), m_param(), m_flag(false) { } }; }
int main() {
    NCreatureAction::CActionEventTriggerParticle e(1, 2, 3); if (e.m_bTriggered || e.m_a != 1 || e.m_b != 2 || e.m_c != 3 || e.m_name.m_pData || e.m_param.m_pData || e.m_flag || g_strings != 2) { std::printf("00695400_TEST FAIL\n"); return 1; }
    std::printf("00695400_TEST PASS\n"); return 0;
}