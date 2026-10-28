// LHNetworkLib::LSocketServer::Think(bool checkForActivity) -- optionally select() over every listener, then accept pending TCP connections (up to each server's limit) and queue every waiting UDP datagram, and finally run LSocket::Think
// Leaf: ?Think@LSocketServer@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090B160, 742 bytes (+ int3 padding). Ego_r.pdb `?Think@LSocketServer@LHNetworkLib@@SIX_N@Z`:
//   a static `__fastcall` (checkForActivity in cl), plain `ret`. The catalog's "LSocket_ProcessNetworkEvents" is descriptive.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes), LSocket::CheckForActivity 0x008F8540 (static fastcall,
//   this chunk), LSocket::IsReadable(long) 0x008F8210, operator new 0x00BFEA1A, LSocket::LSocket 0x008F8A80,
//   LSocket::Accept(LSocket&) 0x008F8EA0, basic_string::assign 0x005693B0 (the packet address's ClearData),
//   memmove 0x00BFEAE6 / vector<char>::_M_fill_insert 0x004110D0 (the inline resize), LSocket::Receive(vector<char>&,
//   LAddress&, const LAddress&) 0x008F9320, operator delete 0x00BFE9BC, LSocket::Think 0x008F84A0.
// Layout: the landed LSocketServer() 0x0090AEC0 / ~LSocketServer 0x0090AA40 (this chunk): ListenerSocket +0,
//   MaxConnections +4, the three STLport lists +0xC/+0x10/+0x14, UDPListenerAddress +0x18; ListOfSocketContainers at
//   0x013BB664; LSocket ConnectionMode +0x60, ServerObject +0x80; DatagrammPacket (0x28) Data +0, Source +0xC.
// Shape: the listener list is a local STLport list emptied with erase(begin(), end()) before its destructor; every
//   server is reached through `(*it)->` (re-read after each call); the connection limit is the inline
//   accepted.size() + connected.size(); `new DatagrammPacket` runs the inline vector / LAddress constructors; the
//   resize fill char is a NAMED local zeroed once before the UDP loop ([esp+0x13] stored ahead of the loop head; the
//   `resize(0x400)` char() temporary is stored inside the loop, 743 bytes, and /Oa is worse).
extern "C" void* __cdecl malloc(unsigned int size);                                // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count); // 0x00BFEAE6
extern "C" unsigned int __cdecl strlen(const char* s);
#pragma intrinsic(strlen)
void* operator new(unsigned int size);                                             // 0x00BFEA1A
void operator delete(void* block);                                                 // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __true_type { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}
inline void _Destroy(char*, char*) { }

template <class _CharT> class char_traits {
public:
    static unsigned int length(const char* __s) { return strlen(__s); }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;                                 // +0x00
    _Tp* _M_finish;                                // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    void _M_allocate_block(unsigned int __n)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _String_base(const _Alloc& __a, unsigned int __n)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { _M_allocate_block(__n); }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _CharT, class _Traits, class _Alloc>
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    basic_string() : _String_base<_CharT, _Alloc>(_Alloc(), 8) { _M_terminate_string(); }
    ~basic_string() { }
    void _M_terminate_string() { *this->_M_finish = _CharT(); }
    _Self& assign(const _CharT* __f, const _CharT* __l);                          // 0x005693B0
    _Self& operator=(const _CharT* __s) { return assign(__s, __s + _Traits::length(__s)); }
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                 // +0x00
    _Tp* _M_finish;                                // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);         // 0x004110D0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

struct _List_node_base {
    _List_node_base* _M_next;                      // +0x00
    _List_node_base* _M_prev;                      // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                   // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    bool operator!=(const _List_iterator& __x) const { return _M_node != __x._M_node; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    _List_iterator operator++(int) { _List_iterator __tmp = *this; _M_node = _M_node->_M_next; return __tmp; }
};
template <class _InputIterator>
inline unsigned int distance(_InputIterator __first, _InputIterator __last)
{
    unsigned int __n = 0;
    while (__first != __last) {
        ++__first;
        ++__n;
    }
    return __n;
}
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    _List_base(const _Alloc&) : _M_node(allocator<_Node>(), (_List_node_base*)0)
    {
        _List_node_base* __n = (_List_node_base*)malloc(sizeof(_Node));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
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

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_node<_Tp> _Node;
    list() : _List_base<_Tp, _Alloc>(_Alloc()) { }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    unsigned int size() { return distance(begin(), end()); }
    protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    public:
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    iterator erase(iterator __first, iterator __last)
    {
        while (__first != __last)
            erase(__first++);
        return __last;
    }
};
} // namespace std

