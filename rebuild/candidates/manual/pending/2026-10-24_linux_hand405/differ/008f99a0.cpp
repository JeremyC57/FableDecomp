// LHNetworkLib::LSocket::Receive(vector<char>&) -- receive into the buffer, discarding the sender/receiver addresses (two empty LAddress locals)
// Leaf: ?Receive@LSocket@LHNetworkLib@@
// Grade: DIFFER(207v207) -- scheduling only: retail issues the first `push offset ""` before the end-of-storage store
//   and sinks each string's start/finish stores below the second push; the lane emits them between the pushes.
//   This best form drops STLport's _String_base zero mem-initialisers (retail has no zero stores for the two local
//   strings; the verbatim `_M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)` form keeps six dead zero stores in the
//   lane: 231v207). Tried: verbatim STLport (231), ServerPort before ServerIP in ClearData (fixes the scalar order),
//   zero-free base (207, this file), STLport 4.6 `_String_base(a, n)` allocating in the base ctor (207, same schedule).
// Retail 0x008F99A0, 0xCF bytes, thiscall `ret 4`, bool. Manifest label "LSocket_ReceiveWithTimeout" (_global) is
// descriptive. PDB name set: `?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@@Z`, the
// one-argument overload of `Receive(vector<char>&, LAddress&, const LAddress&)` (0x008F9320, its only callee besides
// the string helpers).
// Callees: malloc @ 0x00BFEA0E, basic_string::assign(const char*, const char*) @ 0x005693B0 (landed), LSocket::Receive
// (3 args) @ 0x008F9320, free @ 0x00BFEA14.
// Layout: retail LAddress (0x1C bytes, Ego_r names): ServerName +0x00 (12-byte STLport string), ServerIP +0x0C,
// ServerPort +0x10, TransportProtocol +0x14, TransportFlags +0x18.
// Shape: each LAddress is default-constructed inline: the STLport 4.5 default string (malloc(8), terminator), then
// `LAddress() { ClearData(); }` -- `ServerName = ""` (assign(s, s + 0): the literal pushed twice) and the four scalar
// zeroes from the shared zero register (scheduled after the next call's first push). The bool result is parked in the
// dead argument slot across the two destructors (null-tested free of each name block, second local first).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                      // 0x00BFEA14
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)

namespace std {

template <class _CharT> class char_traits {
public:
    static unsigned int length(const _CharT* __s) { return strlen(__s); }
};

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
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a) : _MaybeReboundAlloc(__a) { }
};

template <class _Tp, class _Alloc> class _String_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _String_base(const allocator_type& __a) : _M_end_of_storage(__a) { }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _CharT, class _Traits, class _Alloc>
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef _String_base<_CharT, _Alloc> _Base;
    typedef _Alloc allocator_type;
    basic_string() : _Base(allocator_type())
    {
        this->_M_start = this->_M_end_of_storage.allocate(8);
        this->_M_finish = this->_M_start;
        this->_M_end_of_storage._M_data = this->_M_start + 8;
        _M_terminate_string();
    }
    ~basic_string() { }
    void _M_construct_null(_CharT* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(this->_M_finish); }
    basic_string& assign(const _CharT* __f, const _CharT* __l);                  // 0x005693B0
    basic_string& operator=(const _CharT* __s) { return assign(__s, __s + _Traits::length(__s)); }
};

typedef basic_string<char, char_traits<char>, allocator<char> > string;

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                  // STLport vector, 12 bytes (only passed by reference here)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

}

namespace LHNetworkLib {

struct LAddress {                               // Ego_r.pdb names; retail 0x1C bytes
    std::string   ServerName;                  // +0x00
    unsigned long  ServerIP;                    // +0x0C
    unsigned short ServerPort;                  // +0x10
    long           TransportProtocol;           // +0x14 LSOCKET_PROTOCOL
    long           TransportFlags;              // +0x18 LSOCKET_FLAGS
    LAddress() { ClearData(); }
    void ClearData()
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = 0;
        TransportFlags = 0;
    }
};

class LSocket {
public:
    bool Receive(std::vector<char>& data, LAddress& from, const LAddress& to);    // 0x008F9320
    bool Receive(std::vector<char>& data);
};

bool LSocket::Receive(std::vector<char>& data)
{
    LAddress from;
    LAddress to;
    return Receive(data, from, to);
}

}
