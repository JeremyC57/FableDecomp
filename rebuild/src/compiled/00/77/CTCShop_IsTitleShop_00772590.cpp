// CTCShop::IsTitleShop() const -- query the original shop subdefinition and return its category flag.
// Leaf: ?IsTitleShop@CTCShop@@QBE_NXZ
// Identity: proven; Original const CTCShop query PDB and own field/caller. Full actual CDefClassBase/CSubDef/CParentDef/CThingBaseDef/ShopDef/Resource/CIV hierarchy, private Def default constructor with original friend classes, real CDefPointer storage/SetDynamicCast/lifetimes. Own005130a0 real GetSubDef body: StatGetTypeName CShopDef literal, const GetPSubDefInfo virtualslot14, original nested CSubDefInfo.DefIndex/OriginalParentDefIndex, mutable protected CDefinitionManager::GetPDefFromGlobalIndex009ada10 distinct from const009ad9e0. Source-visible genuine body exposes out-pointer lifetime; no fake callee. Full typed ShopDef prefix through own bools and correct genuine protected STLport vector/list/map storage; inactive ShopDef/manager suffix omitted without totalsizeclaim. CThing real CClass/CBaseObject/CBaseClass ancestry and correct DefGlobalIndex140, unused Thing prefix explicit opaque. Shared original headers aliased unchanged.
// Layout: full original base chains and known typed data, context-only unused prefixes/suffixes explicit.
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderEvidence
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResource_HeaderEvidence
#include "engine/CResource.h"
#undef CResource
#define CDefClassBase CDefClassBase_HeaderEvidence
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#define CClass CClass_HeaderEvidence
#include "engine/CClass.h"
#undef CClass
#define CBaseObject CBaseObject_HeaderEvidence
#include "engine/CBaseObject.h"
#undef CBaseObject
#define CTCBase CTCBase_HeaderEvidence
#include "engine/CTCBase.h"
#undef CTCBase
#define CDefinitionManager CDefinitionManager_HeaderEvidence
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
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
template<class T>class CDefPointer{public:CDefPointeeBase* Object;CDefPointer(ENullDef=ENullDef_DescriptiveNull):Object(0){}CDefPointer(const CDefPointer& other):Object(other.Object){if(Object)Object->AddRef();}CDefPointer(T* object):Object((CDefPointeeBase*)object){if(Object)Object->AddRef();}__forceinline ~CDefPointer(){if(Object){Object->Release();Object=0;}}CDefPointer& operator=(const CDefPointer& other){if(Object!=other.Object){if(Object){Object->Release();Object=0;}Object=other.Object;if(Object)Object->AddRef();}return *this;}template<class U>__forceinline void SetDynamicCast(const CDefPointer<U>&other){CDefPointeeBase*object=other.Object;if(Object!=object){if(Object){Object->Release();Object=0;}Object=object;if(Object)Object->AddRef();}}T* operator->()const{return (T*)Object;}};
namespace std{
template<class T>class allocator{};template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};
template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{};
template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}};
template<class Iter,class Compare>void __fastcall sort(Iter,Iter,Compare);
}
namespace std{template<class T>struct less{};template<class T>struct _Select1st{};struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};template<class T,class A>class _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};template<class K,class V,class Sel,class C,class A>class _Rb_tree:protected _Rb_tree_base<V,A>{public:unsigned int _M_node_count;C _M_key_compare;};template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{private:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,C,A> _M_t;};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;C Compare;bool Dirty;};
class CPersistContext;class CDefinitionManager;class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();const CCharString& operator=(const CCharString&);};class CDefString{public:long TablePos;};
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase{public:class CSubDefInfo{public:long DefIndex,OriginalParentDefIndex;};CDefinitionManager* PDefManager;unsigned long GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;void SetDefaultValsApplied(bool value){DefaultValsApplied=value;}private:CDefClassBase();friend class CSubDefClassBase;friend class CParentDefClassBase;public:CDefClassBase(const CDefClassBase&);CDefClassBase& operator=(const CDefClassBase&);virtual void UnknownDefinitionSlot2(){}virtual void UnknownDefinitionSlot3(){}virtual void UnknownDefinitionSlot4(){}virtual void UnknownDefinitionSlot5(){}virtual void UnknownDefinitionSlot6(){}virtual void UnknownDefinitionSlot7(){}virtual void UnknownDefinitionSlot8(){}virtual void UnknownDefinitionSlot9(){}virtual void UnknownDefinitionSlot10(){}virtual void UnknownDefinitionSlot11(){}virtual void UnknownDefinitionSlot12(){}virtual void UnknownDefinitionSlot13(){}virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&)const;virtual void UnknownDefinitionSlot15(){}virtual void UnknownDefinitionSlot16(){}virtual void UnknownDefinitionSlot17(){} virtual void Transfer(CPersistContext&);template<class T>NOINLINE bool GetSubDef(CDefPointer<const T>*)const;};
#pragma pack(pop)
#pragma pack(push,1)
class CSubDefClassBase:public CDefClassBase{};class CParentDefClassBase:public CDefClassBase{public:CDefString InstantiationName;CVectorMap<unsigned long,CDefClassBase::CSubDefInfo> SubDefInfoMap;virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&)const;};class CThingBaseDef:public CParentDefClassBase{};
#pragma pack(pop)
enum EOpinionAttitudeType{EOpinionAttitudeType_DescriptiveZero=0};class CShopItemDef;class CShopDef:public CSubDefClassBase{public:unsigned long Name,InitialHawkingPhrase,ReplyToRequestPhrase,CustomerFollowPhrase,ThankingPhrase;CCharString ShopkeeperMeshType;std::vector<CShopItemDef> DefaultStock,StockCategoryDefaults;float BuyPriceMultiplier,SellPriceMultiplier,AttitudeBuyPriceMultiplierDefault;std::map<EOpinionAttitudeType,float> AttitudeBuyPriceMultiplier;float AttitudeSellPriceMultiplierDefault;std::map<EOpinionAttitudeType,float> AttitudeSellPriceMultiplier;float MaxStockChangePerSell,MaxStockChangePerBuy,MaxStockRevertPerDay,PriceReductionFractionPerItem,MaxPriceReductionFractionDueToStockCount;unsigned long PreferredShopkeeper;bool IsStockDisplayPermanent,IsBarberShop,IsTattooShop,IsTitleShop,AlwaysOpen;static CCharString __fastcall StatGetTypeName(){return CCharString("CShopDef");}};
class CFailedAllocationHandler{public:virtual ~CFailedAllocationHandler();};class CResourceList:public CFailedAllocationHandler{public:CResource Head;unsigned long ResourceCount;long AllocatedMemory,MaximumMemory;unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;};class CDefinitionManager:public CResourceList{public:CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long)const;template<class T>NOINLINE bool GetDef(long,CDefPointer<const T>*)const;protected:friend class CDefClassBase;CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long);};class CGameDefinitionManager:public CDefinitionManager{};
template<class T>NOINLINE bool CDefinitionManager::GetDef(long index,CDefPointer<const T>*out)const{if(index>0){CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);if(def.Object){out->SetDynamicCast(def);return true;}}return false;}
template<class T>NOINLINE bool CDefClassBase::GetSubDef(CDefPointer<const T>*out)const{const CSubDefInfo*info=GetPSubDefInfo(T::StatGetTypeName());if(!info)return false;CDefPointer<const T> def;def.SetDynamicCast(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=def;return true;}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable&operator=(const CBaseClassNonCopyable&);};
struct CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;protected:CBaseObject();virtual ~CBaseObject();};namespace std{struct _List_node_base;struct _List_iterator_base{_List_node_base* _M_node;};template<class T>struct _Nonconst_traits{};template<class T,class Traits>struct _List_iterator:public _List_iterator_base{};}
class CClass:public CBaseObject{public:std::_List_iterator<CClass*,std::_Nonconst_traits<CClass*> > ClassListPos,TypeListPos;unsigned char Type;};class CThing:public CClass{public:unsigned char _unmodeled14[0x8C-0x14];unsigned short DefGlobalIndex;unsigned char _unmodeled8E[0x94-0x8E];};
class CTCBase:public CBaseClassNonCopyable{public:CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;protected:const CGameDefinitionManager& GetDefinitionManager()const;};
class CTCShop:public CTCBase{public:bool IsTitleShop()const;};bool CTCShop::IsTitleShop()const{CDefPointer<const CThingBaseDef>thingDef;GetDefinitionManager().GetDef(ParentThing->DefGlobalIndex,&thingDef);CDefPointer<const CShopDef>shopDef;return thingDef->GetSubDef(&shopDef)&&shopDef->IsTitleShop;}
#ifdef _MSC_VER
typedef char DefBaseSize[sizeof(CDefClassBase)==37?1:-1];typedef char ParentDefSize[sizeof(CParentDefClassBase)==57?1:-1];typedef char ResourceListSize[sizeof(CResourceList)==60?1:-1];typedef char ThingClassSize[sizeof(CClass)==20?1:-1];typedef char ThingSize[sizeof(CThing)==148?1:-1];typedef char ShopFlagOffset[offsetof(CShopDef,IsTitleShop)==155?1:-1];
#endif