namespace LHNetworkLib {

enum LSOCKET_PROTOCOL { LSOCKET_PROTOCOL_NONE = 0, LSOCKET_PROTOCOL_TCP = 1, LSOCKET_PROTOCOL_UDP = 2 };
enum LSOCKET_SELECT { LSOCKET_SELECT_READ = 1 };

struct LAddress {                                  // retail 0x1C bytes (12-byte STLport string)
    std::string      ServerName;                   // +0x00
    unsigned long    ServerIP;                     // +0x0C
    unsigned short   ServerPort;                   // +0x10
    LSOCKET_PROTOCOL TransportProtocol;            // +0x14
    long             TransportFlags;               // +0x18
    LAddress() { ClearData(); }
    void ClearData()
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = LSOCKET_PROTOCOL_NONE;
        TransportFlags = 0;
    }
};

class LSocketServer;

class LSocket {                                    // retail 0x9C bytes
public:
    unsigned char    _pad00[0x60];
    LSOCKET_PROTOCOL ConnectionMode;               // +0x60
    unsigned char    _pad64[0x80 - 0x64];
    LSocketServer*   ServerObject;                 // +0x80
    unsigned char    _pad84[0x9C - 0x84];
    LSocket();                                     // 0x008F8A80
    bool IsReadable(long timeout);                 // 0x008F8210
    bool Accept(LSocket& socket);                  // 0x008F8EA0
    bool Receive(std::vector<char>& data, LAddress& source, const LAddress& listener);   // 0x008F9320
    static bool __fastcall CheckForActivity(std::list<LSocket*>& sockets, LSOCKET_SELECT select, unsigned long timeout); // 0x008F8540
    static bool __fastcall Think();                // 0x008F84A0
};

struct DatagrammPacket {                           // retail 0x28 bytes
    std::vector<char> Data;                        // +0x00
    LAddress          Source;                      // +0x0C
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
    static void __fastcall Think(bool checkForActivity);
};

void __fastcall LSocketServer::Think(bool checkForActivity)
{
    std::list<LSocketServer*>::iterator it = ListOfSocketContainers.begin();
    if (checkForActivity) {
        std::list<LSocket*> listeners;
        for (it = ListOfSocketContainers.begin(); it != ListOfSocketContainers.end(); ++it)
            listeners.push_back((*it)->ListenerSocket);
        LSocket::CheckForActivity(listeners, LSOCKET_SELECT_READ, (unsigned long)-1);
        listeners.erase(listeners.begin(), listeners.end());
    }
    for (it = ListOfSocketContainers.begin(); it != ListOfSocketContainers.end(); ++it) {
        LSOCKET_PROTOCOL mode = (*it)->ListenerSocket->ConnectionMode;
        if (mode == LSOCKET_PROTOCOL_TCP) {
            while ((*it)->ListenerSocket->IsReadable(0)) {
                if ((*it)->ListOfAcceptedConnections.size() + (*it)->ListOfConnectedSockets.size() < (*it)->MaxConnections) {
                    LSocket* socket = new LSocket;
                    if ((*it)->ListenerSocket->Accept(*socket)) {
                        socket->ServerObject = *it;
                        (*it)->ListOfAcceptedConnections.push_back(socket);
                    }
                }
            }
        } else if (mode == LSOCKET_PROTOCOL_UDP) {
            char fill = 0;
            for (;;) {
                DatagrammPacket* packet = new DatagrammPacket;
                packet->Data.resize(0x400, fill);
                (*it)->ListenerSocket->Receive(packet->Data, packet->Source, (*it)->UDPListenerAddress);
                if (packet->Data.size() == 0) {
                    delete packet;
                    break;
                }
                (*it)->ListOfUDPDatagramms.push_back(packet);
            }
        }
    }
    LSocket::Think();
}

} // namespace LHNetworkLib