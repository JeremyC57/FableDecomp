// NUISystem::CManager::SetKeyPressed() -- Store the requested input state.
// Leaf: ?SetKeyPressed@CManager@NUISystem@@UAEXG@Z
// Flags: default (no /Zc:wchar_t: retail's wchar_t is the <stddef.h> unsigned short typedef, owner 2026-10-29)
// Corrected interface: actual CObservable has eleven original virtuals and a nonvirtual destructor; CManager adds its virtual destructor at slot11 before SetKeyPressed12. Manager vtable01230134/base01230044 and original PDB signatures prove this.
// Identity: Own CManager vtable01230134 slot12 and original PDB SetKeyPressed(wchar_t), member KeyPressed+192. RTTI proves CObservable direct base, original lists4 and maps12 explain donor drift.
// Layout: original class keys, actual bases and typed STLport containers/counting handles. CManager header renamed to restore actual inheritance/types; enum value spellings unspecified. Bank owner declaration is a prefix through UsingRetailBanks only. Unused virtual slots explicitly descriptive.
#define CManager CManager_HeaderLayout
#include "engine/CManager.h"
#undef CManager
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable(){}~CBaseClassNonCopyable(){}private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);};
class CCharStringData;class CCharString{public:CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
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
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;__forceinline void Sort(){if(Dirty){std::sort(Data.begin(),Data.end(),Compare);Dirty=false;}}};

namespace std{
template<class T>struct less{};template<class T>struct _Select1st{};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class T,class A>class _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};
template<class K,class V,class S,class C,class A>class _Rb_tree:protected _Rb_tree_base<V,A>{public:unsigned int _M_node_count;C _M_key_compare;};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{public:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,C,A> _M_t;};
}
class CGraphicDataBank;class CMeshDataBank;class CInputManager;class CTCHeroExperience;class CTCInventoryAbilities;class CTCInventoryWeapons;class CTCInventoryMap;class CTCInventoryQuests;class CTCInventoryExperience;class CTCInventory;class CTCInventoryClothing;class CTCInventoryTrade;class CThing;class CDefinitionManager;class C2DVector{public:float X,Y;};enum EEngine2DLayer{UnspecifiedEngineLayer=0};
namespace NUISystem{enum EEvent{EVENT_DESCRIPTIVE_ZERO=0};class CComponent;class CActionParam;class CObserver;class CObservable{public:virtual void SetEvent(EEvent);virtual void SetOwnedEvent(CObserver*,EEvent);virtual void AddObserver(CObserver*);virtual void AddExclusiveObserver(CObserver*);virtual void RemoveExclusiveObserver(CObserver*);virtual void RemoveObserver(CObserver*);virtual void ClearObservers();virtual CObserver* GetExclusiveObserver();virtual bool IsExclusivelyObserved();virtual void AddConcurrentExclusiveObserver(CObserver*);virtual void RemoveConcurrentExclusiveObserver(CObserver*);std::list<CObserver*> Observers;CObserver* m_pExclusiveObserver;std::list<CObserver*> ConcurrentExclusiveObservers;};class CManager:public CObservable{public:enum EUsedKeys{UnspecifiedUsedKey=0};enum EInputType{UnspecifiedInputType=0};enum EChange{UnspecifiedChange=0};
CCountedPointer<CGraphicDataBank> PGraphicsBank;CCountedPointer<CMeshDataBank> PMeshBank;CInputManager * PInputManager;std::map<EUsedKeys,long> Keys;EInputType InputType;CTCHeroExperience * PTCHeroExperience;CTCInventoryAbilities * PAbilitiesInventory;CTCInventoryWeapons * PWeaponsInventory;CTCInventoryMap * PMapInventory;CTCInventoryQuests * PQuestInventory;CTCInventoryExperience * PExperienceInventory;CTCInventory * PInventory;CTCInventoryClothing * PInventoryClothing;CTCInventoryTrade * PInventoryTrade;CThing * PPlayer;std::list<long> ThingActionParams;std::list<CActionParam*> UIActionParams;CDefinitionManager * m_pDefinitionManager;unsigned long NumberAugmentations;EChange Change;std::map<long,EEngine2DLayer> Layers;long MetaLayer;CCountedPointer<CComponent> BastardChild;std::map<CCharString,CCountedPointer<CComponent> > BastardChildren;C2DVector RightStickForce;bool m_bHasErrorMessage;CComponent * m_pErrorMessage;bool Restoring;double m_fLastRenderTime;C2DVector MouseMovement;CComponent * PMouseCursor;CComponent * PDropped;wchar_t KeyPressed;float DescriptionScrollingOffset;bool DescriptionScrollingToMax;bool MouseDisabled;bool AssignmentItemsExpressionsSwapping;bool AssignmentSpellsSwapping;virtual ~CManager();virtual void SetKeyPressed(wchar_t);};void CManager::SetKeyPressed(wchar_t value){KeyPressed=value;}}
#ifdef _MSC_VER
typedef char ExactKeyOffset[offsetof(NUISystem::CManager,KeyPressed)==192?1:-1];
#endif