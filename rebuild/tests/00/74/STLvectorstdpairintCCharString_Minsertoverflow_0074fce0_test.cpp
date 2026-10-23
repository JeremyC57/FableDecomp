// 0074fce0 _STL::vector<std::pair<int,CCharString>>::_M_insert_overflow
// Flags: /Gr
// Leaf: _M_insert_overflow
// Layout: vector3 pointers; pair stride8 and CCharString at+4 from ctor99ec30 indexed access.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
namespace FableVector_0074fce0 {
struct __false_type {__false_type(){}};
// Layout: CCharString header absent; retail ctor0099ec30, dtor0099eae0, single pointer.
struct CCharString {void* data;CCharString():data(0){} CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
template<class K,class V> struct pair {K first;V second;pair(){} __forceinline pair(const pair& o):first(o.first),second(o.second){}};
typedef pair<int,CCharString> CKeyStringPair;
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct vector {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);
};
#pragma auto_inline(off)
__declspec(noinline) CKeyStringPair* __fastcall __uninitialized_copy(const CKeyStringPair* first,const CKeyStringPair* last,CKeyStringPair* out,const __false_type&) {
 for(;first!=last;++first,++out)if(out)new(out)CKeyStringPair(*first);
 return out;
}
__declspec(noinline) CKeyStringPair* __fastcall __uninitialized_fill_n(CKeyStringPair* out,unsigned int count,const CKeyStringPair& value,const __false_type&) {
 for(;count;--count,++out)if(out)new(out)CKeyStringPair(value);
 return out;
}
__declspec(noinline) void __fastcall _Destroy(CKeyStringPair* first,CKeyStringPair* last,const __false_type&) {
 for(;first!=last;++first)first->~CKeyStringPair();
}
#pragma auto_inline(on)
template<class T> __forceinline const T& max(const T&a,const T&b){return a<b?b:a;}
template<class T> void vector<T>::_M_insert_overflow(T* position,const T& value,const __false_type&,unsigned int count,bool atEnd) {
 const unsigned int oldSize=Size();const unsigned int length=oldSize+max(oldSize,count);
 T* newStart=length ? (T*)malloc(length*sizeof(T)):0;
 T* newFinish=__uninitialized_copy(start,position,newStart,__false_type());
 if(count==1){new(newFinish)T(value);++newFinish;}
 else newFinish=__uninitialized_fill_n(newFinish,count,value,__false_type());
 if(!atEnd)newFinish=__uninitialized_copy(position,finish,newFinish,__false_type());
 T* oldEnd=finish;for(T* i=start;i!=oldEnd;++i)i->second.~CCharString();if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void vector<CKeyStringPair>::_M_insert_overflow(CKeyStringPair*,const CKeyStringPair&,const __false_type&,unsigned int,bool);
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace FableVector_0074fce0 {
#pragma pack(push,1)
struct CStringData {unsigned char prefix[13];unsigned int refs;};
#pragma pack(pop)
static int destroyed=0;
CCharString::CCharString(const CCharString& o):data(o.data){if(data)++((CStringData*)data)->refs;}
CCharString::~CCharString(){if(data && --((CStringData*)data)->refs==0){++destroyed;free(data);}}
CCharString& CCharString::operator=(const CCharString& o){if(data!=o.data){if(data && --((CStringData*)data)->refs==0){++destroyed;free(data);}data=o.data;if(data)++((CStringData*)data)->refs;}return *this;}
template<class T> void vector<T>::Clear(){for(T* p=start;p!=finish;++p)p->~T();if(start)free(start);start=finish=end=0;}
}
int main(){using namespace FableVector_0074fce0;
vector<CKeyStringPair> v;memset(&v,0,sizeof v);__false_type tag;
{CKeyStringPair value;CStringData* data=(CStringData*)malloc(sizeof(CStringData));memset(data,0,sizeof *data);data->refs=1;value.second.data=data;value.first=12;
v._M_insert_overflow(v.start,value,tag,1,true);assert(v.finish-v.start==1 && v.end-v.start==1 && data->refs==2);
v._M_insert_overflow(v.start,value,tag,2,false);assert(v.finish-v.start==3 && v.end-v.start==3 && data->refs==4);
for(int i=0;i<3;++i)assert(v.start[i].second.data==data && v.start[i].first==12);
v._M_insert_overflow(v.finish,value,tag,0,true);assert(v.finish-v.start==3 && data->refs==4);
v._M_insert_overflow(v.start+1,value,tag,1,false);assert(v.finish-v.start==4 && data->refs==5);
v.Clear();assert(data->refs==1 && destroyed==0);}
assert(destroyed==1);puts("0074fce0_TEST PASS");return 0;}