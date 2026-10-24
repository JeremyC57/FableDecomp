// Behaviour test for 0x0090B080 LSocketServer::RequestListener
#include <stdio.h>
static int g_listenResult = 0, g_deleted = 0, g_bound = 0;
struct LAddress { unsigned long ServerIP; long TransportProtocol; };
class LSocket {
public:
    bool NoListeningBinding;
    bool nonBlocking;
    LSocket() : NoListeningBinding(false), nonBlocking(false) { }
    ~LSocket() { ++g_deleted; }
    bool Listen(LAddress& address) { (void)address; return g_listenResult != 0; }
    bool Bind(LAddress& address) { g_bound = (int)address.ServerIP; return true; }
    bool SetNonBlocking(bool value) { nonBlocking = value; return true; }
};
class LSocketServer {
public:
    LSocket* ListenerSocket;
    LAddress UDPListenerAddress;
    bool RequestListener(LAddress& address);
};
bool LSocketServer::RequestListener(LAddress& address)
{
    ListenerSocket = new LSocket;
    bool result = false;
    switch (address.TransportProtocol) {
    case 1:
        if (ListenerSocket->Listen(address))
            return true;
        delete ListenerSocket;
        ListenerSocket = 0;
        break;
    case 2:
        UDPListenerAddress = address;
        result = true;
        ListenerSocket->Bind(UDPListenerAddress);
        ListenerSocket->SetNonBlocking(result);
        ListenerSocket->NoListeningBinding = result;
        break;
    }
    return result;
}
int main()
{
    int ok = 1;
    LSocketServer srv;
    LAddress a;
    a.ServerIP = 5; a.TransportProtocol = 1;
    g_listenResult = 1;
    if (srv.RequestListener(a) != true || srv.ListenerSocket == 0) ok = 0;
    delete srv.ListenerSocket; g_deleted = 0;
    g_listenResult = 0;
    if (srv.RequestListener(a) != false || srv.ListenerSocket != 0 || g_deleted != 1) ok = 0;
    a.TransportProtocol = 2; a.ServerIP = 33;
    if (srv.RequestListener(a) != true || srv.UDPListenerAddress.ServerIP != 33 || g_bound != 33) ok = 0;
    if (srv.ListenerSocket == 0 || !srv.ListenerSocket->nonBlocking || !srv.ListenerSocket->NoListeningBinding) ok = 0;
    delete srv.ListenerSocket;
    a.TransportProtocol = 7;
    if (srv.RequestListener(a) != false || srv.ListenerSocket == 0) ok = 0;
    delete srv.ListenerSocket;
    printf(ok ? "0090b080_TEST PASS\n" : "0090b080_TEST FAIL\n");
    return ok ? 0 : 1;
}