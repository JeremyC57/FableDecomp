// 00773790 CTCShop::GetAttitudeSellPriceMultiplier
// Grade: DIFFER(462v448) -- (smallest masked diff, 123 lines; the prior is 430v448 with 131) register colouring and exit layout. Retail: this->ebp, ParentThing->edi, the zero register ebx pushed only after the house early return, so the two `return 1.0f` exits stay separate; thingDef is cached in esi. Declaring GetDef<T> without the same-TU noinline body gives retail's this/thing roles (probe g, 450v448), but the CDefPointer frame slots and the release code still differ.
// Next step: real out-of-TU GetDef/GetSubDef (declared only) plus the real CDefPointer release shape. Find why VC7.1 puts thingDef at orig+4 instead of retail's orig+0 (slot sharing with the house lookup key), then the zero-register choice.
// Leaf: ?GetAttitudeSellPriceMultiplier@CTCShop@@QBEMXZ
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CPlayer CPlayerFields
#include "engine/CPlayer.h"
#undef CPlayer
#define CTCHeroStats CTCHeroStatsFields
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
#define CTCBuyableHouse CTCBuyableHouseFields
#include "engine/CTCBuyableHouse.h"
#undef CTCBuyableHouse
#define CShopDef CShopDefFields
#include "engine/_quarantine/CShopDef.h"
#undef CShopDef
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CThingBaseDef CThingBaseDefFields
#include "engine/_quarantine/CThingBaseDef.h"
#undef CThingBaseDef
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CBaseObject;class CBaseObjectPointer;class CWorld;class CGameDefinitionManager;class CPlayerManager;
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*Get()const{return reinterpret_cast<T*>(GetPItem());}};
enum ETCInterfaceType{};class C2DVector;class CTCParamsBase;class CEnginePrimitivePrepareInfo;class CPersistContext;namespace NTCMessages{class CMessageBase;}
// Layout: shared CTCBase prefix; native table omits donor editor-only property transfer pair.
class CTCBase:public CBaseClassNonCopyable{protected:const CWorld&PeekWorld()const;const CGameDefinitionManager&GetDefinitionManager()const;const CPlayerManager&PeekPlayerManager()const;CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;
protected:virtual void OnSerialise(CPersistContext&,bool);
public:virtual void OnRemove();
public:virtual void ConstructFromParams(const CTCParamsBase&);
public:virtual void OnCreate();
public:virtual void InitialActivate();
public:virtual void OnInitialActivate();
public:virtual void RenderUpdate(float);
public:virtual bool HasFrameUpdate()const;
public:virtual void FrameUpdate();
public:virtual void BulletTimeFrameUpdate();
public:virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public:virtual void Copy(const CTCBase&);
public:virtual void OnDie();
public:virtual void OnKill();
public:virtual void OnLevelUnload();
public:virtual void OnSetInLimbo(bool);
public:virtual bool Validate(CCharString&)const;
public:virtual bool IsFreeForDeletion()const;
public:virtual bool IsSerialiseCreatable()const;
public:virtual bool IsSerialisableForHeroState()const;
public:virtual ETCInterfaceType GetInterfaceType()const;
public:virtual CCharString GetClassTypeName()const;
public:virtual bool IsComponentSerialisable()const;
public:virtual bool IsDriver()const;
public:virtual bool IsCopyable()const;
public:virtual void GlobalUpdate(bool);
public:virtual void HandleMessage(const NTCMessages::CMessageBase&);
};
namespace std{
typedef unsigned int size_t;typedef int ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}};
template<class T,class A>struct _Alloc_traits{typedef typename A::template rebind<T>::other allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}pointer operator->()const{return &operator*();}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}_Self&operator--(){_M_node=_Rb_global_inst::_M_decrement(_M_node);return*this;}
};
template<class V,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<V>_Node;typedef typename _Alloc_traits<V,A>::allocator_type allocator_type;
 _Rb_tree_base(const allocator_type&);~_Rb_tree_base();
protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;
};
template<class K,class V,class Key,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{
 typedef _Rb_tree_base<V,A>_Base;
protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;typedef _Rb_tree_Color_type _Color_type;
public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;
protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;const_iterator find(const K&)const;const_iterator end()const{return const_iterator(this->_M_header._M_data);}
private:iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:iterator begin(){return static_cast<_Node*>(this->_M_header._M_data->_M_left);}iterator end(){return this->_M_header._M_data;}size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::const_iterator const_iterator;const_iterator find(const K&key)const{return _M_t.find(key);}const_iterator end()const{return _M_t.end();}typedef typename _Rep_type::iterator iterator;iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
};
template<class K,class C=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;
};
}
namespace std{
template<class Dummy>inline typename _Rb_global<Dummy>::_Base_ptr __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr parent=node->_M_parent;while(node==parent->_M_right){node=parent;parent=parent->_M_parent;}if(node->_M_right!=parent)node=parent;}return node;
}
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*begin(){return this->_M_start;}const T*begin()const{return this->_M_start;}T*end(){return this->_M_finish;}const T*end()const{return this->_M_finish;}};
}
template<class T>class CArray:public std::vector<T>{};

