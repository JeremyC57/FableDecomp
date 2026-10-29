#include <stdio.h>
namespace std { template <class _CharT, class _IntT> class __char_traits_base { public:
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n); }; }
template <> int __cdecl std::__char_traits_base<unsigned short, unsigned short>::compare(const unsigned short*, const unsigned short*, unsigned int);
typedef std::__char_traits_base<unsigned short, unsigned short> wtraits;
int main()
{
    unsigned short a[4]={1,2,3,4}; unsigned short b[4]={1,2,3,4};
    if(wtraits::compare(a,b,0)!=0 || wtraits::compare(a,b,4)!=0)return 1;
    b[2]=9; if(wtraits::compare(a,b,4)!=-1)return 2;
    b[2]=0; if(wtraits::compare(a,b,4)!=1)return 3;
    printf("MEMCMP_UNSIGNED16_TEST PASS\n"); return 0;
}
