// LHNetworkLib::LHHttpHeaderStatus::LHHttpHeaderStatus() -- HTTP response status record: build the header block and the three strings, then reset code, location, host, URI and port
// Leaf: ??0LHHttpHeaderStatus
// Retail: Fable.exe (TLC, VC7.1) @ 0x00569C50, 218 bytes, thiscall, plain `ret` (returns this). Callers:
//   CTCHeroOnlineScoreboard::UpdateTransaction 0x0055FB80 (landed; it names this the 0x48-byte "CHttpResponse"
//   record passed to LHHttp2::ParseResponseLine) at 0x0055FBA0 / 0x0055FBC6, and 0x008411E3 in the LHHttp2 code.
//   Ego_r.pdb names the type: `??0LHHttpHeaderStatus@LHNetworkLib@@QAE@XZ`. The catalog label
//   "Transaction_InitializeVectors" is descriptive.
// Callees: malloc @ 0x00BFEA0E (each STLport string's 8-byte initial block), basic_string::assign(const char*,
//   const char*) @ 0x005693B0 (x3, `= ""`).
// Layout: Ego_r.pdb LHHttpHeaderStatus with retail STLport 12-byte strings/vectors (0x48 bytes): hsServerCode +0,
//   hsDocumentHeader +4 (LHHttpHeaders: vector<string> HdrDocumentHeader +4, string HeaderString +0x10),
//   hsLocation +0x1C, hsDocumentSize +0x28 (left unset), hsHost +0x2C, hsURI +0x38, hsPort +0x44 (ushort).
// Shape: members built in declaration order (vector: three zeros; each string: the STLport 4.5 default ctor --
//   zeroed _String_base, malloc(8), terminator), then the body resets the status: code = -1, the three
//   strings `= ""` (operator=(const char*) -> assign(s, s + length(s)), the length folding to 0), port = 0.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" unsigned int __cdecl strlen(const char* text);
#pragma intrinsic(strlen)

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

class _String_base {
public:
    typedef allocator<char> allocator_type;
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (char*)0) { }
};

class basic_string : public _String_base {
public:
    basic_string() : _String_base(allocator_type())
    {
        _M_start = _M_end_of_storage.allocate(8);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + 8;
        _M_terminate_string();
    }
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }
    basic_string& assign(const char* __f, const char* __l);                    // 0x005693B0
    basic_string& operator=(const char* __s) { return assign(__s, __s + strlen(__s)); }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
};

typedef basic_string string;

}

namespace LHNetworkLib {

enum LH_HTTP_SERVER_CODES { LH_HTTP_SERVER_CODE_NONE = -1 };

class LHHttpHeaders { public:                                      // 0x18 in retail
    std::vector<std::string> HdrDocumentHeader;             // +0x00
    std::string              HeaderString;                  // +0x0C
};

struct LHHttpHeaderStatus {                                 // 0x48 in retail
    LH_HTTP_SERVER_CODES hsServerCode;                      // +0x00
    LHHttpHeaders        hsDocumentHeader;                  // +0x04
    std::string          hsLocation;                        // +0x1C
    long                 hsDocumentSize;                    // +0x28
    std::string          hsHost;                            // +0x2C
    std::string          hsURI;                             // +0x38
    unsigned short       hsPort;                            // +0x44
    LHHttpHeaderStatus();
};

LHHttpHeaderStatus::LHHttpHeaderStatus()
{
    hsServerCode = LH_HTTP_SERVER_CODE_NONE;
    hsLocation = "";
    hsHost = "";
    hsURI = "";
    hsPort = 0;
}

}