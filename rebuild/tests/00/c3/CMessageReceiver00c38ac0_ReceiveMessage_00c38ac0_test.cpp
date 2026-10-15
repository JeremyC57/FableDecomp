#include <cstdio>
#include <cstring>

struct SMailMessage { long m_id; long m_param1; long m_param2; };
struct CMailboxQueue {
    SMailMessage m_next;
    void* DispatchMessage(void* message) { *(SMailMessage*)message = m_next; return message; }
};
struct CMessageReceiver {
    CMailboxQueue m_queue; long m_param1; long m_param2;
    long ReceiveMessage()
    {
        SMailMessage message;
        m_queue.DispatchMessage(&message);
        m_param1 = message.m_param1;
        m_param2 = message.m_param2;
        return message.m_id;
    }
};
int main()
{
    CMessageReceiver r; r.m_param1 = r.m_param2 = 0;
    r.m_queue.m_next.m_id = 7; r.m_queue.m_next.m_param1 = 11; r.m_queue.m_next.m_param2 = -3;
    if (r.ReceiveMessage() != 7 || r.m_param1 != 11 || r.m_param2 != -3) { std::printf("00c38ac0_TEST FAIL\n"); return 1; }
    std::printf("00c38ac0_TEST PASS\n");
    return 0;
}