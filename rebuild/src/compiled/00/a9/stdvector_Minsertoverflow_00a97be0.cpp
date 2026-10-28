// std::vector::_M_insert_overflow(pointer position, const value_type& value, const __false_type& tag, unsigned int count, bool atEnd) -- grow storage and insert counted elements.
// Leaf: ?_M_insert_overflow@?$vector@V?$CIVCountedPointer@VCTriangleList@N3DPrimitive@@@@V?$allocator@V?$CIVCountedPointer@VCTriangleList@N3DPrimitive@@@@@std@@@std@@IAEXPAV?$CIVCountedPointer@VCTriangleList@N3DPrimitive@@@@ABV3@ABU__false_type@2@I_N@Z
// Identity: proven; CDeviceIndependent::TriangleLists (ctor0xA98100) and AddTriangleList0xA90900; tailcopy0xA957C0/destroy0xA96990; four-byte intrusive-pointer element.
// Storage audit: original CIVCountedPointer<T>::Object storage is CIVCountedPointeeBase*, preserving original template/API identity. Prior selected snapshot/history unchanged.
// Complete-model audit: original field/base/signature revision; unused common declarations pruned. IVRefCount is unsigned long, Release()const returns long, definition pointers store CDefPointeeBase* and default through ENullDef. All helper uncertainty and omitted unused APIs are documented in COMPLETE_INTRUSIVE_MODEL_AUDIT.md. No original full layout is claimed for forward-only pointees.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define NOINLINE __attribute__((noinline))
typedef __SIZE_TYPE__ PlacementSize;
#else
#define NOINLINE __declspec(noinline)
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
class CIVCountedPointeeBase{public:CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;void AddRef()const{++IVRefCount;}long GetRefCount()const{return (long)IVRefCount;}long Release()const{long result=(long)--IVRefCount;if(result==0)Delete();return result;}};
template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase* Object;CIVCountedPointer():Object(0){}CIVCountedPointer(const CIVCountedPointer& other):Object(other.Object){if(Object)Object->AddRef();}__forceinline ~CIVCountedPointer(){if(Object){Object->Release();Object=0;}}CIVCountedPointer& operator=(const CIVCountedPointer& other){if(Object!=other.Object){if(Object){Object->Release();Object=0;}Object=other.Object;if(Object)Object->AddRef();}return *this;}T* operator->()const{return (T*)Object;}};
namespace std{struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class T>__forceinline void _Destroy(T* p){p->~T();}template<class Iter>NOINLINE void __fastcall __destroy_aux(Iter first,Iter last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}template<class Input,class Output,class Distance>Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}template<class T,class V>__forceinline void _Construct(T* p,const V& value){new(p)T(value);}template<class Input,class Output>inline Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){Output current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}}
namespace std{template<class T>class allocator{public:T* allocate(unsigned int,const void* =0);void deallocate(T*,unsigned int);};template<class V,class T,class Alloc>class _STLP_alloc_proxy:public Alloc{public:V _M_data;};template<class T,class Alloc>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage;};template<class T,class Alloc=allocator<T> >class vector:protected _Vector_base<T,Alloc>{public:typedef unsigned int size_type;typedef T* pointer;void reserve(size_type);protected:using _Vector_base<T,Alloc>::_M_start;using _Vector_base<T,Alloc>::_M_finish;using _Vector_base<T,Alloc>::_M_end_of_storage;template<class Input>T* _M_allocate_and_copy(size_type,Input,Input);void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool);};}
namespace N3DPrimitive{class CTriangleList;}


#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
#else
#include <stdlib.h>
#endif
namespace std {
template<class T> T* allocator<T>::allocate(unsigned int n,const void*) { return n?(T*)malloc(n*sizeof(T)):0; }
template<class T> void allocator<T>::deallocate(T* p,unsigned int) { if(p) free(p); }
template<class T> inline const T& max(const T& a,const T& b) { return a<b?b:a; }
template<class Iter,class Size,class T> __forceinline Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&) { for(;count>0;--count,++first) _Construct(&*first,value); return first; }
template<class T,class Alloc> void vector<T,Alloc>::_M_insert_overflow(pointer position,const T& value,const __false_type&,size_type fillLength,bool atEnd) {
 const size_type oldSize=size_type(_M_finish-_M_start);
 const size_type length=oldSize+max(oldSize,fillLength);
 pointer start=_M_end_of_storage.allocate(length);
 pointer finish=__uninitialized_copy(_M_start,position,start,__false_type());
 if(fillLength==1) { _Construct(finish,value); ++finish; }
 else finish=__uninitialized_fill_n(finish,fillLength,value,__false_type());
 if(!atEnd) finish=__uninitialized_copy(position,_M_finish,finish,__false_type());
 __destroy_aux(_M_start,_M_finish,__false_type());
 _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);
 _M_start=start; _M_finish=finish; _M_end_of_storage._M_data=start+length;
}
}

typedef CIVCountedPointer<N3DPrimitive::CTriangleList> Element;
template void __fastcall std::__destroy_aux<Element*>(Element*,Element*,const std::__false_type&);
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);
template void std::vector<Element>::_M_insert_overflow(Element*,const Element&,const std::__false_type&,unsigned int,bool);

#ifdef _MSC_VER
typedef char AuditSize_0[(sizeof(CIVCountedPointeeBase)==8)?1:-1];
#endif