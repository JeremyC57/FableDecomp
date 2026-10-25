// LHNetworkLib::LSocket::ResolveHost(LAddress&, unsigned long&) -- known IP: hand it out; host "ANY": INADDR_ANY; else resolve the name and cache it in the address
// Leaf: ?ResolveHost@LSocket@LHNetworkLib@@SI_N
// Retail 0x008F86F0, 0xA2 bytes, static __fastcall (address in ecx, ip in edx), plain `ret`, bool. Manifest label
// "Socket_InitWithDefault" (_global) is descriptive. PDB name set: `?ResolveHost@LSocket@LHNetworkLib@@SI_NAAULAddress@2@AAK@Z`.
// Callers: LSocket::ConvertLAddress2Sockaddr 0x008F8830 (ip = sockaddr_in::sin_addr).
// Callees: malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6, free @ 0x00BFEA14 (the STLport string temporary "ANY",
// literal 0x01273128), LSocket::ResolveHost(const string&) @ 0x008F86A0.
// Layout: retail LAddress (0x1C bytes, Ego_r names): ServerName +0x00 (12-byte STLport string), ServerIP +0x0C,
// ServerPort +0x10, TransportProtocol +0x14, TransportFlags +0x18.
// Shape: `address.ServerName == string("ANY")` builds a real temporary (STLport 4.x string(const char*) inlined:
// malloc(n + 1), __copy_trivial's memmove, terminator) and compares size then `memcmp == 0` (intrinsic `repe cmpsb`);
// the bool rides in a frame byte across the temporary's null-tested free. The constructor is `__forceinline` (VC7.1
// keeps it out of line otherwise). The address's IP is a named local (`serverIP`): retail stores eax at the shared
// `ip = ...` tail from both paths (the member form re-reads [ebx+0Ch]). The failure test reads the caller's `ip`,
// not the resolved value -- retail compares `[edi]` before any store (an original-source quirk, reproduced).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                      // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
extern "C" int   __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _CharT> class char_traits {
public:
    static unsigned int length(const char* __s) { return strlen(__s); }
    static int compare(const char* __s1, const char* __s2, unsigned int __n) { return memcmp(__s1, __s2, __n); }
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline char* uninitialized_copy(const char* __first, const char* __last, char* __result)
{
    return (char*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _String_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    void _M_allocate_block(unsigned int __n)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _CharT, class _Traits, class _Alloc>
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef _String_base<_CharT, _Alloc> _Base;
    typedef _Alloc allocator_type;
    __forceinline basic_string(const _CharT* __s, const allocator_type& __a = allocator_type()) : _Base(__a)
    {
        _M_range_initialize(__s, __s + _Traits::length(__s));
    }
    ~basic_string() { }
    void _M_construct_null(_CharT* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
    void _M_range_initialize(const _CharT* __f, const _CharT* __l)
    {
        int __n = __l - __f;
        this->_M_allocate_block(__n + 1);
        this->_M_finish = uninitialized_copy(__f, __l, this->_M_start);
        _M_terminate_string();
    }
    const _CharT* c_str() const { return this->_M_start; }
    const _CharT* data() const { return this->_M_start; }
    unsigned int size() const { return this->_M_finish - this->_M_start; }
};

template <class _CharT, class _Traits, class _Alloc>
inline bool operator==(const basic_string<_CharT, _Traits, _Alloc>& __x, const basic_string<_CharT, _Traits, _Alloc>& __y)
{
    return __x.size() == __y.size() && _Traits::compare(__x.data(), __y.data(), __x.size()) == 0;
}

typedef basic_string<char, char_traits<char>, allocator<char> > string;

}
namespace LHNetworkLib {

struct LAddress {                               // Ego_r.pdb names; retail 0x1C bytes (12-byte STLport string)
    std::string   ServerName;                  // +0x00
    unsigned long  ServerIP;                    // +0x0C
    unsigned short ServerPort;                  // +0x10
    long           TransportProtocol;           // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;              // +0x18 LSOCKET_FLAGS
};

class LSocket {
public:
    static unsigned long __fastcall ResolveHost(const std::string& host);          // 0x008F86A0
    static bool __fastcall ResolveHost(LAddress& address, unsigned long& ip);
};

bool __fastcall LSocket::ResolveHost(LAddress& address, unsigned long& ip)
{
    unsigned long serverIP = address.ServerIP;
    if (serverIP == 0) {
        if (address.ServerName == std::string("ANY")) {
            ip = 0;
            return true;
        }
        serverIP = ResolveHost(address.ServerName);
        address.ServerIP = serverIP;
        if (ip == (unsigned long)-1)
            return false;
    }
    ip = serverIP;
    return true;
}

}