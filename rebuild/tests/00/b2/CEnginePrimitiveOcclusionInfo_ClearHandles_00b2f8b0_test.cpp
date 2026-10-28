#include <stdio.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" void* __cdecl memmove(void*,const void*,size_t);
namespace std {
inline void* __copy_trivial(const void* first,const void* last,void* result) { return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first); }
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc> class _Vector_base { public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; };
template<class T,class Alloc=allocator<T> > class vector:protected _Vector_base<T,Alloc> { public:using _Vector_base<T,Alloc>::_M_start;using _Vector_base<T,Alloc>::_M_finish;using _Vector_base<T,Alloc>::_M_end_of_storage;public: T* erase(T* first,T* last) { T* result=(T*)__copy_trivial(last,_M_finish,first); _M_finish=result; return first; } void clear() { erase(_M_start,_M_finish); } };
}
template<class T> class CArray:public std::vector<T> {};
class CIVCountedPointeeBase { public: virtual ~CIVCountedPointeeBase() {} mutable unsigned long IVRefCount; }; class CEnginePrimitiveOcclusionInfo:public CIVCountedPointeeBase { public: long InvisibleFrames; CArray<unsigned long> OcclusionTestHandles; public: void ClearHandles(); };
void CEnginePrimitiveOcclusionInfo::ClearHandles() { OcclusionTestHandles.clear(); }
int main() { CEnginePrimitiveOcclusionInfo object; unsigned long data[3]; object.OcclusionTestHandles._M_start=data; object.OcclusionTestHandles._M_finish=data+3; object.OcclusionTestHandles._M_end_of_storage._M_data=data+3; object.ClearHandles(); if(object.OcclusionTestHandles._M_finish!=data || object.OcclusionTestHandles._M_start!=data || object.OcclusionTestHandles._M_end_of_storage._M_data!=data+3) goto fail; object.ClearHandles(); if(object.OcclusionTestHandles._M_finish!=data) goto fail; puts("00b2f8b0_TEST PASS"); return 0; fail: puts("00b2f8b0_TEST FAIL"); return 1; }