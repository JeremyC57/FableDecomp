#include <cstdio>
// Behaviour test for 00c219f0: both stages run on the event in order; the result is 0 once the state word is 6, else -1.
class CInputEvent { public: int m_key; };
static int g_order[8]; static int g_n = 0;
struct CInputEventProcessor_00c219f0 {
    short m_state;
    short m_nextState;
    void BeginEvent(CInputEvent* event) { g_order[g_n++] = 1; (void)event; }
    void HandleEvent(CInputEvent* event) { g_order[g_n++] = 2; if (event->m_key == 5) m_state = m_nextState; }
    long ProcessEvent(CInputEvent* event);
};
long CInputEventProcessor_00c219f0::ProcessEvent(CInputEvent* event)
{
    BeginEvent(event);
    HandleEvent(event);
    return m_state == 6 ? 0 : -1;
}
int main()
{
    CInputEventProcessor_00c219f0 p; p.m_state = 0; p.m_nextState = 6;
    CInputEvent other; other.m_key = 1;
    CInputEvent finish; finish.m_key = 5;
    long r1 = p.ProcessEvent(&other);
    long r2 = p.ProcessEvent(&finish);
    p.m_state = 7;
    long r3 = p.ProcessEvent(&other);
    if (r1 != -1 || r2 != 0 || r3 != -1 || g_n != 6 || g_order[0] != 1 || g_order[1] != 2 || g_order[4] != 1 || g_order[5] != 2) {
        std::printf("00c219f0_TEST FAIL\n"); return 1;
    }
    std::printf("00c219f0_TEST PASS\n");
    return 0;
}
