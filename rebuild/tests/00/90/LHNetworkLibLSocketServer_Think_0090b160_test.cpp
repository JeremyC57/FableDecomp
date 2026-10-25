// Behaviour test for 0x0090B160 LHNetworkLib::LSocketServer::Think: TCP servers accept while the listener is readable
// and below the connection limit (accepted sockets get their server and are queued; a socket over the limit is not
// accepted), UDP servers queue 0x400-byte packets until an empty receive (which is freed), then LSocket::Think runs.
#include <cstdio>
#include <list>
#include <vector>

struct Server;
static int g_pendingTcp = 0, g_pendingUdp = 0, g_activityChecks = 0, g_socketThinks = 0, g_packetsFreed = 0;
struct Socket {
    int Mode; Server* ServerObject;
    Socket() : Mode(0), ServerObject(0) { }
    bool IsReadable(long) { return g_pendingTcp > 0; }
    bool Accept(Socket&) { --g_pendingTcp; return true; }
    void Receive(std::vector<char>& data) { if (g_pendingUdp > 0) { --g_pendingUdp; data.resize(100); } else data.resize(0); }
};
struct Packet { std::vector<char> Data; ~Packet() { ++g_packetsFreed; } };
struct Server {
    Socket* ListenerSocket; unsigned int MaxConnections;
    std::list<Socket*> Accepted, Connected; std::list<Packet*> Packets;
};
static std::list<Server*> g_servers;

static void Think(bool checkForActivity)
{
    std::list<Server*>::iterator it;
    if (checkForActivity) {
        std::list<Socket*> listeners;
        for (it = g_servers.begin(); it != g_servers.end(); ++it)
            listeners.push_back((*it)->ListenerSocket);
        g_activityChecks += (int)listeners.size();
        listeners.erase(listeners.begin(), listeners.end());
    }
    for (it = g_servers.begin(); it != g_servers.end(); ++it) {
        int mode = (*it)->ListenerSocket->Mode;
        if (mode == 1) {
            while ((*it)->ListenerSocket->IsReadable(0)) {
                if ((*it)->Accepted.size() + (*it)->Connected.size() < (*it)->MaxConnections) {
                    Socket* socket = new Socket;
                    if ((*it)->ListenerSocket->Accept(*socket)) {
                        socket->ServerObject = *it;
                        (*it)->Accepted.push_back(socket);
                    }
                } else
                    --g_pendingTcp;              // test-only: drain so the model terminates (retail spins)
            }
        } else if (mode == 2) {
            char fill = 0;
            for (;;) {
                Packet* packet = new Packet;
                packet->Data.resize(0x400, fill);
                (*it)->ListenerSocket->Receive(packet->Data);
                if (packet->Data.size() == 0) { delete packet; break; }
                (*it)->Packets.push_back(packet);
            }
        }
    }
    ++g_socketThinks;
}

int main()
{
    int ok = 1;
    Socket tcpListener, udpListener; tcpListener.Mode = 1; udpListener.Mode = 2;
    Server tcp; tcp.ListenerSocket = &tcpListener; tcp.MaxConnections = 2;
    Server udp; udp.ListenerSocket = &udpListener; udp.MaxConnections = 0;
    tcp.Connected.push_back(0);                  // one already connected
    g_servers.push_back(&tcp); g_servers.push_back(&udp);
    g_pendingTcp = 1; g_pendingUdp = 3;
    Think(true);
    ok &= g_activityChecks == 2 && g_socketThinks == 1;
    ok &= tcp.Accepted.size() == 1 && tcp.Accepted.front()->ServerObject == &tcp;
    ok &= udp.Packets.size() == 3 && udp.Packets.front()->Data.size() == 100 && g_packetsFreed == 1;
    g_pendingTcp = 1;
    Think(false);                                // at the limit: nothing accepted
    ok &= g_activityChecks == 2 && g_socketThinks == 2 && tcp.Accepted.size() == 1 && g_packetsFreed == 2;
    std::printf("0090b160_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}