// 004110d0 _STL::vector<char>::_M_fill_insert (catalog: CString_FillRange)
// Flags: /O2 /Oy /W3 
// Leaf: _M_fill_insert
#include <string.h>
#include <stdlib.h>
struct __true_type { __true_type(){} };
namespace _STL {
__forceinline char* CopyChars(const char*first,const char*last,char*out){if(last!=first)memmove(out,first,last-first);return out+(last-first);}
__forceinline char* CopyBackward(const char*first,const char*last,char*out){int count=last-first;if(count>0)memmove(out-count,first,count);return out-count;}
__forceinline void FillChars(char*first,char*last,const char&value){memset(first,(unsigned char)value,last-first);}
// Layout: retail [ecx+0,4,8] is STLport vector<char> storage.
__forceinline char* FillN(char*first,unsigned count,const char&value){memset(first,(unsigned char)value,count);return first+count;}
template<class T> struct vector {
 T *first,*last,*limit;
 void _M_insert_overflow(T*,const T&,const __true_type&,unsigned,bool);
 void _M_fill_insert(T* pos,unsigned n,const T& value);
};
template<class T> void vector<T>::_M_fill_insert(T*pos,unsigned n,const T&value) {
 if(n) {
  if(unsigned(limit-last)>=n) {
   T copy=value; unsigned after=last-pos; T* old=last;
   if(after>n) {
    CopyChars(old-n,old,old);
    last+=n;
    CopyBackward(pos,old-n,old);
    FillChars(pos,pos+n,copy);
   } else {
    FillN(last,n-after,copy); last+=n-after;
    CopyChars(pos,old,last);
    last+=after; FillChars(pos,old,copy);
   }
  } else { __true_type t; _M_insert_overflow(pos,value,t,n,false); }
 }
}
template struct vector<char>;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace _STL {
static int overflows;
template<>void vector<char>::_M_insert_overflow(char*pos,const char&value,const __true_type&,unsigned n,bool atEnd){assert(!atEnd);++overflows;unsigned off=pos-first,old=last-first;char copy=value;char*q=(char*)malloc(old+n);memcpy(q,first,off);memset(q+off,(unsigned char)copy,n);memcpy(q+off+n,pos,old-off);free(first);first=q;last=q+old+n;limit=last;}
void Reset(vector<char>&v,const unsigned char*p,unsigned n,unsigned capacity){if(v.first)free(v.first);v.first=(char*)malloc(capacity);memset(v.first,0x55,capacity);memcpy(v.first,p,n);v.last=v.first+n;v.limit=v.first+capacity;}
void Check(vector<char>&v,const unsigned char*p,unsigned n){assert(unsigned(v.last-v.first)==n&&!memcmp(v.first,p,n));}
}
int main(){using namespace _STL;vector<char>v;v.first=v.last=v.limit=0;unsigned char base[4]={0x10,0xfe,0x30,0x40};char value=(char)0x80;
 Reset(v,base,4,32);v._M_fill_insert(v.first+1,2,value);unsigned char a[6]={0x10,0x80,0x80,0xfe,0x30,0x40};Check(v,a,6);
 value=(char)0xe5;v._M_fill_insert(v.last,3,value);unsigned char b[9]={0x10,0x80,0x80,0xfe,0x30,0x40,0xe5,0xe5,0xe5};Check(v,b,9);char*old=v.first;v._M_fill_insert(v.first,0,value);assert(v.first==old&&overflows==0);Check(v,b,9);
 Reset(v,base,4,32);value=(char)0x80;v._M_fill_insert(v.first+2,2,value);unsigned char c[6]={0x10,0xfe,0x80,0x80,0x30,0x40};Check(v,c,6);
 Reset(v,base,4,32);value=(char)0x9a;v._M_fill_insert(v.first+3,3,value);unsigned char d[7]={0x10,0xfe,0x30,0x9a,0x9a,0x9a,0x40};Check(v,d,7);
 Reset(v,base,4,32);v._M_fill_insert(v.first,2,v.first[2]);unsigned char e[6]={0x30,0x30,0x10,0xfe,0x30,0x40};Check(v,e,6);
 Reset(v,base,4,4);v._M_fill_insert(v.first+2,5,v.first[1]);unsigned char f[9]={0x10,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0x30,0x40};Check(v,f,9);assert(overflows==1);free(v.first);puts("004110d0_TEST PASS");}