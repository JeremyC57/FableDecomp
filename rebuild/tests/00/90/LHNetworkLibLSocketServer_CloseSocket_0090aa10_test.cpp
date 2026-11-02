// test: LHNetworkLib::LSocketServer::CloseSocket -- find the socket in the connected list and remove every entry equal to it; other lists untouched
#include <stdio.h>
#include <list>
#include <algorithm>
struct LSocket { int id; };
struct LSocketServer {
    std::list<LSocket*> ListOfAcceptedConnections, ListOfConnectedSockets;
    void CloseSocket(LSocket* socket) {
        std::list<LSocket*>::iterator it = std::find(ListOfConnectedSockets.begin(), ListOfConnectedSockets.end(), socket);
        ListOfConnectedSockets.remove(*it);
    }
};
int main() {
    LSocket a = { 1 }, b = { 2 }, c = { 3 };
    LSocketServer s;
    s.ListOfConnectedSockets.push_back(&a); s.ListOfConnectedSockets.push_back(&b); s.ListOfConnectedSockets.push_back(&c);
    s.ListOfAcceptedConnections.push_back(&b);
    s.CloseSocket(&b);
    if (s.ListOfConnectedSockets.size() != 2 || s.ListOfConnectedSockets.front() != &a || s.ListOfConnectedSockets.back() != &c
        || s.ListOfAcceptedConnections.size() != 1) { printf("0090aa10_TEST FAIL\n"); return 1; }
    printf("0090aa10_TEST PASS\n");
    return 0;
}