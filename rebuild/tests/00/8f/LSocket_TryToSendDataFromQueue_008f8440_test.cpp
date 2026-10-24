// Behaviour test for 0x008F8440 LSocket::TryToSendDataFromQueue
#include <stdio.h>
#include <string.h>
static int g_writeable = 1, g_sendResult = 0, g_sendCalls = 0, g_eraseCalls = 0;
static int sendStub(unsigned int s, const char* buf, int len, int flags) { (void)s; (void)buf; (void)len; (void)flags; ++g_sendCalls; return g_sendResult; }
struct CCharVector {
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    char* begin() { return _M_start; }
    char* end() { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    char* erase(char* first, char* last)
    {
        ++g_eraseCalls;
        memmove(first, last, _M_finish - last);
        _M_finish -= (last - first);
        return first;
    }
};
class LSocket {
public:
    unsigned int Socket;
    CCharVector  SendOutputQueue;
    bool IsWriteable(long timeoutMs) { (void)timeoutMs; return g_writeable != 0; }
    bool TryToSendDataFromQueue();
};
bool LSocket::TryToSendDataFromQueue()
{
    int sent = 0;
    if (SendOutputQueue.size() != 0 && Socket != (unsigned int)-1) {
        if (IsWriteable(0)) {
            sent = sendStub(Socket, SendOutputQueue.begin(), SendOutputQueue.size(), 0);
            if (sent == -1)
                return false;
        }
        SendOutputQueue.erase(SendOutputQueue.begin() + sent, SendOutputQueue.begin() + sent);
    }
    return true;
}
int main()
{
    int ok = 1;
    char data[8] = "abcdefg";
    LSocket s;
    s.Socket = 3;
    s.SendOutputQueue._M_start = data; s.SendOutputQueue._M_finish = data + 7; s.SendOutputQueue._M_end_of_storage = data + 8;
    g_sendResult = 4;
    if (s.TryToSendDataFromQueue() != true || g_sendCalls != 1 || g_eraseCalls != 1 || s.SendOutputQueue.size() != 7) ok = 0;
    g_sendResult = -1;
    if (s.TryToSendDataFromQueue() != false || g_eraseCalls != 1) ok = 0;
    g_writeable = 0;
    if (s.TryToSendDataFromQueue() != true || g_sendCalls != 2 || g_eraseCalls != 2) ok = 0;
    s.SendOutputQueue._M_finish = data;
    if (s.TryToSendDataFromQueue() != true || g_eraseCalls != 2) ok = 0;
    printf(ok ? "008f8440_TEST PASS\n" : "008f8440_TEST FAIL\n");
    return ok ? 0 : 1;
}