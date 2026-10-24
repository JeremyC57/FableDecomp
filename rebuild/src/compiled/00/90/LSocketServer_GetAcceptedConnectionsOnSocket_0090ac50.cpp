// LSocketServer::GetAcceptedConnectionsOnSocket() -- pops the first accepted connection (null when none): removed from ListOfAcceptedConnections, appended to ListOfConnectedSockets, returned
// Leaf: ?GetAcceptedConnectionsOnSocket@LSocketServer@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090AC50, 73 bytes, __fastcall, returns LSocket*.
// Identity: Ego_r.pdb ?GetAcceptedConnectionsOnSocket@LSocketServer@LHNetworkLib@@QAEPAVLSocket@2@XZ (PDB order puts it right
//   before the ctor, retail 0x0090AEC0 here); no call or absolute reference to it in retail (an unreferenced out-of-line copy). Global namespace,
//   as the landed LSocket / LSocketServer rows. The catalog label "LinkedList_PopFront" is an autoname.
// Callees: list<LSocket*>::remove(LSocket* const&) 0x0090B8E0 (out of line), malloc 0x00BFEA0E (the push_back node).
// Layout: see the LSocketServer ctor 0x0090AEC0: ListOfAcceptedConnections +0xC, ListOfConnectedSockets +0x10.
// Shape: the empty test is `begin() == end()` through iterator objects (begin node loaded once into eax, `cmp eax,edx`;
//   STLport empty() gives `cmp [eax],eax`); the value is read off that iterator; the
//   remove() argument is the REFERENCE `*it` returns (retail pushes &node->_M_data, not a local), then the inline
//   STLport push_back of the copy kept in edi.
#include <string.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

inline void* operator new(size_t, void* p) { return p; }

struct SRetailAllocProxy {                      // STLport _STLP_alloc_proxy<char*, char, allocator>: the end-of-storage pointer
    char* m_data;
    SRetailAllocProxy(char* data) : m_data(data) { }
};

struct CRetailStringBase {                      // STLport _String_base
    char* _M_start;
    char* _M_finish;
    SRetailAllocProxy _M_end_of_storage;
    CRetailStringBase(size_t n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = static_cast<char*>(malloc(n));
        _M_finish = _M_start;
        _M_end_of_storage.m_data = _M_start + n;
    }
};

struct CRetailString : CRetailStringBase {      // STLport basic_string<char>: 8-byte initial block, terminated
    CRetailString() : CRetailStringBase(8)
    {
        *_M_finish = 0;
    }
    CRetailString& Assign(const char* first, const char* last);   // 0x005693B0
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};

struct LAddress {                               // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    CRetailString  ServerName;                  // +0x00
    unsigned long  ServerIP;                    // +0x0C
    unsigned short ServerPort;                  // +0x10
    long           TransportProtocol;           // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;              // +0x18 LSOCKET_FLAGS
    LAddress()                                  // 0x00841E90 (landed), inline here
        : ServerName()
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = 0;
        TransportFlags = 0;
    }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    bool operator==(const _List_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Tp> struct SListAllocProxy {   // STLport _STLP_alloc_proxy<_List_node_base*, ...>
    _List_node_base* _M_data;
    SListAllocProxy(_List_node_base* p) : _M_data(p) { }
};

template <class _Tp> class list {               // STLport list: one sentinel pointer in retail
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;

    SListAllocProxy<_Tp> _M_node;

    list() : _M_node(0)
    {
        _List_node_base* __n = (_List_node_base*)malloc(sizeof(_Node));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator end() { return iterator(_M_node._M_data); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    void remove(const _Tp& __value);            // 0x0090B8E0 (list<LSocket*>)
};

class LSocket;
struct DatagrammPacket;

class LSocketServer {                           // retail 0x34 bytes
public:
    LSocket*                ListenerSocket;             // +0x00
    unsigned long           MaxConnections;             // +0x04
    bool                    Paused;                     // +0x08
    list<LSocket*>          ListOfAcceptedConnections;  // +0x0C
    list<LSocket*>          ListOfConnectedSockets;     // +0x10
    list<DatagrammPacket*>  ListOfUDPDatagramms;        // +0x14
    LAddress                UDPListenerAddress;         // +0x18
    static list<LSocketServer*> ListOfSocketContainers; // 0x013BB664
    LSocketServer();
    LSocket* GetAcceptedConnectionsOnSocket();
};

LSocket* LSocketServer::GetAcceptedConnectionsOnSocket()
{
    list<LSocket*>::iterator it = ListOfAcceptedConnections.begin();
    if (it == ListOfAcceptedConnections.end())
        return 0;
    LSocket* socket = *it;
    ListOfAcceptedConnections.remove(*it);
    ListOfConnectedSockets.push_back(socket);
    return socket;
}