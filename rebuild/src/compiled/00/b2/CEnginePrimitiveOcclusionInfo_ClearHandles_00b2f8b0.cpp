// CEnginePrimitiveOcclusionInfo::ClearHandles() -- clear the trivial handle vector.
// Leaf: ?ClearHandles@CEnginePrimitiveOcclusionInfo@@AAEXXZ
// Identity: proven; own CheckVisibilityResult0xB2F8E0 calls at0xB2F9D4 on its info argument; PDB private ClearHandles and OcclusionTestHandles+12.
// Layout: no target generated header. Real RTTI base chain retains the secondary vptr at+12 for appearance callbacks; donor16-byte vector becomes retail12-byte vector. The compiler supplies the secondary receiver convention.
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" void* __cdecl memmove(void*,const void*,size_t);
namespace std {
inline void* __copy_trivial(const void* first,const void* last,void* result) { return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first); }
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc> class _Vector_base { protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; };
template<class T,class Alloc=allocator<T> > class vector:protected _Vector_base<T,Alloc> { protected:using _Vector_base<T,Alloc>::_M_start;using _Vector_base<T,Alloc>::_M_finish;using _Vector_base<T,Alloc>::_M_end_of_storage;public: T* erase(T* first,T* last) { T* result=(T*)__copy_trivial(last,_M_finish,first); _M_finish=result; return first; } void clear() { erase(_M_start,_M_finish); } };
}
template<class T> class CArray:public std::vector<T> {};
class CIVCountedPointeeBase { public: virtual ~CIVCountedPointeeBase() {} mutable unsigned long IVRefCount; }; class CEnginePrimitiveOcclusionInfo:public CIVCountedPointeeBase { public: long InvisibleFrames; CArray<unsigned long> OcclusionTestHandles; private: void ClearHandles(); };
void CEnginePrimitiveOcclusionInfo::ClearHandles() { OcclusionTestHandles.clear(); }