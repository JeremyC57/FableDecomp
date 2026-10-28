// CCountedPointer::DeleteData(void* object) -- Delete the typed pointee and its owned plain-element array.
// Leaf: ?DeleteData@?$CCountedPointer@VCSplinePosParamData@CPSCSplineBase@NParticleEngine@@@@SIXPAX@Z
// Identity: Own CPSCSplineBase::DatabaseCopy00b22530 constructor00b21a00 and Reset00b23170 bind this callback. Original nested CSplinePosParamData and SplinePosParam12, retail24. Outer CPSCSplineBase is naming scope only.
// Layout: genuine original CArray/std::vector/base/allocator-proxy. All owned array elements are trivial. Original fields retained, no active pointee header.
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:~vector(){if(this->_M_start)free(this->_M_start);}};}
template<class T>class CArray:public std::vector<T>{};
template<class T>class CCountedPointer{public:static void __fastcall DeleteData(void* object){delete static_cast<T*>(object);}};
class CCardinalSpline;namespace NParticleEngine{class CParticleSystem;class CPSCSplineBase{public:class CSplinePosParamData{public:CParticleSystem* PParentParticleSystem;CCardinalSpline* Spline;bool MaintainSplineShape;CArray<unsigned long> SplinePosParam;};};}
template void __fastcall CCountedPointer<NParticleEngine::CPSCSplineBase::CSplinePosParamData>::DeleteData(void*);