// std::vector::_Insert_n(iterator,unsigned int,const value_type&) -- Original Dinkumware pointer insertion, growth and alias-safe temporary.
// Leaf: ?_Insert_n@?$vector@PAVCUnprovenQueueRequestOne@@V?$allocator@PAVCUnprovenQueueRequestOne@@@std@@@std@@QAEXPAPAVCUnprovenQueueRequestOne@@IABQAV3@@Z
// Identity: descriptive. Own worker-queue destructor00c14560 inserts null quit request into member24, whose actual allocator/first/last/end offsets0/4/8/12 prove Dinkumware vector16. Own _Xlen00c13840,_Ucopy00c13ec0,_Ufill00c13fc0,fill00c13a80,copy_backward00c13d40. Original library pointer pointee type not recovered, distinct descriptive incomplete classes prevent conflating helper addresses. Actual scalar new00bfea1a/scalar delete00bfe9bc, imported memmove01440144; 1.5 capacity growth and max elements0x3fffffff.
// Layout: genuine vector with allocator member then three pointers; helpers use actual copy/fill semantics.
#include <new>
#ifdef _MSC_VER
extern "C" __declspec(dllimport) void* __cdecl memmove(void*,const void*,size_t);
#else
#include <string.h>
#endif
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define NOINLINE __attribute__((noinline))
#define NORETURN __attribute__((noreturn))
#else
#define NOINLINE __declspec(noinline)
#define NORETURN __declspec(noreturn)
#endif
namespace std{
template<class T>class allocator{public:T* allocate(unsigned int n){return (T*)operator new(n*sizeof(T));}void deallocate(T* p,unsigned int){operator delete(p);}unsigned int max_size()const{return 0xffffffffU/(unsigned int)sizeof(T);}};
template<class T>__forceinline T* copy(T* first,T* last,T* result){unsigned int n=(unsigned int)(last-first);return (T*)memmove(result,first,n*sizeof(T))+n;}
template<class T>NOINLINE void __fastcall fill(T* first,T* last,const T& value){for(;first!=last;++first)*first=value;}
template<class T>NOINLINE T* __fastcall copy_backward(T* first,T* last,T* result){unsigned int n=(unsigned int)(last-first);T* out=result-n;memmove(out,first,n*sizeof(T));return out;}
template<class T,class A=allocator<T> >class vector{public:typedef T* iterator;typedef unsigned int size_type;A _Alval;T* _Myfirst;T* _Mylast;T* _Myend;
size_type size()const{return _Myfirst==0?0:(size_type)(_Mylast-_Myfirst);}size_type capacity()const{return _Myfirst==0?0:(size_type)(_Myend-_Myfirst);}size_type max_size()const{return _Alval.max_size();}
NORETURN void _Xlen();
NOINLINE T* _Ucopy(T* first,T* last,T* result){return copy(first,last,result);}
NOINLINE T* _Ufill(T* first,size_type n,const T& value){T* out=first;for(size_type i=n;i>0;--i,++out)*out=value;return first+n;}
void _Insert_n(iterator where,size_type count,const T& value);
};
template<class T,class A>void vector<T,A>::_Insert_n(iterator where,size_type count,const T& value){T copyValue=value;size_type newCapacity=capacity();if(count==0)return;
if(max_size()-size()<count)_Xlen();
if(newCapacity<size()+count){newCapacity=max_size()-newCapacity/2<newCapacity?0:newCapacity+newCapacity/2;if(newCapacity<size()+count)newCapacity=size()+count;
T* fresh=_Alval.allocate(newCapacity);T* out=copy(_Myfirst,where,fresh);out=_Ufill(out,count,copyValue);copy(where,_Mylast,out);count+=size();if(_Myfirst)_Alval.deallocate(_Myfirst,_Myend-_Myfirst);_Myend=fresh+newCapacity;_Mylast=fresh+count;_Myfirst=fresh;
}else if((size_type)(_Mylast-where)<count){T* oldLast=_Mylast;_Ucopy(where,oldLast,where+count);_Ufill(_Mylast,count-(_Mylast-where),copyValue);_Mylast+=count;fill(where,_Mylast-count,copyValue);
}else{T* oldLast=_Mylast;_Mylast=_Ucopy(oldLast-count,oldLast,oldLast);copy_backward(where,oldLast-count,oldLast);fill(where,where+count,copyValue);}}
}
class CUnprovenQueueRequestOne;typedef std::vector<CUnprovenQueueRequestOne*> Target;template void Target::_Insert_n(CUnprovenQueueRequestOne**,unsigned int,CUnprovenQueueRequestOne* const&);