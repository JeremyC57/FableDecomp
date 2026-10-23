// 004110d0 Row004110d0::Vector::Insert
// Flags: /O2 /Oy /W3 
// Leaf: Insert
#include <string.h>
#include <stdlib.h>
namespace Row004110d0 {
struct Tag { Tag(){} };
__forceinline char* CopyChars(const char*first,const char*last,char*out){if(last!=first)memmove(out,first,last-first);return out+(last-first);}
__forceinline char* CopyBackward(const char*first,const char*last,char*out){int count=last-first;if(count>0)memmove(out-count,first,count);return out-count;}
__forceinline void FillChars(char*first,char*last,const char&value){memset(first,(unsigned char)value,last-first);}
// Layout: retail [ecx+0,4,8] is STLport vector<char> storage.
__forceinline char* FillN(char*first,unsigned count,const char&value){memset(first,(unsigned char)value,count);return first+count;}
template<class T> struct Vector {
 T *first,*last,*limit;
 void Overflow(T*,const T&,const Tag&,unsigned,bool);
 void Insert(T* pos,unsigned n,const T& value);
};
template<class T> void Vector<T>::Insert(T*pos,unsigned n,const T&value) {
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
  } else { Tag t; Overflow(pos,value,t,n,false); }
 }
}
template struct Vector<char>;
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace Row004110d0 {
static int overflows;
template<>void Vector<char>::Overflow(char*pos,const char&value,const Tag&,unsigned n,bool atEnd){assert(!atEnd);++overflows;unsigned off=pos-first,old=last-first;char copy=value;char*q=(char*)malloc(old+n);memcpy(q,first,off);memset(q+off,(unsigned char)copy,n);memcpy(q+off+n,pos,old-off);free(first);first=q;last=q+old+n;limit=last;}
void Reset(Vector<char>&v,const unsigned char*p,unsigned n,unsigned capacity){if(v.first)free(v.first);v.first=(char*)malloc(capacity);memset(v.first,0x55,capacity);memcpy(v.first,p,n);v.last=v.first+n;v.limit=v.first+capacity;}
void Check(Vector<char>&v,const unsigned char*p,unsigned n){assert(unsigned(v.last-v.first)==n&&!memcmp(v.first,p,n));}
}
int main(){using namespace Row004110d0;Vector<char>v;v.first=v.last=v.limit=0;unsigned char base[4]={0x10,0xfe,0x30,0x40};char value=(char)0x80;
 Reset(v,base,4,32);v.Insert(v.first+1,2,value);unsigned char a[6]={0x10,0x80,0x80,0xfe,0x30,0x40};Check(v,a,6);
 value=(char)0xe5;v.Insert(v.last,3,value);unsigned char b[9]={0x10,0x80,0x80,0xfe,0x30,0x40,0xe5,0xe5,0xe5};Check(v,b,9);char*old=v.first;v.Insert(v.first,0,value);assert(v.first==old&&overflows==0);Check(v,b,9);
 Reset(v,base,4,32);value=(char)0x80;v.Insert(v.first+2,2,value);unsigned char c[6]={0x10,0xfe,0x80,0x80,0x30,0x40};Check(v,c,6);
 Reset(v,base,4,32);value=(char)0x9a;v.Insert(v.first+3,3,value);unsigned char d[7]={0x10,0xfe,0x30,0x9a,0x9a,0x9a,0x40};Check(v,d,7);
 Reset(v,base,4,32);v.Insert(v.first,2,v.first[2]);unsigned char e[6]={0x30,0x30,0x10,0xfe,0x30,0x40};Check(v,e,6);
 Reset(v,base,4,4);v.Insert(v.first+2,5,v.first[1]);unsigned char f[9]={0x10,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0x30,0x40};Check(v,f,9);assert(overflows==1);free(v.first);puts("004110d0_TEST PASS");}