template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:__declspec(noinline)std::pair<K,V>*LowerBound(const K&key){std::pair<K,V>*first=this->begin();long count=this->end()-first;while(count>0){long half=count/2;std::pair<K,V>*middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}__declspec(noinline)const std::pair<K,V>*LowerBound(const K&key)const{const std::pair<K,V>*first=this->begin();long count=this->end()-first;while(count>0){long half=count/2;const std::pair<K,V>*middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}};
// Layout: shared CThing component bitset20/map44, and BaseDefIndex8c. No replacement layout is invented.
class CThing:public CThingFields{public:
 template<class T>__forceinline bool PeekTC(const T**out)const{ETCInterfaceType kind=T::StatGetInterfaceType();if(HasTC(kind)){*out=static_cast<const T*>(&PeekTC(kind));return true;}return false;}
 unsigned long GetDefGlobalIndex()const{return DefGlobalIndex;}
 bool HasTC(ETCInterfaceType type)const{if(reinterpret_cast<const unsigned long*>(UsingComponents)[type>>5]&(1UL<<(type&31)))return true;return false;}
 __forceinline CTCBase&GetTC(ETCInterfaceType type){ETCInterfaceType key=type;CVectorMap<ETCInterfaceType,CTCBase*>&map=*reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>*>(Components);std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(key);return *((it==map.end()||type<it->first)?map.end():it)->second;}
 __forceinline const CTCBase&PeekTC(ETCInterfaceType type)const{ETCInterfaceType key=type;const CVectorMap<ETCInterfaceType,CTCBase*>&map=*reinterpret_cast<const CVectorMap<ETCInterfaceType,CTCBase*>*>(Components);const std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(key);return *((it==map.end()||type<it->first)?map.end():it)->second;}
};
class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{};class CThingAICreature:public CThingCreatureBase{};class CThingPlayerCreature:public CThingAICreature{};
class CTCBuyableHouse:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0xd5);}bool GetOwnedByPlayer()const{return reinterpret_cast<const CTCBuyableHouseFields*>(this)->OwnedByPlayer;}};
class CTCHeroStats:public CTCBase{public:float GetTradeSkill()const;};
class CPlayer:public CPlayerFields{public:CThingPlayerCreature*GetPControlledCreature()const;};
class CPlayerManager{public:const CPlayer&GetMainPlayer()const;};
enum EOpinionAttitudeType{};
class COpinionReactionManager{public:EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing&)const;};
class CWorld:public CWorldFields{public:const COpinionReactionManager&PeekOpinionReactionManager()const{return *POpinionReactionManager_Data;}};
class CThingBaseDef:public CParentDefClassBase{};
// Layout: shared CShopDef flattened members supply defaults70 and map74, each map12bytes; original polymorphic base retained.
class CShopDef:public CSubDefClassBase{public:static CCharString __fastcall StatGetTypeName(){return CCharString("CShopDef");}};
class CDefinitionManager:public CDefinitionManagerFields{friend class CDefClassBase;protected:CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long);public:CDefPointer<const CDefClassBase>GetPDefFromGlobalIndex(long)const;
 template<class T>bool GetDef(long index,CDefPointer<const T>*out)const;
};
// Layout: raw donor keeps derived manager forward-only; no derived fields are read here. Base public template is called by the native row.
class CGameDefinitionManager:public CDefinitionManager{};
template<class T>__declspec(noinline)bool CDefClassBase::GetSubDef(CDefPointer<const T>*out)const{const CSubDefInfo*info=GetPSubDefInfo(T::StatGetTypeName());if(!info)return false;CDefPointer<const T>value(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));out->SetDynamicCast(value);return true;}
// Layout: original CTCBase12, public StockStatec, private intelligent keeper10; remaining shop fields are unread.
class CTCShop:public CTCBase{public:enum ESTOCK_STATE{};ESTOCK_STATE StockState;private:CIntelligentPointer<CThingCreatureBase>PShopKeeper;public:float GetAttitudeSellPriceMultiplier()const;};
// File-local original float, tc_shop.obj DATA32, donor3+12a80. Retail1382998 and six following vector/colour objects preserve the donor order.
static float MAXIMUM_TRADE_SKILL=0.84f;
float CTCShop::GetAttitudeSellPriceMultiplier()const{
 const CThing&thing=ParentThing;
 const CTCBuyableHouse*house;if(thing.PeekTC(&house)){if(house->GetOwnedByPlayer())return 1.0f;}
 CDefPointer<const CShopDef>shopDef;CDefPointer<const CThingBaseDef>thingDef;
 GetDefinitionManager().GetDef(thing.GetDefGlobalIndex(),&thingDef);
 if(!thingDef->GetSubDef(&shopDef))return 1.0f;
 const CShopDefFields&fields=*reinterpret_cast<const CShopDefFields*>(shopDef.Get());
 float multiplier=fields.AttitudeSellPriceMultiplierDefault;
 if(PShopKeeper.Get()&&PShopKeeper.Get()->HasTC(static_cast<ETCInterfaceType>(0x4f))){
  const COpinionReactionManager&reactions=PeekWorld().PeekOpinionReactionManager();
  EOpinionAttitudeType attitude=reactions.GetThingAttitudeToPlayer(*PShopKeeper.Get());
  const std::map<EOpinionAttitudeType,float>&map=*reinterpret_cast<const std::map<EOpinionAttitudeType,float>*>(fields.AttitudeSellPriceMultiplier);
  std::map<EOpinionAttitudeType,float>::const_iterator it=map.find(attitude);if(it!=map.end())multiplier=it->second;
 }
 CThingPlayerCreature*hero=PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
 if(hero&&hero->HasTC(static_cast<ETCInterfaceType>(4))){float skill=static_cast<CTCHeroStats&>(hero->GetTC(static_cast<ETCInterfaceType>(4))).GetTradeSkill();multiplier+=(1.0f-multiplier)*(skill*MAXIMUM_TRADE_SKILL);}
 return multiplier;
}
