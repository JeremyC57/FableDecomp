// Behaviour test for 0x00568440 _STL::basic_string<char>::basic_string(const char*, const allocator&): start
// empty (null start/finish/end of storage), then range-initialise from [s, s + strlen(s))
#include <stdio.h>
#include <string.h>
static const char* gFirst = 0;
static const char* gLast = 0;
static int gInitCalls = 0;
static bool gEmptyAtInit = false;
namespace _STL {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
template <class _CharT> struct char_traits {
    static unsigned int length(const char* __s) { return (unsigned int)strlen(__s); }
};
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _String_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _String_base(const allocator_type& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef _Traits traits_type;
    typedef typename _String_base<_CharT, _Alloc>::allocator_type allocator_type;
    basic_string(const _CharT* __s, const allocator_type& __a = allocator_type())
        : _String_base<_CharT, _Alloc>(__a)
    {
        _M_range_initialize(__s, __s + traits_type::length(__s), forward_iterator_tag());
    }
    void _M_range_initialize(const _CharT* __f, const _CharT* __l, const forward_iterator_tag&)   // stand-in
    {
        gFirst = __f; gLast = __l; ++gInitCalls;
        gEmptyAtInit = this->_M_start == 0 && this->_M_finish == 0 && this->_M_end_of_storage._M_data == 0;
    }
};
}
static const char gText[] = "SG_MINION_WANDER";
static const char gEmpty[] = "";
int main()
{
    bool ok = true;
    _STL::basic_string<char> s(gText);
    ok = ok && gInitCalls == 1 && gFirst == gText && gLast == gText + 16 && gEmptyAtInit;
    _STL::allocator<char> a;
    _STL::basic_string<char> e(gEmpty, a);
    ok = ok && gInitCalls == 2 && gFirst == gEmpty && gLast == gEmpty && gEmptyAtInit;
    if (ok) { printf("00568440_TEST PASS\n"); return 0; }
    printf("00568440_TEST FAIL\n");
    return 1;
}