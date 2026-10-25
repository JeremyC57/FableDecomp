// 00569450 std::vector<std::pair<int,CWideString>>::_M_insert_overflow
// Flags: /Gr
// Leaf: _M_insert_overflow
// Layout: vector3 pointers; pair stride8 and CWideString at+4 from ctor99b720 indexed access.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
namespace FableVector_00569450 {
struct __false_type {__false_type(){}};
// Layout: CWideString header absent; retail ctor0099ec30, dtor0099eae0, single pointer.
struct CWideString {void* data;CWideString():data(0){} CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
template<class K,class V> struct pair {K first;V second;pair(){} __forceinline pair(const pair& o):first(o.first),second(o.second){}};
typedef pair<int,CWideString> CKeyStringPair;
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
 T* oldEnd=finish;for(T* i=start;i!=oldEnd;++i)i->second.~CWideString();if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void vector<CKeyStringPair>::_M_insert_overflow(CKeyStringPair*,const CKeyStringPair&,const __false_type&,unsigned int,bool);
}