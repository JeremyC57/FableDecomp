// Test for 0x00404280 std::operator==(const basic_string<wchar_t>&, const wchar_t*) (STLport 4.6, wchar_t == unsigned short):
// equal length and contents -> true through one compare call; a different length short-circuits before compare.
#include <stdio.h>
static int g_compareCalls;
namespace std {
// STLport 4.6.2: char_traits / allocator / basic_string are `class` templates (mangled V); wint_t == unsigned short.
template <class _Tp> class allocator;
template <class _CharT, class _IntT> class __char_traits_base { public:
    typedef _CharT char_type;
    static bool __cdecl eq(const char_type& __c1, const char_type& __c2) { return __c1 == __c2; }
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60
    static unsigned int __cdecl length(const char_type* __s) {
        const _CharT _NullChar = _CharT();
        unsigned int __i;
        for (__i = 0; !eq(__s[__i], _NullChar); ++__i) {}
        return __i;
    }
};
template <class _CharT> class char_traits : public __char_traits_base<_CharT, _CharT> { };
template <class _CharT, class _Traits, class _Alloc> class basic_string { public:
    _CharT* _M_start;                     // _String_base<_CharT,_Alloc>
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const _CharT* data() const { return _M_start; }
};
}
namespace std {
template <class _CharT, class _Traits, class _Alloc>
bool __cdecl operator==(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __t);   // the row's instantiation
}
template <> int __cdecl std::__char_traits_base<unsigned short, unsigned short>::compare(const unsigned short* a,const unsigned short* b,unsigned int n)
{
    ++g_compareCalls; for(unsigned int i=0;i<n;++i)if(a[i]!=b[i])return a[i]<b[i]?-1:1; return 0;
}
typedef std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > wstring_t;
int main()
{
    unsigned short data[4]={65,66,67,0}; unsigned short same[4]={65,66,67,0};
    unsigned short diff[4]={65,66,68,0}; unsigned short shortText[3]={65,66,0};
    wstring_t s; s._M_start=data; s._M_finish=data+3; s._M_end_of_storage=data+4;
    if(!(s==same) || g_compareCalls!=1)return 1;
    if((s==diff) || g_compareCalls!=2)return 2;
    if((s==shortText) || g_compareCalls!=2)return 3;
    printf("WIDESTRING_EQUALS_LEN_TEST PASS\n"); return 0;
}
