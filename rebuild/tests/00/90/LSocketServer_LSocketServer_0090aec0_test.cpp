// Behaviour test for 0x0090AEC0 LSocketServer::LSocketServer: member defaults, empty lists, default UDP address,
// registration in the static ListOfSocketContainers.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct CRetailString {
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CRetailString()
    {
        _M_start = (char*)std::malloc(8);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + 8;
        *_M_finish = 0;
    }
    CRetailString& Assign(const char* first, const char* last)
    {
        size_t n = (size_t)(last - first);
        if ((size_t)(_M_end_of_storage - _M_start) < n + 1) {
            std::free(_M_start);
            _M_start = (char*)std::malloc(n + 1);
            _M_end_of_storage = _M_start + n + 1;
        }
        std::memcpy(_M_start, first, n);
        _M_finish = _M_start + n;
        *_M_finish = 0;
        return *this;
    }
    CRetailString& operator=(const char* text) { return Assign(text, text + std::strlen(text)); }
};

struct LAddress {
    CRetailString  ServerName;
    unsigned int   ServerIP;
    unsigned short ServerPort;
    int            TransportProtocol;
    int            TransportFlags;
    LAddress() : ServerName()
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = 0;
        TransportFlags = 0;
    }
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };

template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;
    list()
    {
        _M_node = (_List_node_base*)std::malloc(sizeof(_Node));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    void push_back(const _Tp& x)
    {
        _Node* tmp = (_Node*)std::malloc(sizeof(_Node));
        new (&tmp->_M_data) _Tp(x);
        _List_node_base* n = _M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
    int count() const
    {
        int c = 0;
        for (_List_node_base* it = _M_node->_M_next; it != _M_node; it = it->_M_next)
            ++c;
        return c;
    }
    _Tp& back() { return ((_Node*)_M_node->_M_prev)->_M_data; }
};

class LSocket;
struct DatagrammPacket;

class LSocketServer {
public:
    LSocket*                ListenerSocket;
    unsigned int            MaxConnections;
    bool                    Paused;
    list<LSocket*>          ListOfAcceptedConnections;
    list<LSocket*>          ListOfConnectedSockets;
    list<DatagrammPacket*>  ListOfUDPDatagramms;
    LAddress                UDPListenerAddress;
    static list<LSocketServer*> ListOfSocketContainers;
    LSocketServer();
};
list<LSocketServer*> LSocketServer::ListOfSocketContainers;

LSocketServer::LSocketServer()
{
    Paused = false;
    MaxConnections = 0x800;
    ListenerSocket = 0;
    ListOfSocketContainers.push_back(this);
}

int main()
{
    int ok = 1;
    LSocketServer* a = new LSocketServer;
    if (a->ListenerSocket != 0 || a->MaxConnections != 2048 || a->Paused) ok = 0;
    if (a->ListOfAcceptedConnections.count() != 0 || a->ListOfConnectedSockets.count() != 0
        || a->ListOfUDPDatagramms.count() != 0) ok = 0;
    if (a->UDPListenerAddress.ServerName._M_start[0] != 0 || a->UDPListenerAddress.ServerIP != 0
        || a->UDPListenerAddress.ServerPort != 0 || a->UDPListenerAddress.TransportProtocol != 0
        || a->UDPListenerAddress.TransportFlags != 0) ok = 0;
    if (LSocketServer::ListOfSocketContainers.count() != 1 || LSocketServer::ListOfSocketContainers.back() != a) ok = 0;
    LSocketServer* b = new LSocketServer;
    if (LSocketServer::ListOfSocketContainers.count() != 2 || LSocketServer::ListOfSocketContainers.back() != b) ok = 0;
    if (!ok) { std::printf("0090aec0_TEST FAIL\n"); return 1; }
    std::printf("0090aec0_TEST PASS\n");
    return 0;
}