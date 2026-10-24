// LSocketServer::LSocketServer() -- an idle socket server (no listener, 2048 max connections, not paused, three empty socket/packet lists, a default UDP listener address) registered in the static ListOfSocketContainers list
// Leaf: ??0LSocketServer@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090AEC0, 181 bytes (192 with padding), __fastcall, returns this.
// Identity: Ego_r.pdb ??0LSocketServer@LHNetworkLib@@QAE@XZ; the only caller is the factory at 0x0090B120 (`new` 0x34
//   bytes = retail sizeof(LSocketServer), then RequestListener 0x0090B080 -- landed). Global namespace, as the landed
//   LSocket / LSocketServer rows. The catalog label "Manager_ConstructAndRegister_0090aec0" is an autoname.
// Callees: malloc 0x00BFEA0E (the three list sentinels, the string's 8-byte initial block and the registration node),
//   STLport basic_string<char>::assign(const char*, const char*) 0x005693B0 (the "" literal 0x0122D70E as both bounds).
// Layout (retail 0x34, Ego_r.pdb names): ListenerSocket +0, MaxConnections +4, Paused +8, ListOfAcceptedConnections +0xC,
//   ListOfConnectedSockets +0x10, ListOfUDPDatagramms +0x14 (STLport lists are one sentinel pointer), UDPListenerAddress
//   +0x18 (retail LAddress 0x1C bytes: the 12-byte STLport string, IP +0xC, port +0x10, protocol +0x14, flags +0x18).
//   The static ListOfSocketContainers (list<LSocketServer*>) lives at 0x013BB664.
// Shape: STLport _List_base ctors (the _STLP_alloc_proxy sentinel zeroed, then the malloc'd node linked to itself), the
//   inline LAddress default ctor of the landed 0x00841E90 (string(), then = "" and the four zero stores in the body), the
//   three scalar members assigned in the BODY (Paused, MaxConnections, ListenerSocket order), then the inline STLport
//   push_back(this) = insert(end(), x) with the placement-new null test on &node->_M_data.
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
};

LSocketServer::LSocketServer()
{
    Paused = false;
    MaxConnections = 0x800;
    ListenerSocket = 0;
    ListOfSocketContainers.push_back(this);
}