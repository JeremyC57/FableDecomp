#include <stdio.h>
#include <string.h>
// 0099b150 CWideString::operator!=
// Flags: /Zc:wchar_t
// Leaf: ??9CWideString@@QBE_NPB_W@Z
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
 static __forceinline bool eq(const C&a,const C&b){return a==b;}
 static __forceinline size_t length(const C*s){const C zero=C();size_t n;for(n=0;!eq(s[n],zero);++n){}return n;}
};
template<class C>class char_traits:public __char_traits_base<C,C>{};
// MSVC7.1 CRT wint_t is unsigned short; preserve the real wchar specialization.
template<>class char_traits<wchar_t>:public __char_traits_base<wchar_t,unsigned short>{};
template<class C,class A>class _String_base {public:C*_M_start;C*_M_finish;_STLP_alloc_proxy<C*,C,A>_M_end_of_storage;};
template<class C,class T,class A>class basic_string:protected _String_base<C,A>{public:
 __forceinline size_t size()const{return this->_M_finish-this->_M_start;}
 __forceinline const C*data()const{return this->_M_start;}
};
template<class C,class T,class A>__forceinline bool __cdecl operator==(const basic_string<C,T,A>&x,const C*s){size_t n=T::length(s);return x.size()==n&&T::compare(x.data(),s,n)==0;}
}
// Layout: donorCWidestringData contains Data thenNoRefs; STLport shrinksData
// to12bytes inretail. PStringData0; Data starts0/finish4/end8; NoRefs12.
class CWideStringData{public:std::basic_string<wchar_t,std::char_traits<wchar_t>,std::allocator<wchar_t> >Data;long NoRefs;
 __forceinline bool operator==(const wchar_t*s)const{return Data==s;}
};
class CWideString{public:CWideStringData*PStringData;
 __forceinline bool operator==(const wchar_t*s)const{if(PStringData==0)return wcscmp(s,L"")==0;return *PStringData==s;}
 bool operator!=(const wchar_t*)const;
};
bool CWideString::operator!=(const wchar_t*s)const{return !(*this==s);}

static unsigned int compareCalls;
namespace std{
template<class C,class I>int __char_traits_base<C,I>::compare(const C*a,const C*b,size_t n){++compareCalls;for(size_t k=0;k<n;++k)if(a[k]!=b[k])return a[k]<b[k]?-1:1;return 0;}
}
int main(){
 wchar_t text[63][8];unsigned int lengths[63];memset(text,0,sizeof(text));
 for(unsigned int i=0;i<63;++i){unsigned int n=0,q=i+1;while(q>1){text[i][n++]=(q&1)?(wchar_t)0xfffe:(wchar_t)1;q>>=1;}lengths[i]=n;}
 CWideString subject;CWideStringData block;block.NoRefs=0x12345;unsigned int cases=0,expectedCalls=0;
 for(unsigned int i=0;i<63;++i){
  subject.PStringData=0;
  if((subject!=text[i])!=(lengths[i]!=0)||subject.PStringData!=0)return 1;
  ++cases;
  for(unsigned int j=0;j<63;++j){
   wchar_t*words[3]={text[i],text[i]+lengths[i],text[i]+8};memcpy(&block.Data,words,sizeof(words));subject.PStringData=&block;
   unsigned char before[sizeof(block)];memcpy(before,&block,sizeof(block));wchar_t saved[8];memcpy(saved,text[j],sizeof(saved));
   bool different=lengths[i]!=lengths[j];
   if(!different){++expectedCalls;for(unsigned int k=0;k<lengths[i];++k)if(text[i][k]!=text[j][k])different=true;}
   if((subject!=text[j])!=different)return 2;
   if(subject.PStringData!=&block||memcmp(before,&block,sizeof(block))||memcmp(saved,text[j],sizeof(saved)))return 3;
   ++cases;
  }
 }
 if(cases!=4032||compareCalls!=expectedCalls||expectedCalls!=1365)return 4;
 puts("0099b150_TEST PASS");return 0;
}