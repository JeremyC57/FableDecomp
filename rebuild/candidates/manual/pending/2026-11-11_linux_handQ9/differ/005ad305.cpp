// CTCInventoryTrade::ApplyTattooCard(long itemDefIndex) -- applies a bought tattoo card to the hero: the card def's tattoo layers and the appearance morph (prior draft, hand680)
// Grade: DIFFER(ours v retail) -- DIFFER(620v620): regraded as it sits with FABLE_HDR_OVERLAY (2026-11-11), residue unchanged from the prior draft: a three-way cycle in the second half (the appearance-morph block): retail morph->edi (spilled to [ebp-10h]), the zero constant->esi, the exclusive-items cursor->ebx; built morph->esi, zero->ebx, cursor->esi. Same frame (3Ch), slots, instructions and layout.
// Earlier grade (prior draft): DIFFER(620v620) -- a three-way cycle in the second half (the appearance-morph block): retail morph->edi (spilled to [ebp-10h]), the zero constant->esi, the exclusive-items cursor->ebx; built morph->esi, zero->ebx, cursor->esi. Same frame (3Ch), slots, instructions and layout.
// Next step: 12 shapes (initialisation of morph and heroMorph, layers/def order, signed count, count placement, inner block, guard forms) leave the cycle. Try the real CTCHeroMorph::AddTattoo overload set (the const-reference item form failed to compile against the local model) and the real std::set<long> insert, or treat as a reg-cycle lane gap.
// Leaf: ?ApplyTattooCard@CTCInventoryTrade@@QAEXJ@Z
// Flags: /O1 /Oy-
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CBaseClassNonCopyable.h"
#define CTCInventoryTrade CTCInventoryTradeFields
#include "engine/CTCInventoryTrade.h"
#undef CTCInventoryTrade
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCHeroMorph CTCHeroMorphFields
#include "engine/_quarantine/CTCHeroMorph.h"
#undef CTCHeroMorph
#define CTattooDef CTattooDefFields
#include "engine/CTattooDef.h"
#undef CTattooDef
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingBaseDef CThingBaseDefFields
#include "engine/_quarantine/CThingBaseDef.h"
#undef CThingBaseDef
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CDisplayEngine CDisplayEngineFields
#include "engine/CDisplayEngine.h"
#undef CDisplayEngine
namespace std{
typedef unsigned int size_t;typedef int ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator()throw(){}allocator(const allocator&)throw(){}template<class U>allocator(const allocator<U>&)throw(){}~allocator()throw(){}};
template<class T,class A>struct _Alloc_traits{typedef typename A::template rebind<T>::other allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef const T*pointer;typedef const T&reference;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}
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
public:size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;
};
template<class K,class C=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;
public:typedef _Rb_tree_iterator<K,_Const_traits<K> >iterator;set();~set();pair<iterator,bool>insert(const K&);
};
}


