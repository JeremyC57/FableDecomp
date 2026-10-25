// LHNetworkLib::LSocketServer::~LSocketServer() -- close and delete the listener, unregister from the server list, delete every accepted/connected socket and pending UDP packet, empty the three lists
// Leaf: ??1LSocketServer@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090AA40, 454 bytes, thiscall, ends in a tail `jmp` to the first list's destructor.
// Identity: Ego_r.pdb `??1LSocketServer@LHNetworkLib@@QAE@XZ` (non-virtual); the catalog's "CGuiControlTreePane_Destructor"
//   is a propagated label. Callers: the scalar-deleting destructor and LSocketServer::KillListener / CloseAllListeners.
// Callees: LSocket::Close 0x008F8C70 and LSocket::~LSocket 0x008F9780 (landed), operator delete 0x00BFE9BC, free
//   0x00BFEA14, list<DatagrammPacket*>::~list 0x0090B7D0, list<LSocket*>::~list 0x008F9F50 (twice, the last a tail jmp).
// Layout: the landed LSocketServer() 0x0090AEC0 (retail 0x34 bytes): ListenerSocket +0, MaxConnections +4, Paused +8,
//   ListOfAcceptedConnections +0xC, ListOfConnectedSockets +0x10, ListOfUDPDatagramms +0x14 (STLport lists: one sentinel
//   pointer), UDPListenerAddress +0x18 (retail LAddress, 12-byte STLport string first); the static
//   ListOfSocketContainers (list<LSocketServer*>) at 0x013BB664. DatagrammPacket: vector<char> Data +0, LAddress Source +0xC.
// Shape: `if (ListenerSocket) ListenerSocket->Close(); delete ListenerSocket; ListenerSocket = 0;`, then
//   erase(find(this)) on the server list (no end test, as retail), three delete loops that re-read begin() after each
//   delete (retail never advances the iterator: the socket loops rely on the sockets unlinking themselves, the packet
//   loop also nulls the slot), the three inline STLport clear()s (accepted, UDP, connected), then the member
//   destructors: the address string inline, the lists out of line.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    ~_String_base() { _M_deallocate_block(); }
};
template <class _CharT> class char_traits { };
template <class _CharT, class _Traits, class _Alloc>
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    ~basic_string() { }
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }
};

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    bool operator==(const _List_iterator& __x) const { return _M_node == __x._M_node; }
    bool operator!=(const _List_iterator& __x) const { return _M_node != __x._M_node; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~list();                                            // out of line (0x008F9F50 / 0x0090B7D0)
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void clear()
    {
        _Node* __cur = (_Node*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};
} // namespace std

namespace LHNetworkLib {

struct LAddress {                                       // retail 0x1C bytes
    std::string    ServerName;                          // +0x00
    unsigned long  ServerIP;                            // +0x0C
    unsigned short ServerPort;                          // +0x10
    long           TransportProtocol;                   // +0x14
    long           TransportFlags;                      // +0x18
};

class LSocket {
public:
    bool Close();                                       // 0x008F8C70
    ~LSocket();                                         // 0x008F9780
};

struct DatagrammPacket {
    std::vector<char> Data;                             // +0x00
    LAddress          Source;                           // +0x0C
};

class LSocketServer {
public:
    LSocket*                          ListenerSocket;            // +0x00
    unsigned long                     MaxConnections;            // +0x04
    bool                              Paused;                    // +0x08
    std::list<LSocket*>               ListOfAcceptedConnections; // +0x0C
    std::list<LSocket*>               ListOfConnectedSockets;    // +0x10
    std::list<DatagrammPacket*>       ListOfUDPDatagramms;       // +0x14
    LAddress                          UDPListenerAddress;        // +0x18
    static std::list<LSocketServer*>  ListOfSocketContainers;    // 0x013BB664
    ~LSocketServer();
};

LSocketServer::~LSocketServer()
{
    if (ListenerSocket)
        ListenerSocket->Close();
    delete ListenerSocket;
    ListenerSocket = 0;
    ListOfSocketContainers.erase(std::find(ListOfSocketContainers.begin(), ListOfSocketContainers.end(), this));
    std::list<LSocket*>::iterator it = ListOfAcceptedConnections.begin();
    for (; it != ListOfAcceptedConnections.end(); it = ListOfAcceptedConnections.begin())
        delete *it;
    for (it = ListOfConnectedSockets.begin(); it != ListOfConnectedSockets.end(); it = ListOfConnectedSockets.begin())
        delete *it;
    for (std::list<DatagrammPacket*>::iterator packet = ListOfUDPDatagramms.begin(); packet != ListOfUDPDatagramms.end();
         packet = ListOfUDPDatagramms.begin()) {
        delete *packet;
        *packet = 0;
    }
    ListOfAcceptedConnections.clear();
    ListOfUDPDatagramms.clear();
    ListOfConnectedSockets.clear();
}

} // namespace LHNetworkLib