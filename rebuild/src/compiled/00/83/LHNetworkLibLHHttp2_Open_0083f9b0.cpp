// LHNetworkLib::LHHttp2::Open() -- open the HTTP connection socket: 1 without a host name; otherwise create the socket and open it from ANY:0 to the proxy (when one is set) or the host, 0 on success, 3 on failure
// Leaf: ?Open@LHHttp2@LHNetworkLib@@QAE
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083F9B0, 360 bytes (+ int3 padding), thiscall `ret`.
// Identity: Ego_r.pdb ?Open@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@XZ (the no-argument overload; the debug build's
//   0x0084E0B0). The catalog's "LHHttp_InitializeConnection" is descriptive.
// Callees: operator new 0x00BFEA1A (0x9C bytes), LSocket::LSocket 0x008F8A80, LAddress::LAddress() 0x00841E90 (landed),
//   operator!=(const string&, const char*) 0x00566EA0 (cdecl, out of line), LAddress::LAddress(const string&, unsigned
//   short) 0x00841EE0 (landed), LAddress::operator= 0x00841F60 (landed), malloc 0x00BFEA0E / memmove 0x00BFEAE6 (the
//   inline string("ANY")), free 0x00BFEA14, LSocket::Open(const LAddress&, const LAddress&, bool) 0x008F9820.
// Data: "" 0x0122D70E, "ANY" 0x01273128.
// Layout: retail LHHttp2 (the landed ~LHHttp2 0x008410D0 model): Host +0, Hostname +4, Port +0x10, ProxyHost +0x874,
//   ProxyPort +0x880 (unsigned long, passed as unsigned short); LAddress 0x1C bytes (12-byte STLport string first).
// Shape: `Hostname == ""` is the inline STLport size test + zero-length memcmp; the proxy/host choice assigns a temporary
//   LAddress (one shared constructor/operator=/free tail for both arms); the local address is built from the inline
//   string("ANY") temporary (malloc(4), memmove 3, terminator), whose block pointer stays in esi for its free because
//   the TU knows LAddress(const string&, unsigned short) only reads the name and STLport's _M_throw_length_error has a
//   body here (an opaque callee taking either address forces the pointer back through memory).
#include <string.h>
#pragma intrinsic(memcmp, strlen)

extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6
extern "C" int __cdecl puts(const char* text);
extern "C" void __cdecl abort();

namespace std {

struct __false_type { __false_type() { } };

template <class _CharT> class char_traits {
public:
    static unsigned int length(const _CharT* __s) { return strlen(__s); }
    static int compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n) { return memcmp(__s1, __s2, __n); }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}
inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _String_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    unsigned int max_size() const { return ((unsigned int)-1 / sizeof(_Tp)) - 1; }
    void _M_allocate_block(unsigned int __n)
    {
        if ((__n <= (max_size() + 1)) && (__n > 0)) {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        }
        else
            _M_throw_length_error();
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    __declspec(noinline) void _M_throw_length_error() const                            // 0x00565A30 (retail throws)
    {
        puts("basic_string");
        abort();
    }
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef basic_string<_CharT, _Traits, _Alloc> _Self;
    typedef typename _String_base<_CharT, _Alloc>::allocator_type allocator_type;

    basic_string() : _String_base<_CharT, _Alloc>(allocator_type())
    {
        this->_M_allocate_block(8);
        _M_terminate_string();
    }
    __forceinline basic_string(const _CharT* __s, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + _Traits::length(__s));
    }
    ~basic_string() { }
    _Self& operator=(const _Self& __s)
    {
        if (&__s != this)
            _M_assign_dispatch(__s._M_start, __s._M_finish, __false_type());
        return *this;
    }
    _Self& operator=(const _CharT* __s) { return assign(__s, __s + _Traits::length(__s)); }

    size_type size() const { return this->_M_finish - this->_M_start; }
    const _CharT* data() const { return this->_M_start; }
    _Self& assign(const _CharT* __f, const _CharT* __l);                               // 0x005693B0
    _Self& _M_assign_dispatch(const _CharT* __f, const _CharT* __l, const __false_type&);   // 0x00569140

    template <class _InputIter>
    __forceinline void _M_range_initialize(_InputIter __f, _InputIter __l)
    {
        int __n = __l - __f;
        this->_M_allocate_block(__n + 1);
        this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
        _M_terminate_string();
    }
    void _M_construct_null(_CharT* __p) { *__p = _CharT(); }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
};

template <class _CharT, class _Traits, class _Alloc>
inline bool operator==(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __c)
{
    unsigned int __n = _Traits::length(__c);
    return __s.size() == __n && _Traits::compare(__s.data(), __c, __n) == 0;
}
template <class _CharT, class _Traits, class _Alloc>
bool operator!=(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __c);  // 0x00566EA0

typedef basic_string<char> string;
}

namespace LHNetworkLib {

struct LAddress {                                                // retail 0x1C bytes
    std::string    ServerName;                                   // +0x00
    unsigned long  ServerIP;                                     // +0x0c
    unsigned short ServerPort;                                   // +0x10
    long           TransportProtocol;                            // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;                               // +0x18 LSOCKET_FLAGS
    LAddress();                                                  // 0x00841E90
    LAddress(const std::string& serverName, unsigned short serverPort);   // 0x00841EE0
    LAddress& operator=(const LAddress& other);                  // 0x00841F60
};

#pragma auto_inline(off)
LAddress::LAddress(const std::string& serverName, unsigned short serverPort)   // 0x00841EE0 (landed)
{
    ServerName = "";
    ServerPort = 0;
    ServerIP = 0;
    TransportProtocol = 0;
    TransportFlags = 0;
    ServerName = serverName;
    ServerPort = serverPort;
}
#pragma auto_inline(on)

class LSocket {
public:
    unsigned char _body[0x9c];
    LSocket();                                                   // 0x008F8A80
    bool Open(const LAddress& remote, const LAddress& local, bool blocking);   // 0x008F9820
};

enum LH_RETURN { LH_OK = 0, LH_NO_HOST = 1, LH_OPEN_FAILED = 3 };

class LHHttp2 { public:
    LSocket*       Host;                                         // +0x000
    std::string    Hostname;                                     // +0x004
    unsigned short Port;                                         // +0x010
    unsigned char  _pad012[0x874 - 0x12];                        // +0x012 PreparedRequest .. CurrentHeaderPos
    std::string    ProxyHost;                                    // +0x874
    unsigned long  ProxyPort;                                    // +0x880
    LH_RETURN Open();
};

LH_RETURN LHHttp2::Open()
{
    if (Hostname == "")
        return LH_NO_HOST;
    Host = new LSocket;
    LAddress remote;
    if (ProxyHost != "")
        remote = LAddress(ProxyHost, (unsigned short)ProxyPort);
    else
        remote = LAddress(Hostname, Port);
    LAddress local(std::string("ANY"), 0);
    if (Host->Open(remote, local, true))
        return LH_OK;
    return LH_OPEN_FAILED;
}

}