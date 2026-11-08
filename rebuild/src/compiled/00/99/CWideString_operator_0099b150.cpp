// 0099b150 CWideString::operator!=
// Flags: /Zc:wchar_t
// Leaf: ??9CWideString@@QBE_NPB_W@Z
#include "fable_compiler.h"
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#endif
extern "C" int __cdecl wcscmp(const wchar_t*,const wchar_t*);
namespace std{
template<class T>class allocator{};
template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};
template<class C,class I>class __char_traits_base {public:
 static int __cdecl compare(const C*,const C*,size_t);
 static FABLE_FORCEINLINE bool eq(const C&a,const C&b){return a==b;}
 static FABLE_FORCEINLINE size_t length(const C*s){const C zero=C();size_t n;for(n=0;!eq(s[n],zero);++n){}return n;}
};
template<class C>class char_traits:public __char_traits_base<C,C>{};
// MSVC7.1 CRT wint_t is unsigned short; preserve the real wchar specialization.
template<>class char_traits<wchar_t>:public __char_traits_base<wchar_t,unsigned short>{};
template<class C,class A>class _String_base {public:C*_M_start;C*_M_finish;_STLP_alloc_proxy<C*,C,A>_M_end_of_storage;};
template<class C,class T,class A>class basic_string:protected _String_base<C,A>{public:
 FABLE_FORCEINLINE size_t size()const{return this->_M_finish-this->_M_start;}
 FABLE_FORCEINLINE const C*data()const{return this->_M_start;}
};
template<class C,class T,class A>FABLE_FORCEINLINE bool __cdecl operator==(const basic_string<C,T,A>&x,const C*s){size_t n=T::length(s);return x.size()==n&&T::compare(x.data(),s,n)==0;}
}
// Layout: donorCWidestringData contains Data thenNoRefs; STLport shrinksData
// to12bytes inretail. PStringData0; Data starts0/finish4/end8; NoRefs12.
class CWideStringData{public:std::basic_string<wchar_t,std::char_traits<wchar_t>,std::allocator<wchar_t> >Data;long NoRefs;
 FABLE_FORCEINLINE bool operator==(const wchar_t*s)const{return Data==s;}
};
class CWideString{public:CWideStringData*PStringData;
 FABLE_FORCEINLINE bool operator==(const wchar_t*s)const{if(PStringData==0)return wcscmp(s,L"")==0;return *PStringData==s;}
 bool operator!=(const wchar_t*)const;
};
bool CWideString::operator!=(const wchar_t*s)const{return !(*this==s);}