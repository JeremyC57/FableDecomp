// 00794e60 CTCUnholyPower::CancelForCutscene
// Leaf: ?CancelForCutscene@CTCUnholyPower@@QAEXXZ
// Flags:
#include "engine/CBaseClassNonCopyable.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingCreatureBase CThingCreatureBaseFields
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CEnvironment CEnvironmentFields
#include "engine/CEnvironment.h"
#undef CEnvironment
class CCharString;class CWorld;class CThing;class CBaseObject;class CBaseObjectPointer;
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(const CIntelligentPointer&o):CBaseIntelligentPointer(){SetPItem(o.GetPItem());}T*Get()const{return reinterpret_cast<T*>(GetPItem());}};
class C3DVector{public:float X,Y,Z;};
class CEnginePrimitiveThemeChannelDesc{public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;};
enum ETCInterfaceType{};class C2DVector;class CTCParamsBase;class CEnginePrimitivePrepareInfo;class CPersistContext;namespace NTCMessages{class CMessageBase;}
// Layout: shared CTCBase prefix; native table omits donor editor-only property transfer pair.
class CTCBase:public CBaseClassNonCopyable{protected:CWorld&GetWorld()const;CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;
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
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:iterator begin(){return static_cast<_Node*>(this->_M_header._M_data->_M_left);}iterator end(){return this->_M_header._M_data;}size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::iterator iterator;iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
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
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*end(){return this->_M_finish;}};
}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{private:C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);};
// Layout: shared CThing bitset20/map44 and original dead bit91.
class CThing:public CThingFields{public:void Kill(bool);void RemoveTC(ETCInterfaceType);bool IsDead()const{return (CreatedBySerialisation[0]&1)!=0;}
 __forceinline bool HasTC(ETCInterfaceType kind)const{if(reinterpret_cast<const unsigned long*>(UsingComponents)[kind>>5]&(1UL<<(kind&31)))return true;return false;}
 __forceinline CTCBase&GetTC(ETCInterfaceType kind){ETCInterfaceType key=kind;CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);return *((i==map.end()||kind<i->first)?map.end():i)->second;}
 template<class T>__forceinline bool GetTC(T**out){ETCInterfaceType kind=T::StatGetInterfaceType();if(HasTC(kind)){*out=static_cast<T*>(&GetTC(kind));return true;}return false;}
};
class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{public:void FinishCurrentAction();};
class CEnvironment:public CEnvironmentFields{public:void TransitionToDefaultThemeAllChannels(long,float);};
// Layout: shared world scoped environment pointer18; original getter returns CEnvironment&.
class CWorld:public CWorldFields{public:CEnvironment&GetEnvironment(){return *PEnvironment_ptr;}};
class CTCSoundPlayer:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x10);}void StopAttachedSound(unsigned long);};
enum EHeroAbility{};
// Layout: native ability mutex c/current enum10, raw PDB names and native interface a7 RTTI confirm.
class CTCSpecialAbilities:public CTCBase{private:bool AbilityMutexLock,UsingBed;EHeroAbility CurrentAbility;public:static void __fastcall FadeOrKillEffect(CIntelligentPointer<CThing>&);void UnLockAbilitiyMutex(){AbilityMutexLock=false;CurrentAbility=static_cast<EHeroAbility>(0);}};
// Layout: raw CTCUnholyPower typed base12; native CutSceneCancel6c, map78, effects9c/a4/b4/bc and LoopingSoundc4. Unread Timer/PTarget/HitEffects intervals are not claimed as new complete types.
class CTCUnholyPower:public CTCBase{
private:
 class PerCreatureInfo{public:CIntelligentPointer<CThingCreatureBase>ActualCreature;long FramesToGetHit;CIntelligentPointer<CThing>Effect;CEnginePrimitiveThemeChannelDesc DefaultChannel;};
 unsigned char unreadToCancel[0x6c-0xc];bool CutSceneCancel;unsigned char unreadTarget[0x78-0x6d];
 std::map<CThingCreatureBase*,PerCreatureInfo>LastFrameCreaturesHit;
 unsigned char unreadEffects[0x9c-0x84];CIntelligentPointer<CThing>BuildUpEffect,DarkFacesEffect;CEnginePrimitiveThemeChannelDesc HeroDefaultChannel,SpellExclusionChannel;CIntelligentPointer<CThing>HandEffect1,HandEffect2;unsigned long LoopingSound;
 void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase>);
 void StopSound(unsigned long sample){CTCSoundPlayer*sound;if(ParentThing.GetTC(&sound))sound->StopAttachedSound(sample);}
public:void CancelForCutscene();
};
void CTCUnholyPower::CancelForCutscene(){
 std::map<CThingCreatureBase*,PerCreatureInfo>::iterator end=LastFrameCreaturesHit.end();
 for(std::map<CThingCreatureBase*,PerCreatureInfo>::iterator it=LastFrameCreaturesHit.begin();it!=end;++it){CIntelligentPointer<CThingCreatureBase>creature=it->second.ActualCreature;ResetCreatureHitState(creature);}
 CutSceneCancel=true;
 GetWorld().GetEnvironment().TransitionToDefaultThemeAllChannels(2,0.001f);
 CThingCreatureBase&parent=static_cast<CThingCreatureBase&>(ParentThing);parent.FinishCurrentAction();
 if(parent.HasTC(static_cast<ETCInterfaceType>(0xa7)))static_cast<CTCSpecialAbilities&>(parent.GetTC(static_cast<ETCInterfaceType>(0xa7))).UnLockAbilitiyMutex();
 if(BuildUpEffect.Get()&&!BuildUpEffect.Get()->IsDead())BuildUpEffect.Get()->Kill(true);
 if(HandEffect1.Get()&&!HandEffect1.Get()->IsDead())CTCSpecialAbilities::FadeOrKillEffect(HandEffect1);
 if(HandEffect2.Get()&&!HandEffect2.Get()->IsDead())CTCSpecialAbilities::FadeOrKillEffect(HandEffect2);
 if(DarkFacesEffect.Get()&&!DarkFacesEffect.Get()->IsDead())DarkFacesEffect.Get()->Kill(true);
 StopSound(LoopingSound);
 parent.RemoveTC(static_cast<ETCInterfaceType>(0x86));
}
