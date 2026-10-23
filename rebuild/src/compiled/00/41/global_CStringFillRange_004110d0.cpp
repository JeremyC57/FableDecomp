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