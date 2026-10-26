#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 13393a6a444a90f03a054919c86aee56c44422acec7f17d4f08520cc56ae0430
// 00ad5f80 STL::Vector16_string16::InsertGrow_00ad5f80
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride16 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_00ad5f80 {Tag_00ad5f80(){}};
// Layout: CCharString header absent; retail ctor0099ec30, dtor0099eae0, single pointer.
class CCharString { public:void* data;CCharString():data(0){} CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
struct Element_00ad5f80{CCharString name;int value;char flag;int index;Element_00ad5f80(){}__forceinline Element_00ad5f80(const Element_00ad5f80& o):name(o.name),value(o.value),flag(o.flag),index(o.index){}};
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_00ad5f80 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_00ad5f80&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_00ad5f80* __fastcall UninitializedCopy(const Element_00ad5f80* first,const Element_00ad5f80* last,Element_00ad5f80* out,const Tag_00ad5f80&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_00ad5f80(*first);
 return out;
}
__declspec(noinline) Element_00ad5f80* __fastcall UninitializedFill(Element_00ad5f80* out,unsigned int count,const Element_00ad5f80& value,const Tag_00ad5f80&) {
 for(;count;--count,++out)if(out)new(out)Element_00ad5f80(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_00ad5f80* first,Element_00ad5f80* last,const Tag_00ad5f80&) {
 for(;first!=last;++first)first->~Element_00ad5f80();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_00ad5f80<T>::InsertGrow(T* position,const T& value,const Tag_00ad5f80&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_00ad5f80());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_00ad5f80());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_00ad5f80());
 T* oldEnd=finish;for(T* i=start;i!=oldEnd;++i)i->name.~CCharString();if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_00ad5f80<Element_00ad5f80>::InsertGrow(Element_00ad5f80*,const Element_00ad5f80&,const Tag_00ad5f80&,unsigned int,bool);

// END exact source snapshot
CCharString::CCharString(const CCharString& o):data(o.data){} CCharString::~CCharString(){} CCharString& CCharString::operator=(const CCharString& o){data=o.data;return *this;}
template<class T> void Vector_00ad5f80<T>::Clear(){for(T* p=start;p!=finish;++p)p->~T();if(start)free(start);}

int main(){Vector_00ad5f80<Element_00ad5f80> v;v.start=(Element_00ad5f80*)malloc(sizeof(Element_00ad5f80));v.finish=v.start;v.end=v.start+1;Tag_00ad5f80 tag;Element_00ad5f80 value;int key=8;value.name.data=&key;value.value=12;value.flag=1;value.index=3;
v.InsertGrow(v.start,value,tag,1,true);assert(v.finish-v.start==1 && v.end-v.start==1);
v.InsertGrow(v.start,value,tag,2,false);assert(v.finish-v.start==3 && v.end-v.start==3);assert(v.start[0].name.data==&key && v.start[2].flag==1 && v.start[2].index==3);
v.Clear();puts("00ad5f80_TEST PASS");return 0;}