#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 c970aad44eb67891b47ffcc912a98fb2cbf0fd8e7cd01ad484319fa20d1d8698
// 0052c150 STL::Vector24_symbol24::InsertGrow_0052c150
// Flags: /O2 /Oy /Gr
// Leaf: InsertGrow
// Layout: element stride24 derived from retail pointer division and allocation; concrete identity uncertain except cited caller.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
struct Tag_0052c150 {Tag_0052c150(){}};
// Layout: CCharString header absent; retail ctor0099ec30, dtor0099eae0, single pointer.
struct CCharString {void* data;CCharString():data(0){} CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
// Layout: CSymbolInfo has string+0, vector12+4, int+16, byte+20; stride24, caller CSymbolMap::Insert.
struct Inner_0052c150{int* start;int* finish;int* end;Inner_0052c150():start(0),finish(0),end(0){}Inner_0052c150(const Inner_0052c150&);~Inner_0052c150();};
struct Element_0052c150{CCharString name;Inner_0052c150 vector;int kind;char flag;Element_0052c150(){}__forceinline Element_0052c150(const Element_0052c150& o):name(o.name),vector(o.vector),kind(o.kind),flag(o.flag){}};
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct Vector_0052c150 {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void InsertGrow(T*,const T&,const Tag_0052c150&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) Element_0052c150* __fastcall UninitializedCopy(const Element_0052c150* first,const Element_0052c150* last,Element_0052c150* out,const Tag_0052c150&) {
 for(;first!=last;++first,++out)if(out)new(out)Element_0052c150(*first);
 return out;
}
__declspec(noinline) Element_0052c150* __fastcall UninitializedFill(Element_0052c150* out,unsigned int count,const Element_0052c150& value,const Tag_0052c150&) {
 for(;count;--count,++out)if(out)new(out)Element_0052c150(value);
 return out;
}
__declspec(noinline) void __fastcall DestroyRange(Element_0052c150* first,Element_0052c150* last,const Tag_0052c150&) {
 for(;first!=last;++first)first->~Element_0052c150();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& Max(const T&a,const T&b){return a<b?b:a;}
template<class T> void Vector_0052c150<T>::InsertGrow(T* position,const T& value,const Tag_0052c150&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+Max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=UninitializedCopy(start,position,newStart,Tag_0052c150());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=UninitializedFill(newFinish,count,value,Tag_0052c150());
 if(!atEnd)newFinish=UninitializedCopy(position,finish,newFinish,Tag_0052c150());
 DestroyRange(start,finish,Tag_0052c150());if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void Vector_0052c150<Element_0052c150>::InsertGrow(Element_0052c150*,const Element_0052c150&,const Tag_0052c150&,unsigned int,bool);

// END exact source snapshot
CCharString::CCharString(const CCharString& o):data(o.data){} CCharString::~CCharString(){} CCharString& CCharString::operator=(const CCharString& o){data=o.data;return *this;}
Inner_0052c150::Inner_0052c150(const Inner_0052c150& o):start(o.start),finish(o.finish),end(o.end){}Inner_0052c150::~Inner_0052c150(){}
template<class T> void Vector_0052c150<T>::Clear(){for(T* p=start;p!=finish;++p)p->~T();if(start)free(start);}

int main(){Vector_0052c150<Element_0052c150> v;v.start=(Element_0052c150*)malloc(sizeof(Element_0052c150));v.finish=v.start;v.end=v.start+1;Tag_0052c150 tag;Element_0052c150 value;int key=8;value.name.data=&key;value.kind=12;value.flag=1;
v.InsertGrow(v.start,value,tag,1,true);assert(v.finish-v.start==1 && v.end-v.start==1);
v.InsertGrow(v.start,value,tag,2,false);assert(v.finish-v.start==3 && v.end-v.start==3);assert(v.start[0].name.data==&key && v.start[2].kind==12 && v.start[2].flag==1);
v.Clear();puts("0052c150_TEST PASS");return 0;}