// Behaviour test for CNavigatorSwitch::SendState @ 0x00C2A7A0 (build a state message from the switch's own state byte and hand it to the receiver; the argument is ignored).
#include <cstdio>

struct CNavigatorStateMessage {
    virtual ~CNavigatorStateMessage() { }
    CNavigatorStateMessage(unsigned char state) : m_state(state) { }
    unsigned char m_state;
};

struct CNavigatorReceiver {
    int received;
    unsigned char lastState;
    virtual ~CNavigatorReceiver() { }
    virtual void Receive(CNavigatorStateMessage* message) { ++received; lastState = message->m_state; }
};

struct CNavigatorSwitch {
    unsigned char      m_state;
    CNavigatorReceiver m_receiver;
    void SendState(unsigned char)
    {
        CNavigatorStateMessage message(m_state);
        m_receiver.Receive(&message);
    }
};

int main()
{
    CNavigatorSwitch s;
    s.m_state = 3;
    s.m_receiver.received = 0;
    s.m_receiver.lastState = 0;
    s.SendState(9);
    bool ok = s.m_receiver.received == 1 && s.m_receiver.lastState == 3;
    s.m_state = 0;
    s.SendState(1);
    ok = ok && s.m_receiver.received == 2 && s.m_receiver.lastState == 0;
    if (!ok) { std::printf("00c2a7a0_TEST FAIL\n"); return 1; }
    std::printf("00c2a7a0_TEST PASS\n");
    return 0;
}