// std::__copy(Input first, Input last, Output result, const random_access_iterator_tag& tag, int* distance) -- assign counted pointers forwards.
// Leaf: ??$__copy@PAV?$CDefPointer@$$CBVCThingBaseDef@@@@PAV1@H@std@@YIPAV?$CDefPointer@$$CBVCThingBaseDef@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Identity: proven; CTCHeroMorph::Tattoos own caller GetTattooThings0x71DE20 uses exact mutable CDefPointer<const CThingBaseDef>; existing real binding; four-byte element and pointee count+4.
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
namespace std{struct __false_type{__false_type(){}};struct input_iterator_tag{};struct forward_iterator_tag:input_iterator_tag{};struct bidirectional_iterator_tag:forward_iterator_tag{};struct random_access_iterator_tag:bidirectional_iterator_tag{};template<class T>__forceinline void _Destroy(T* p){p->~T();}template<class Iter>void __fastcall __destroy_aux(Iter first,Iter last,const __false_type&){for(;first!=last;++first)_Destroy(&*first);}template<class Input,class Output,class Distance>Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance n=last-first;n>0;--n){*result=*first;++first;++result;}return result;}template<class T,class V>__forceinline void _Construct(T* p,const V& value){new(p)T(value);}template<class Input,class Output>inline Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&){Output current=result;for(;first!=last;++first,++current)_Construct(&*current,*first);return current;}}
class CThingBaseDef;


typedef CDefPointer<const CThingBaseDef> Element;
template Element* __fastcall std::__copy<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);

#ifdef _MSC_VER
typedef char AuditSize_0[(sizeof(CIVCountedPointeeBase)==8)?1:-1];
#endif