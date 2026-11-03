// Behaviour test for 0x00C2A890 CNavigatorSwitch::SendStateAndReset: the first message goes out, then the state as it was,
// then the state is cleared before the second message goes out.
#include <cstdio>
static int gLog[8];
static int gCount = 0;
struct CNavigatorSwitch {
    void*         __vftable;
    unsigned char m_state;
    void SendMessage012b4168() { gLog[gCount++] = 100 + m_state; }
    void SendMessage012b4170() { gLog[gCount++] = 200 + m_state; }
    void SendState(unsigned char) { gLog[gCount++] = 300 + m_state; }
    void SendStateAndReset();
};
void CNavigatorSwitch::SendStateAndReset()
{
    SendMessage012b4168();
    SendState(m_state);
    m_state = 0;
    SendMessage012b4170();
}
int main()
{
    CNavigatorSwitch s;
    s.__vftable = 0;
    s.m_state = 1;
    s.SendStateAndReset();
    if (gCount != 3 || gLog[0] != 101 || gLog[1] != 301 || gLog[2] != 200 || s.m_state != 0) {
        std::printf("00c2a890_TEST FAIL\n");
        return 1;
    }
    std::printf("00c2a890_TEST PASS\n");
    return 0;
}