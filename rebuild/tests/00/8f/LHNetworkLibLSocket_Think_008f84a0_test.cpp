// Behaviour test for 0x008F84A0 LSocket::Think: flushes every open socket once when the queued flag is set.
#include <cstdio>

class LSocket;

struct SSocketListNode {
    SSocketListNode* _M_next;
    SSocketListNode* _M_prev;
    LSocket*         _M_data;
};

struct CSocketList {
    SSocketListNode* _M_node;
    SSocketListNode* begin() const { return _M_node->_M_next; }
    SSocketListNode* end() const { return _M_node; }
};

bool        g_bSocketDataQueued;
CSocketList g_AllSockets;

class LSocket {
public:
    unsigned int Socket;
    int          m_flushes;
    bool TryToSendDataFromQueue() { ++m_flushes; return true; }
    static bool Think();
};

bool LSocket::Think()
{
    if (g_bSocketDataQueued) {
        for (SSocketListNode* it = g_AllSockets.begin(); it != g_AllSockets.end(); it = it->_M_next) {
            if (it->_M_data->Socket != (unsigned int)-1)
                it->_M_data->TryToSendDataFromQueue();
        }
        g_bSocketDataQueued = false;
    }
    return true;
}

int main()
{
    LSocket a, b, c;
    a.Socket = 5; b.Socket = (unsigned int)-1; c.Socket = 9;
    a.m_flushes = b.m_flushes = c.m_flushes = 0;
    SSocketListNode head, na, nb, nc;
    head._M_next = &na; na._M_next = &nb; nb._M_next = &nc; nc._M_next = &head;
    head._M_prev = &nc; na._M_prev = &head; nb._M_prev = &na; nc._M_prev = &nb;
    na._M_data = &a; nb._M_data = &b; nc._M_data = &c; head._M_data = 0;
    g_AllSockets._M_node = &head;
    int fail = 0;
    g_bSocketDataQueued = false;
    if (!LSocket::Think() || a.m_flushes || c.m_flushes) fail = 1;           // nothing queued: no flush
    g_bSocketDataQueued = true;
    if (!LSocket::Think() || a.m_flushes != 1 || b.m_flushes != 0 || c.m_flushes != 1 || g_bSocketDataQueued) fail = 1;
    if (fail) { std::printf("008f84a0_TEST FAIL\n"); return 1; }
    std::printf("008f84a0_TEST PASS\n");
    return 0;
}