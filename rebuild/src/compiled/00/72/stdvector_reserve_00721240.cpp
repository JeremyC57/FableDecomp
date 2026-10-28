// std::vector::reserve(unsigned int n) -- grow capacity while preserving the existing counted elements.
// Leaf: ?reserve@?$vector@V?$CDefPointer@$$CBVCThingBaseDef@@@@V?$allocator@V?$CDefPointer@$$CBVCThingBaseDef@@@@@std@@@std@@QAEXI@Z
// Identity: proven; CTCHeroMorph::GetTattooThings0x71DE20, own Tattoos vector, allocation0x71EA40/destroy0x5B26E9; four-byte elements and STLport12-byte vector.
// Complete-model audit: original field/base/signature revision; unused common declarations pruned. IVRefCount is unsigned long, Release()const returns long, definition pointers store CDefPointeeBase* and default through ENullDef. All helper uncertainty and omitted unused APIs are documented in COMPLETE_INTRUSIVE_MODEL_AUDIT.md. No original full layout is claimed for forward-only pointees.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResource_HeaderEvidence
#include "engine/CResource.h"
#undef CResource
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
class CResourceList;class CResource:public CIVCountedPointeeBase{public:CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;unsigned long ResourceSize,LastUsedFrame;CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}virtual ~CResource();};class CDefPointeeBase:public CResource{public:virtual void Delete()const;};enum ENullDef{ENullDef_DescriptiveNull=0};
template<class T>class CDefPointer{public:CDefPointeeBase* Object;CDefPointer(ENullDef=ENullDef_DescriptiveNull):Object(0){}CDefPointer(const CDefPointer& other):Object(other.Object){if(Object)Object->AddRef();}CDefPointer(T* object):Object((CDefPointeeBase*)object){if(Object)Object->AddRef();}__forceinline ~CDefPointer(){if(Object){Object->Release();Object=0;}}CDefPointer& operator=(const CDefPointer& other){if(Object!=other.Object){if(Object){Object->Release();Object=0;}Object=other.Object;if(Object)Object->AddRef();}return *this;}T* operator->()const{return (T*)Object;}};
namespace std{struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class T>__forceinline void _Destroy(T* p){p->~T();}template<class Iter>NOINLINE void __fastcall __destroy_aux(Iter first,Iter last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}template<class Input,class Output,class Distance>Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}template<class T,class V>__forceinline void _Construct(T* p,const V& value){new(p)T(value);}template<class Input,class Output>inline Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){Output current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}}
namespace std{template<class T>class allocator{public:T* allocate(unsigned int,const void* =0);void deallocate(T*,unsigned int);};template<class V,class T,class Alloc>class _STLP_alloc_proxy:public Alloc{public:V _M_data;};template<class T,class Alloc>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage;};template<class T,class Alloc=allocator<T> >class vector:protected _Vector_base<T,Alloc>{public:typedef unsigned int size_type;typedef T* pointer;void reserve(size_type);protected:using _Vector_base<T,Alloc>::_M_start;using _Vector_base<T,Alloc>::_M_finish;using _Vector_base<T,Alloc>::_M_end_of_storage;template<class Input>T* _M_allocate_and_copy(size_type,Input,Input);void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool);};}
class CThingBaseDef;


#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
#else
#include <stdlib.h>
#endif
namespace std {
template<class T> T* allocator<T>::allocate(unsigned int n,const void*) { return n?(T*)malloc(n*sizeof(T)):0; }
template<class T> void allocator<T>::deallocate(T* p,unsigned int) { if(p) free(p); }
template<class T,class Alloc> void vector<T,Alloc>::reserve(size_type n) { if(size_type(_M_end_of_storage._M_data-_M_start)<n) { const size_type oldSize=size_type(_M_finish-_M_start); T* result; if(_M_start) { result=_M_allocate_and_copy(n,_M_start,_M_finish); __destroy_aux(_M_start,_M_finish,__false_type()); _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start); } else result=_M_end_of_storage.allocate(n); _M_start=result; _M_finish=result+oldSize; _M_end_of_storage._M_data=result+n; } }
}

typedef CDefPointer<const CThingBaseDef> Element;
template void __fastcall std::__destroy_aux<Element*>(Element*,Element*,const std::__false_type&);
template void std::vector<Element>::reserve(unsigned int);

#ifdef _MSC_VER
typedef char AuditSize_0[(sizeof(CIVCountedPointeeBase)==8)?1:-1];
#endif