// Behaviour test for 0x008F8C70 LSocket::Close
#include <stdio.h>
static int g_closeResult = 0, g_closeCalls = 0, g_serverCalls = 0, g_clearCalls = 0;
static int closesocket(unsigned int s) { (void)s; ++g_closeCalls; return g_closeResult; }
class LSocket;
class LSocketServer {
public:
    void CloseSocket(LSocket* socket) { (void)socket; ++g_serverCalls; }
};
class LSocket {
public:
    unsigned int   Socket;
    long           ConnectionMode;
    LSocketServer* ServerObject;
    void ClearData() { ++g_clearCalls; }
    bool Close();
};
bool LSocket::Close()
{
    if (Socket != (unsigned int)-1 && ConnectionMode != 0) {
        if (closesocket(Socket) != 0)
            return false;
    }
    if (ServerObject)
        ServerObject->CloseSocket(this);
    ClearData();
    Socket = (unsigned int)-1;
    ConnectionMode = 0;
    return true;
}
int main()
{
    int ok = 1;
    LSocketServer server;
    LSocket s;
    s.Socket = 5; s.ConnectionMode = 1; s.ServerObject = &server;
    g_closeResult = -1;
    if (s.Close() != false || s.Socket != 5 || g_clearCalls != 0) ok = 0;
    g_closeResult = 0;
    if (s.Close() != true || s.Socket != (unsigned int)-1 || s.ConnectionMode != 0 || g_serverCalls != 1 || g_clearCalls != 1) ok = 0;
    s.ServerObject = 0;
    if (s.Close() != true || g_closeCalls != 2 || g_serverCalls != 1 || g_clearCalls != 2) ok = 0;
    printf(ok ? "008f8c70_TEST PASS\n" : "008f8c70_TEST FAIL\n");
    return ok ? 0 : 1;
}