namespace std{
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector();unsigned int size()const{return this->_M_finish-this->_M_start;}bool empty()const{return this->_M_start==this->_M_finish;}T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}T*begin(){return this->_M_start;}const T*begin()const{return this->_M_start;}T*end(){return this->_M_finish;}const T*end()const{return this->_M_finish;}void push_back(const T&);};
template<unsigned int N>struct _Base_bitset{unsigned long _M_w[N];};
template<unsigned int N>class bitset:private _Base_bitset<(N+31)/32>{public:bool test(unsigned int)const;};
}
template<class T>class CArray:public std::vector<T>{};
class CTCBase;enum ETCInterfaceType{};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:__declspec(noinline)std::pair<K,V>*LowerBound(const K&key){std::pair<K,V>*first=this->begin();long count=this->end()-first;while(count>0){long half=count/2;std::pair<K,V>*middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}__declspec(noinline)const std::pair<K,V>*LowerBound(const K&key)const{const std::pair<K,V>*first=this->begin();long count=this->end()-first;while(count>0){long half=count/2;const std::pair<K,V>*middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}};
// Layout: shared CThing component bitset20 and map44 projected to original STLport types.
class CThing:public CThingFields{public:
 template<class T>FABLE_FORCEINLINE bool GetTC(T**out){return GetTC(T::StatGetInterfaceType(),out);}
 template<class T>FABLE_FORCEINLINE bool GetTC(ETCInterfaceType kind,T**out){if(reinterpret_cast<std::bitset<274>&>(UsingComponents).test(kind)){ETCInterfaceType key=kind;CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(key);*out=reinterpret_cast<T*>((it==map.end()||kind<it->first?map.end():it)->second);return true;}return false;}
 template<class T>FABLE_FORCEINLINE bool PeekTC(T**out)const{ETCInterfaceType kind=T::StatGetInterfaceType();if(reinterpret_cast<const std::bitset<274>&>(UsingComponents).test(kind)){ETCInterfaceType key=kind;const CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<const CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);const std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(key);*out=reinterpret_cast<T*>((it==map.end()||kind<it->first?map.end():it)->second);return true;}return false;}
};
class CTCParamsBase;
// Layout: shared CTCBase12 restored to its original reference member and virtual prefix through FrameUpdate(slot9).
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;protected:virtual void OnSerialise(CPersistContext&,bool);public:virtual~CTCBase();virtual void OnRemove();virtual void ConstructFromParams(const CTCParamsBase&);virtual void OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();virtual void RenderUpdate(float);virtual bool HasFrameUpdate()const;virtual void FrameUpdate();};
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CDefinitionManager:public CDefinitionManagerFields{public:CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long);};
class CThingBaseDef:public CParentDefClassBase{};
enum ECompositeBlendType{};
// Layout: shared CTattooDef offsets28..3d; the unobserved remainder is omitted, not asserted as a complete object.
class CTattooDef:public CSubDefClassBase{public:CCharString TattooName;long TextureLayer,ReplacingTextureIndex,BankIndex;ECompositeBlendType Blend;bool RemovesAllTattoos,Permanent;static CCharString __fastcall StatGetTypeName(){return CCharString("CTattooDef");}};
template<class T>__declspec(noinline)bool CDefClassBase::GetSubDef(CDefPointer<const T>*out)const{const CSubDefInfo*info=GetPSubDefInfo(T::StatGetTypeName());if(!info)return false;CDefPointer<const T>value(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));out->SetDynamicCast(value);return true;}
class CIEngine;
class CTCAppearanceMorphBase:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(3);}void UploadTextureMorphsToEngine(CIEngine&,const std::set<long>*,bool,float);};
// Layout: hero methods use the shared CTCBase subobject; no unread hero state is modelled.
class CTCHeroMorph:public CTCAppearanceMorphBase{public:virtual void FrameUpdate();void GetTattooThings(std::vector<CDefPointer<const CThingBaseDef> >&)const;void AddTattoo(long);void AddTattoo(CDefPointer<const CThingBaseDef>&);};
class CTCInventoryItem:public CTCBase{public:static CDefPointer<const CTattooDef>__fastcall GetTattooCardDef(long);};
class CDisplayEngine:public CDisplayEngineFields{public:CIEngine&Get3DEngine(){return *Engine3D;}};
namespace boost{template<class T>class scoped_ptr{T*ptr;public:T&operator*()const{return *ptr;}};}
// Layout: original MainGameComponent display engine at28, native engine pointer at2c.
class CMainGameComponent{protected:static CMainGameComponent*PMainGameComponent;unsigned char _pad_0[0x28];boost::scoped_ptr<CDisplayEngine>PDisplayEngine;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}CDisplayEngine&GetDisplayEngine(){return *PDisplayEngine;}};
// Layout: shared CTCInventoryTrade native receiver230; only its CTCBase ParentThing reference4 is accessed.
class CTCInventoryTrade:public CTCBase{private:unsigned char _pad_0x0c[sizeof(CTCInventoryTradeFields)-sizeof(CTCBase)];public:void ApplyTattooCard(long);};
void CTCInventoryTrade::ApplyTattooCard(long defIndex)
{
    std::vector<CDefPointer<const CThingBaseDef> > tattoos;
    std::vector<CDefPointer<const CThingBaseDef> > exclusive;
    {
        const CTCHeroMorph* morph;
        if (static_cast<const CThing&>(ParentThing).PeekTC(&morph))
            morph->GetTattooThings(tattoos);
    }
    for (unsigned int i = 0; i < tattoos.size(); ++i) {
        CDefPointer<const CTattooDef> def;
        if (tattoos[i]->GetSubDef(&def) && def->Permanent)
            exclusive.push_back(tattoos[i]);
    }
    CTCHeroMorph* heroMorph = 0;
    if (ParentThing.GetTC(&heroMorph)) {
        heroMorph->AddTattoo(defIndex);
        heroMorph->FrameUpdate();
    }
    CTCAppearanceMorphBase* morph;
    if (ParentThing.GetTC(&morph)) {
        CDefPointer<const CTattooDef> tattoo = CTCInventoryItem::GetTattooCardDef(defIndex);
        std::set<long> layers;
        if (tattoo->RemovesAllTattoos) {
            morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), 0, false, 0.0f);
            unsigned int count = exclusive.size();
            CDefPointer<const CThingBaseDef>* items = exclusive.begin();
            for (unsigned int i = 0; i < count; ++i) {
                if (heroMorph) {
                    heroMorph->AddTattoo(items[i]);
                    heroMorph->FrameUpdate();
                }
                CDefPointer<const CTattooDef> def;
                if (items[i]->GetSubDef(&def))
                    layers.insert(def->ReplacingTextureIndex);
            }
            if (!exclusive.empty())
                morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), &layers, false, 0.0f);
        } else {
            layers.insert(tattoo->ReplacingTextureIndex);
            morph->UploadTextureMorphsToEngine(CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine(), &layers, false, 0.0f);
        }
    }
}
