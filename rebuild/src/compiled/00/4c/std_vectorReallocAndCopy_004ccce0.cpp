// 004ccce0 _STL::vector<std::pair<unsigned_int,unsigned_int>>::_M_insert_overflow
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@II@FableVector_004ccce0@@@FableVector_004ccce0@@QAEXPAU?$pair@II@2@ABU32@ABU__false_type@2@I_N@Z
// Layout: oracle stride8; two copied dwords, trivial destructor; vector3 pointers. Concrete pair key/value aliases unknown.
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int,void* p){return p;}
namespace FableVector_004ccce0 {
struct __false_type {__false_type(){}};
template<class K,class V> struct pair {K first;V second;pair(){} __forceinline pair(const pair& o):first(o.first),second(o.second){}};
typedef pair<unsigned int,unsigned int> CKeyValuePair;
// Layout: retail STL vector object uses three contiguous pointer words at+0,+4,+8.
template<class T> struct vector {
 T* start;T* finish;T* end;
 unsigned int Size()const{return finish-start;}
 void Clear();
 void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);
};
#pragma auto_inline(on)
__forceinline CKeyValuePair* __fastcall __uninitialized_copy(const CKeyValuePair* first,const CKeyValuePair* last,CKeyValuePair* out,const __false_type&) {
 for(;first!=last;++first,++out)if(out)new(out)CKeyValuePair(*first);
 return out;
}
__forceinline CKeyValuePair* __fastcall __uninitialized_fill_n(CKeyValuePair* out,unsigned int count,const CKeyValuePair& value,const __false_type&) {
 for(;count>0;--count,++out)if(out)new(out)CKeyValuePair(value);
 return out;
}
__forceinline void __fastcall _Destroy(CKeyValuePair* first,CKeyValuePair* last,const __false_type&) {
 for(;first!=last;++first)first->~CKeyValuePair();
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
 T* oldEnd=finish;for(T* i=start;i!=oldEnd;++i)i->~T();if(start)free(start);
 start=newStart;finish=newFinish;end=newStart+length;
}
template void vector<CKeyValuePair>::_M_insert_overflow(CKeyValuePair*,const CKeyValuePair&,const __false_type&,unsigned int,bool);
}