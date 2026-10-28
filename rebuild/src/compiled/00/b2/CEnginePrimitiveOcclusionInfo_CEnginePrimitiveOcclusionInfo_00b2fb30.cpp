// CEnginePrimitiveOcclusionInfo::~CEnginePrimitiveOcclusionInfo() -- Release owned sequence storage and destroy the base.
// Leaf: ??1CEnginePrimitiveOcclusionInfo@@UAE@XZ
// Identity: Own deleting-dtor00b2fb10 and full RTTI12a1284; CIVCountedPointeeBase at0, InvisibleFrames+8, handles+12, base vtable12354d0.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std {
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A>class _Vector_base {public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base(){_M_start=0;_M_finish=0;_M_end_of_storage._M_data=0;}__forceinline ~_Vector_base(){if(_M_start)free(_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:__forceinline ~vector(){for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();}};
}
template<class T>class CArray {public:std::vector<T> Data;};
// Layout: shared base headers describe flattened vptrs; genuine polymorphic hierarchy is required for compiler-generated destruction. Original base fields and offsets retained; no target base reconstruction claim.
class CIVCountedPointeeBase {public:virtual __forceinline ~CIVCountedPointeeBase(){}virtual void Delete()const;unsigned long IVRefCount;};
class CEnginePrimitiveOcclusionInfo:public CIVCountedPointeeBase {public:long InvisibleFrames;CArray<unsigned long> OcclusionTestHandles;bool Seen;unsigned long TestFrame;};


void DestroyCandidate(CEnginePrimitiveOcclusionInfo* p){p->CEnginePrimitiveOcclusionInfo::~CEnginePrimitiveOcclusionInfo();}