// Behaviour test for 0x0090AA40 LHNetworkLib::LSocketServer::~LSocketServer: the listener is closed and deleted, the
// server leaves the global server list, every accepted/connected socket is deleted (each socket unlinks itself from its
// server's lists, which is what ends retail's begin()-rereading loops), and the lists end empty. (A non-empty UDP packet
// list would never drain in retail -- the loop re-reads begin() and only nulls the slot -- so it is left empty here.)
#include <cstdio>
#include <list>
#include <algorithm>

struct Server;
static int g_closed = 0, g_deleted = 0;
struct Socket {
    Server* Owner;
    explicit Socket(Server* owner) : Owner(owner) { }
    bool Close() { ++g_closed; return true; }
    ~Socket();
};
struct Packet { int Size; };
struct Server {
    Socket* ListenerSocket;
    std::list<Socket*> Accepted, Connected;
    std::list<Packet*> Packets;
    static std::list<Server*> All;
    Server() : ListenerSocket(0) { All.push_back(this); }
    ~Server()
    {
        if (ListenerSocket)
            ListenerSocket->Close();
        delete ListenerSocket;
        ListenerSocket = 0;
        All.erase(std::find(All.begin(), All.end(), this));
        std::list<Socket*>::iterator it;
        for (it = Accepted.begin(); it != Accepted.end(); it = Accepted.begin())
            delete *it;
        for (it = Connected.begin(); it != Connected.end(); it = Connected.begin())
            delete *it;
        for (std::list<Packet*>::iterator p = Packets.begin(); p != Packets.end(); p = Packets.begin()) {
            delete *p;
            *p = 0;
        }
        Accepted.clear();
        Packets.clear();
        Connected.clear();
    }
};
std::list<Server*> Server::All;
Socket::~Socket()
{
    ++g_deleted;
    if (Owner) { Owner->Accepted.remove(this); Owner->Connected.remove(this); }
}

int main()
{
    int ok = 1;
    Server* other = new Server;
    Server* s = new Server;
    s->ListenerSocket = new Socket(0);
    s->Accepted.push_back(new Socket(s));
    s->Accepted.push_back(new Socket(s));
    s->Connected.push_back(new Socket(s));
    ok &= Server::All.size() == 2;
    delete s;
    ok &= g_closed == 1;                       // only the listener is closed explicitly
    ok &= g_deleted == 4;                      // listener + 2 accepted + 1 connected
    ok &= Server::All.size() == 1 && Server::All.front() == other;
    delete other;                              // no listener: nothing closed
    ok &= g_closed == 1 && Server::All.empty();
    std::printf("0090aa40_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}