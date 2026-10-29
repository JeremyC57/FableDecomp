#include <stdio.h>
struct WideStringRange { unsigned short* begin; unsigned short* end; };
int __cdecl WideString_EqualsLen(const WideStringRange*,const unsigned short*);
static int g_compareCalls;
namespace std {
template <class _CharT, class _IntT> class __char_traits_base { public:   // STLport 4.6 char_traits.h; wint_t == unsigned short
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60
};
}
template <> int __cdecl std::__char_traits_base<unsigned short, unsigned short>::compare(const unsigned short* a,const unsigned short* b,unsigned int n)
{
    ++g_compareCalls; for(unsigned int i=0;i<n;++i)if(a[i]!=b[i])return a[i]<b[i]?-1:1; return 0;
}
int main()
{
    unsigned short data[4]={65,66,67,0}; unsigned short same[4]={65,66,67,0};
    unsigned short diff[4]={65,66,68,0}; unsigned short shortText[3]={65,66,0};
    WideStringRange range={data,data+3};
    if(!WideString_EqualsLen(&range,same) || g_compareCalls!=1)return 1;
    if(WideString_EqualsLen(&range,diff) || g_compareCalls!=2)return 2;
    if(WideString_EqualsLen(&range,shortText) || g_compareCalls!=2)return 3;
    printf("WIDESTRING_EQUALS_LEN_TEST PASS\n"); return 0;
}
