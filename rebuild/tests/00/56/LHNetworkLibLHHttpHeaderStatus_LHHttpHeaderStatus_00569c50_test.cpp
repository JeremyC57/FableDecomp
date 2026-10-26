// Behaviour test for 0x00569C50 LHNetworkLib::LHHttpHeaderStatus::LHHttpHeaderStatus().
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_assigns = 0;
namespace stl {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
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
    ~basic_string() { std::free(_M_start); }
    void _M_construct_null(char* __p) { *__p = 0; }
    void _M_terminate_string() { _M_construct_null(_M_finish); }
    basic_string& assign(const char* __f, const char* __l)      // stand-in for 0x005693B0 (fits the 8-byte block)
    {
        ++g_assigns;
        unsigned int n = (unsigned int)(__l - __f);
        std::memcpy(_M_start, __f, n);
        _M_finish = _M_start + n;
        *_M_finish = 0;
        return *this;
    }
    basic_string& operator=(const char* __s) { return assign(__s, __s + std::strlen(__s)); }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
typedef basic_string string;
}

namespace LHNetworkLib {
enum LH_HTTP_SERVER_CODES { LH_HTTP_SERVER_CODE_NONE = -1 };
class LHHttpHeaders { public:
    stl::vector<stl::string> HdrDocumentHeader;
    stl::string              HeaderString;
};
struct LHHttpHeaderStatus {
    LH_HTTP_SERVER_CODES hsServerCode;
    LHHttpHeaders        hsDocumentHeader;
    stl::string          hsLocation;
    long                 hsDocumentSize;
    stl::string          hsHost;
    stl::string          hsURI;
    unsigned short       hsPort;
    LHHttpHeaderStatus()
    {
        hsServerCode = LH_HTTP_SERVER_CODE_NONE;
        hsLocation = "";
        hsHost = "";
        hsURI = "";
        hsPort = 0;
    }
};
}

static int Empty(const stl::string& s)
{
    return s._M_start != 0 && s._M_finish == s._M_start && s._M_end_of_storage._M_data == s._M_start + 8 && *s._M_start == 0;
}

int main()
{
    LHNetworkLib::LHHttpHeaderStatus st;
    st.hsDocumentSize = 0;
    int ok = st.hsServerCode == -1 && st.hsPort == 0 && Empty(st.hsLocation) && Empty(st.hsHost) && Empty(st.hsURI)
        && Empty(st.hsDocumentHeader.HeaderString) && st.hsDocumentHeader.HdrDocumentHeader._M_start == 0
        && st.hsDocumentHeader.HdrDocumentHeader._M_finish == 0 && st.hsDocumentHeader.HdrDocumentHeader._M_end_of_storage._M_data == 0
        && g_assigns == 3;
    if (!ok) { std::printf("00569c50_TEST FAIL\n"); return 1; }
    std::printf("00569c50_TEST PASS\n");
    return 0;
}