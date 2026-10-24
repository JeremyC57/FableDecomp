// Behaviour test for 0x0090B450 LSocketServer::CheckForAcceptedConnectionsOnSocket
#include <stdio.h>
static int g_think = 0;
struct CSocketListNode { CSocketListNode* _M_next; CSocketListNode* _M_prev; void* _M_data; };
struct CSocketList {
    CSocketListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class LSocket;
class LSocketServer {
public:
    LSocket*    ListenerSocket;
    CSocketList ListOfAcceptedConnections;
    static void Think(bool force) { if (!force) ++g_think; }
    bool CheckForAcceptedConnectionsOnSocket();
};
bool LSocketServer::CheckForAcceptedConnectionsOnSocket()
{
    if (ListenerSocket == 0)
        return false;
    Think(false);
    return !ListOfAcceptedConnections.empty();
}
int main()
{
    int ok = 1;
    CSocketListNode sentinel, item;
    sentinel._M_next = &sentinel; sentinel._M_prev = &sentinel;
    LSocketServer srv;
    srv.ListOfAcceptedConnections._M_node = &sentinel;
    srv.ListenerSocket = 0;
    if (srv.CheckForAcceptedConnectionsOnSocket() != false || g_think != 0) ok = 0;
    srv.ListenerSocket = (LSocket*)&item;
    if (srv.CheckForAcceptedConnectionsOnSocket() != false || g_think != 1) ok = 0;
    sentinel._M_next = &item; item._M_next = &sentinel;
    if (srv.CheckForAcceptedConnectionsOnSocket() != true || g_think != 2) ok = 0;
    printf(ok ? "0090b450_TEST PASS\n" : "0090b450_TEST FAIL\n");
    return ok ? 0 : 1;
}