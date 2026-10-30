// CTCBed::OnCreate() -- fetch the parent definition and its component sub-definition.
// Leaf: ?OnCreate@CTCBed@@UAEXXZ
// Identity: proven; RTTI own vtable slot4; PDB virtual OnCreate(); PDef+20 and GetSubDef<CBedDef>0x00746c20; CTCBase protected const GetDefinitionManager0x6869D0 then CDefinitionManager::GetDef<CThingBaseDef>0x42AF3C.
// Storage audit: original CIVCountedPointer<T>::Object storage is CIVCountedPointeeBase*, preserving original template/API identity. Prior selected snapshot/history unchanged.
// Layout: no target class header. Generated CTCBase/CThing headers flatten polymorphism (CThing is quarantined); local real base chain and minimal named DefGlobalIndex view preserve ParentThing+4 and retail index+0x8C. OverheadDisplay lists are retail4-byte nodes.
// Complete-model audit: original field/base/signature revision; unused common declarations pruned. IVRefCount is unsigned long, Release()const returns long, definition pointers store CDefPointeeBase* and default through ENullDef. All helper uncertainty and omitted unused APIs are documented in COMPLETE_INTRUSIVE_MODEL_AUDIT.md. No original full layout is claimed for forward-only pointees.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResource_HeaderEvidence
#include "engine/CResource.h"
#undef CResource
#define CDefClassBase CDefClassBase_HeaderEvidence
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#define CParentDefClassBase CParentDefClassBase_HeaderEvidence
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#define CResourceList CResourceList_HeaderEvidence
#include "engine/CResourceList.h"
#undef CResourceList
#define CDefinitionManager CDefinitionManager_HeaderEvidence
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CTCBase CTCBase_HeaderEvidence
#include "engine/CTCBase.h"
#undef CTCBase
#define CBaseObject CBaseObject_HeaderEvidence
#include "engine/CBaseObject.h"
#undef CBaseObject
#define CClass CClass_HeaderEvidence
#include "engine/CClass.h"
#undef CClass
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
namespace std{template<class T>class allocator{public:T* allocate(unsigned int,const void* =0);void deallocate(T*,unsigned int);};template<class V,class T,class Alloc>class _STLP_alloc_proxy:public Alloc{public:V _M_data;};template<class T,class Alloc>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage;};template<class T,class Alloc=allocator<T> >class vector:protected _Vector_base<T,Alloc>{public:typedef unsigned int size_type;typedef T* pointer;void reserve(size_type);protected:using _Vector_base<T,Alloc>::_M_start;using _Vector_base<T,Alloc>::_M_finish;using _Vector_base<T,Alloc>::_M_end_of_storage;template<class Input>T* _M_allocate_and_copy(size_type,Input,Input);void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool);};}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass{};
class CThing;class CGameDefinitionManager;class CTCBase:public CBaseClassNonCopyable{public:CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;CTCBase(CThing&);virtual ~CTCBase();virtual void UnknownCTCSlot1(){}virtual void UnknownCTCSlot2(){}virtual void UnknownCTCSlot3(){}virtual void OnCreate();protected:const CGameDefinitionManager& GetDefinitionManager()const;};
struct CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;protected:CBaseObject();virtual ~CBaseObject();};namespace std{struct _List_node_base;struct _List_iterator_base{_List_node_base* _M_node;};template<class T>struct _Nonconst_traits{};template<class T,class Traits>struct _List_iterator:public _List_iterator_base{};}
class CClass:public CBaseObject{public:std::_List_iterator<CClass*,std::_Nonconst_traits<CClass*> > ClassListPos,TypeListPos;unsigned char Type;};class CThing:public CClass{public:unsigned char _unmodeled14[0x8C-0x14];unsigned short DefGlobalIndex;unsigned char _unmodeled8E[0x94-0x8E];};
namespace std{template<class K,class V>struct pair{K first;V second;};}template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;CompareType Compare;bool Dirty;};class CDefString{public:long TablePos;};class CDefinitionManager;
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase{public:class CSubDefInfo;CDefinitionManager* PDefManager;unsigned long GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;template<class T>bool GetSubDef(CDefPointer<const T>*)const;};class CParentDefClassBase:public CDefClassBase{public:CDefString InstantiationName;CVectorMap<unsigned long,CDefClassBase::CSubDefInfo> SubDefInfoMap;};class CThingBaseDef:public CParentDefClassBase{};
#pragma pack(pop)
class CFailedAllocationHandler{public:virtual ~CFailedAllocationHandler(){}};class CResourceList:public CFailedAllocationHandler{public:CResource Head;unsigned long ResourceCount;long AllocatedMemory,MaximumMemory;unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;};class CDefinitionManager:public CResourceList{public:CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long)const;template<class T>NOINLINE bool GetDef(long,CDefPointer<const T>*)const;};class CGameDefinitionManager:public CDefinitionManager{};
template<class T>NOINLINE bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out)const{if(index>0){CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);if(def.Object){CDefPointeeBase* object=def.Object;if(out->Object!=object){if(out->Object){out->Object->Release();out->Object=0;}out->Object=object;if(object)object->AddRef();}return true;}}return false;}
class CBedDef;class CTCBed:public CTCBase{public:bool Empty,GettingIn,GettingOut,Animating;long BedSleepAnim;CDefPointer<const CBedDef> PDef;CTCBed(CThing&);virtual void OnCreate();};void CTCBed::OnCreate(){CDefPointer<const CThingBaseDef> thingDef;if(GetDefinitionManager().GetDef(ParentThing->DefGlobalIndex,&thingDef))thingDef->GetSubDef(&PDef);}

#ifdef _MSC_VER
typedef char AuditSize_0[(sizeof(CIVCountedPointeeBase)==8)?1:-1];
typedef char AuditSize_1[(sizeof(CResource)==28)?1:-1];
typedef char AuditSize_2[(sizeof(CDefPointeeBase)==28)?1:-1];
typedef char AuditSize_3[(sizeof(CDefClassBase)==37)?1:-1];
typedef char AuditSize_4[(sizeof(CParentDefClassBase)==57)?1:-1];
typedef char AuditSize_5[(sizeof(CResourceList)==60)?1:-1];
typedef char AuditSize_6[(sizeof(CClass)==20)?1:-1];
typedef char AuditSize_7[(sizeof(CThing)==148)?1:-1];
typedef char AuditSize_8[(sizeof(CTCBase)==12)?1:-1];
#endif