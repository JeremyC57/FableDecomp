// 0057de57 CTCHeroStats::PrepareToKickHeroToMostRecentSafeRegion
// Leaf: ?PrepareToKickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ
// Flags: /O1 /Oy-
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#endif
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
 _Link_type&_M_leftmost()const{return(_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:iterator begin(){return iterator(_M_leftmost());}iterator end(){return iterator(this->_M_header._M_data);}
 size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}V&operator[](const K&);
};
template<class K,class C=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;
};
}


#pragma optimize("y",off)
#pragma optimize("s",on)
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();bool operator!=(const CCharString&)const;};
namespace std{
template<class T,class A>class _Vector_base{public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Vector_base(const A&);~_Vector_base();protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,allocator_type>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T value_type;};
}
// Named observedRegion prefix: two truevector12 thenName18, unusedtail omitted.
class CRegion{protected:std::vector<long>Maps,VisMaps;CCharString Name;public:const CCharString&GetName()const{return Name;}};
class C3DVector;class C2DBoxI;class CDrawMapInfo;
#include "engine/CBaseClassNonCopyable.h"
class CIDrawWorldMap{public:
 virtual long DrawGetNoMaps()const=0;
 virtual CDrawMapInfo DrawGetMapInfo(long)const=0;
 virtual C2DBoxI DrawGetDimensions()const=0;
 virtual long DrawGetGranularity()const=0;
 virtual bool DrawIsPosInMap(const C3DVector&)const=0;
 virtual float DrawGetSizeZAt(const C3DVector&,float)const=0;
 virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector&)const=0;
 virtual long DrawGetMyPlayerMapNumber()const=0;
 virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual long DrawGetFrame()const=0;
 virtual~CIDrawWorldMap();
};

// OriginalserviceAPI projection; unobserveddata/virtualtails are not claimed.
class CWorldMap:public CBaseClassNonCopyable,public CIDrawWorldMap{public:long GetLoadedRegion()const;const CRegion&GetRegion(long)const;};
// CWorld is used only as scopeforits staticGetFrame; no CWorld object isprojected.
class CWorld{public:static long __fastcall GetFrame();};
class CThingMarker;class CClass;
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
class CThing:public CThingFields{public:bool IsIncludedInThingScans()const;};
namespace std{
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*p):_M_node(p){};};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{typedef _List_node<T>_Node;_List_iterator(_Node*p):_List_iterator_base(p){}typename Traits::reference operator*()const{return static_cast<_Node*>(_M_node)->_M_data;}_List_iterator&operator++(){_M_node=_M_node->_M_next;return*this;}bool operator!=(const _List_iterator&p)const{return _M_node!=p._M_node;}};
template<class T,class A>class _List_base{protected:typedef _List_node<T>_Node;_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{public:typedef _List_iterator<T,_Const_traits<T> >const_iterator;const_iterator begin()const{return const_iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}const_iterator end()const{return const_iterator(this->_M_node._M_data);}};
}

class CThingFilter_HasDefName{private:CCharString Name;public:CThingFilter_HasDefName(const CCharString&name):Name(name){}bool operator()(const CThing&)const;};
class CThingFilter_HasScriptData{private:CCharString Data;public:CThingFilter_HasScriptData(const CCharString&data):Data(data){}bool operator()(const CThing&)const;};
template<class A,class B>class CFilter_Bind2{protected:const A&F1;const B&F2;public:CFilter_Bind2(const A&a,const B&b):F1(a),F2(b){}template<class T>bool operator()(const T&item)const{return F1(reinterpret_cast<const CThing&>(item))&&F2(reinterpret_cast<const CThing&>(item));}};
#define CThingSearchTools CThingSearchToolsFields
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
class CThingSearchTools:public CThingSearchToolsFields{protected:const std::list<CClass*>&PeekTypeList(long)const;public:template<class F,class T>bool GetPFirstLeafThing(const F&,T**)const;};
template<>__declspec(noinline)bool CThingSearchTools::GetPFirstLeafThing<CFilter_Bind2<CThingFilter_HasDefName,CThingFilter_HasScriptData>,CThingMarker>(const CFilter_Bind2<CThingFilter_HasDefName,CThingFilter_HasScriptData>&filter,CThingMarker**out)const{
 const std::list<CClass*>&things=PeekTypeList(11);
 for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it){CThingMarker*item=reinterpret_cast<CThingMarker*>(*it);if(reinterpret_cast<const CThing*>(item)->IsIncludedInThingScans()&&filter(*item)){*out=item;return true;}}
 return false;
}
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;protected:CWorld&GetWorld()const;const CWorldMap&PeekWorldMap()const;const CThingSearchTools&PeekThingSearchTools()const;};
class CTCHeroStats:public CTCBase{public:
 struct MirroredCriminalStatus{long Crimes[10];long LastTimeSeenByGuard,LastTimeExited;bool ExitedUsingTeleporter;float EntrancePosX,EntrancePosY,EntrancePosZ,EntranceAngle;};
 std::map<CCharString,MirroredCriminalStatus>OutstandingCrimes;
 void GetOutstandingFineAndFramesLeft(const CCharString&,long&,long&);
 void PrepareToKickHeroToMostRecentSafeRegion();
};
#define CTCHeroStats CTCHeroStatsFields
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
#define CRegion CRegionFields
#include "engine/CRegion.h"
#undef CRegion
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseFields));
template<class A,class B>inline CFilter_Bind2<A,B>__fastcall GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
void CTCHeroStats::PrepareToKickHeroToMostRecentSafeRegion(){
 long mostRecentExit=0;
 CCharString currentRegion=PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetName();
 CThingMarker*marker;
 if(PeekThingSearchTools().GetPFirstLeafThing(GFFilter_Bind2(CThingFilter_HasDefName("REGION_KICK_TO_POINT"),CThingFilter_HasScriptData(currentRegion)),&marker)){}else{
  for(std::map<CCharString,MirroredCriminalStatus>::iterator it=OutstandingCrimes.begin();it!=OutstandingCrimes.end();++it){
   long fine,framesLeft;
   GetOutstandingFineAndFramesLeft((*it).first,fine,framesLeft);
   if(fine==0&&(*it).first!=currentRegion){if((*it).second.LastTimeExited>mostRecentExit)mostRecentExit=(*it).second.LastTimeExited;}
  }
  if(mostRecentExit!=0&&OutstandingCrimes[currentRegion].LastTimeExited>0)OutstandingCrimes[currentRegion].LastTimeExited=GetWorld().GetFrame();
 }
}