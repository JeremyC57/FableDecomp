// LHNetworkLib::LSocket::ClearData() -- reset a socket's state: no protocol, no pending activity, both addresses cleared, the byte counters zeroed, the socket information wiped, INVALID_SOCKET, both byte queues emptied, not listening
// Leaf: ?ClearData@LSocket@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FA120, 352 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: Ego_r.pdb `?ClearData@LSocket@LHNetworkLib@@AAEXXZ` (private); the catalog's "Socket_ReceiveDataAndBuffer"
//   is descriptive. Callers: LSocket::LSocket 0x008F8A80 (this chunk) and LSocket::Close 0x008F8C70 (landed).
// Callees: basic_string::assign(const char*, const char*) 0x005693B0 (landed; `ServerName = ""` with strlen("")
//   folded: two pushes of the same literal), memmove 0x00BFEAE6 (the vector<char> erase), vector<char>::_M_fill_insert
//   0x004110D0 (landed; resize's grow arm).
// Layout: retail LSocket (0x9C bytes, the landed LSocket rows' model with Ego_r.pdb names) over 12-byte STLport strings
//   and vectors; LAddress::ClearData (Ego_r.pdb) is the inline `ServerName = ""; ServerPort = 0; ServerIP = 0;
//   TransportProtocol = 0; TransportFlags = 0;` (the LAddress constructor runs the same stores).
// Shape: InternalPacketBuffer gets `resize(0)` then `clear()`, SendOutputQueue `clear()` then `resize(0)`: STLport
//   `resize(n) { resize(n, _Tp()); }` over a `const _Tp&` resize (`n < size()` -> erase(begin() + n, end()) else insert(end(), n - size(), x) with the char()
//   temporary homed at [esp+0x13]; a by-value `_Tp __x` puts it at +0x10) and erase's __copy_ptrs/__copy_trivial (`last == first ? result : memmove(..) + n`, the compare of the
//   finish pointer with itself left unfolded); the 16-byte SocketInformation memset is the intrinsic.
extern "C" void* __cdecl malloc(unsigned int size);                                // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count); // 0x00BFEAE6
extern "C" void* __cdecl memset(void* dest, int c, unsigned int count);
extern "C" unsigned int __cdecl strlen(const char* s);
#pragma intrinsic(memset, strlen)
inline void* operator new(unsigned int, void* p) { return p; }

struct sockaddr_in {                               // winsock sockaddr_in, 16 bytes
    short          sin_family;
    unsigned short sin_port;
    unsigned long  sin_addr;
    char           sin_zero[8];
};

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
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value allocate(unsigned int __n) { return _MaybeReboundAlloc::allocate(__n); }
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
    _String_base(const _Alloc& __a, unsigned int __n)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { _M_allocate_block(__n); }
};

template <class _CharT, class _Traits, class _Alloc>
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    basic_string() : _String_base<_CharT, _Alloc>(_Alloc(), 8) { _M_terminate_string(); }
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
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
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
    void clear() { erase(begin(), end()); }
};

template <class _Tp> struct _List_node_base_t { };
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
    bool operator!=(const _List_iterator& __x) const { return _M_node != __x._M_node; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
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
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node<_Tp>* _M_node;                      // the sentinel
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    unsigned int size() { return distance(begin(), end()); }
    _List_node<_Tp>* _M_create_node(const _Tp& __x)
    {
        _List_node<_Tp>* __p = (_List_node<_Tp>*)malloc(sizeof(_List_node<_Tp>));
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _List_node<_Tp>* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
} // namespace std

namespace LHNetworkLib {

enum LSOCKET_PROTOCOL { LSOCKET_PROTOCOL_NONE = 0 };
enum LSOCKET_FLAGS { LSOCKET_FLAGS_NONE = 0 };

struct LAddress {                                  // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    std::string      ServerName;                   // +0x00
    unsigned long    ServerIP;                     // +0x0C
    unsigned short   ServerPort;                   // +0x10
    LSOCKET_PROTOCOL TransportProtocol;            // +0x14
    LSOCKET_FLAGS    TransportFlags;               // +0x18
    LAddress() { ClearData(); }
    void ClearData()
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = LSOCKET_PROTOCOL_NONE;
        TransportFlags = LSOCKET_FLAGS_NONE;
    }
};

class LSocketServer;

class LSocket {                                    // retail 0x9C bytes (Ego_r.pdb 196 with its larger LAddress/vector)
public:
    LAddress          RemoteAddress;               // +0x00
    LAddress          LocalAddress;                // +0x1C
    unsigned int      Socket;                      // +0x38
    sockaddr_in       SocketInformation;           // +0x3C
    sockaddr_in       SocketInterfaceInformation;  // +0x4C
    bool              ListeningSocket;             // +0x5C
    LSOCKET_PROTOCOL  ConnectionMode;              // +0x60
    std::vector<char> SendOutputQueue;             // +0x64
    std::vector<char> InternalPacketBuffer;        // +0x70
    bool              NoListeningBinding;          // +0x7C
    LSocketServer*    ServerObject;                // +0x80
    bool              ActivityPending;             // +0x84
    unsigned long     NrTotalDataSent;             // +0x88
    unsigned long     NrTotalDataReceived;         // +0x8C
    unsigned long     NrLastDataSent;              // +0x90
    unsigned long     NrLastDataReceived;          // +0x94
    void*             _DEBUGFP;                    // +0x98
    LSocket();
private:
    void ClearData();
public:
};

} // namespace LHNetworkLib

void LHNetworkLib::LSocket::ClearData()
{
    ConnectionMode = LSOCKET_PROTOCOL_NONE;
    ActivityPending = false;
    RemoteAddress.ClearData();
    LocalAddress.ClearData();
    NrLastDataReceived = 0;
    NrTotalDataReceived = 0;
    NrTotalDataSent = 0;
    NrLastDataSent = 0;
    memset(&SocketInformation, 0, sizeof(SocketInformation));
    Socket = (unsigned int)-1;
    InternalPacketBuffer.resize(0);
    InternalPacketBuffer.clear();
    SendOutputQueue.clear();
    SendOutputQueue.resize(0);
    ListeningSocket = false;
}