#include <cstdio>
static int g_strings = 0;
namespace N_0099a2d0 { struct CBase { virtual void v00() { } CBase() { } }; } using N_0099a2d0::CBase;
class CCharString { public: void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
namespace NCreatureAction {
class CActionEventBase : public CBase { public: bool m_bTriggered; long m_a; long m_b; long m_c; CActionEventBase(long a, long b, long c) : CBase(), m_bTriggered(false), m_a(a), m_b(b), m_c(c) { } };
struct CActionEventAddObject : CActionEventBase { CCharString m_name; CCharString m_param; bool m_flag;
    CActionEventAddObject(long a, long b, long c) : CActionEventBase(a, b, c), m_name(), m_param(), m_flag(false) { } }; }
int main() {
    NCreatureAction::CActionEventAddObject e(4, 5, 6); if (e.m_bTriggered || e.m_a != 4 || e.m_b != 5 || e.m_c != 6 || e.m_name.m_pData || e.m_param.m_pData || e.m_flag || g_strings != 2) { std::printf("00695690_TEST FAIL\n"); return 1; }
    std::printf("00695690_TEST PASS\n"); return 0;
}