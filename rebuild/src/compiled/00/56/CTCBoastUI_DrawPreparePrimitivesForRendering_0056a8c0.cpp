// CTCBoastUI::DrawPreparePrimitivesForRendering -- actual UI update/draw using interpolation time andengine.
// Leaf: ?DrawPreparePrimitivesForRendering@CTCBoastUI@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O2 /Oy /W3
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
};
}

class CCharStringData;class CWideStringData;
class CCharString{public:CCharStringData*PStringData;CCharString(const char*,long=-1);~CCharString();};
class CWideString{public:CWideStringData*PStringData;CWideString();CWideString(const CWideString&);~CWideString();};
class C2DVector;class C2DBoxF;class CRGBColour;class CIEngine;class CEnginePrimitiveHandle;class CFontBank;
template<class T>class CDefPointer;class CCPPointerInfo;
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;CCountedPointer(T*);CCountedPointer(const CCountedPointer&);~CCountedPointer();T*operator->()const{return Data;}};
namespace std{template<class T,class A=allocator<T> >class vector;template<class T,class A=allocator<T> >class list;}
#ifndef PASS9_UI_BASE_INTERFACES_HPP
#define PASS9_UI_BASE_INTERFACES_HPP
// Include after original STLport set/allocator and the referenced forward types.
// Raw tables in UI_RAW_TABLES.json; Observer exact six nonabstract APIs in both
// original PDBs. Pure slots are fixed by actual complete derived override tables.
namespace NUISystem {
class CComponent;
enum EEvent {};enum EType {};enum EActionParamType {};
class CAction;class CUIState;class CUIDef;class CDeletion;
class CUIInterface {
public:virtual~CUIInterface();virtual void Update(float)=0;
 virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*)=0;
};
class CObserver {
public:virtual~CObserver();virtual void ProcessEvent(EEvent)=0;
 virtual bool IsObservingEvent(EEvent);virtual void ObserveEvent(EEvent);
 virtual void IgnoreEvent(EEvent);virtual void IgnoreAllEvents();
 virtual void ObserveAllEvents();virtual void SetPreventObserving(bool);
 std::set<EEvent>EventsToObserve;bool PreventObservation;
};
class CActionParam {
public:virtual~CActionParam();EActionParamType ActionParamType;
};
}
#endif
namespace NUISystem{class CComponent:public CUIInterface,public CObserver,public CActionParam{
// ORIGINAL declaration fragment; insert inside actual NUISystem::CComponent.
// Not a standalone runtime base/model: real base chain must be CUIInterface,
// CObserver, CActionParam with their own independently recovered prefixes.
// Every133 original method appears once, physicalretail slots1..133.
public:
 virtual ~CComponent(); // native0, true overriding destructor; exact publicUAE destructor in both original PDBs.
 virtual void Update(float); // native1
 virtual void Draw(CIEngine &, CEnginePrimitiveHandle *, long, long *, NUISystem::CComponent *); // native2
 virtual NUISystem::CComponent * FindChild(CCharString); // native3
 virtual NUISystem::CComponent * FindChildForward(CCharString); // native4
 virtual void FindAllChildren(CCharString, std::list<NUISystem::CComponent *, std::allocator<NUISystem::CComponent *> > &); // native5
 virtual void FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *, std::allocator<NUISystem::CComponent *> > &); // native6
 virtual void SetPAcceptButton(NUISystem::CComponent*); // native7; FW16387 sourceTU order41c580->41c590
 virtual NUISystem::CComponent * FindChildNonRecursive(CCharString); // native8
 virtual NUISystem::CComponent * FindFirstChildOfType(NUISystem::EType); // native9
 virtual bool HasChild(NUISystem::CComponent *) const; // native10
 virtual void SetZoom(C2DVector const &); // native11
 virtual C2DVector GetZoom() const; // native12
 virtual void SetColour(CRGBColour const &); // native13
 virtual CRGBColour GetColour() const; // native14
 virtual void SetPosition(C2DVector const &); // native15
 virtual C2DVector GetPosition() const; // native16
 virtual C2DVector GetParentPosition() const; // native17
 virtual void SetParentPosition(C2DVector &); // native18
 virtual C2DVector GetParentZoom() const; // native19
 virtual void SetParentZoom(C2DVector &); // native20
 virtual CRGBColour GetParentColour() const; // native21
 virtual void SetParentColour(CRGBColour &); // native22
 virtual C2DVector GetSize() const; // native23
 virtual C2DBoxF GetBoundingBox() const; // native24
 virtual C2DVector const & GetRenderPosition() const; // native25
 virtual C2DVector const & GetRenderZoom() const; // native26
 virtual CRGBColour const & GetRenderColour() const; // native27
 virtual void SetRenderPosition(C2DVector &); // native28
 virtual void SetRenderZoom(C2DVector &); // native29
 virtual void SetRenderColour(CRGBColour &); // native30
 virtual void ChangePosition(C2DVector const &, float, bool); // native31
 virtual void ChangePositionDelta(C2DVector const &, float, bool); // native32
 virtual void ChangeTargetPositionDelta(C2DVector const &, float, bool); // native33
 virtual void UpdatePosition(float); // native34
 virtual void ChangeZoom(C2DVector const &, float, bool); // native35
 virtual void ChangeZoomDelta(C2DVector const &, float, bool); // native36
 virtual void UpdateZoom(float); // native37
 virtual void ChangeColour(CRGBColour const &, float, bool); // native38
 virtual void ChangeColourDelta(CRGBColour const &, float, bool); // native39
 virtual void UpdateColour(float); // native40
 virtual void ResetState(); // native41
 virtual void ForceState(unsigned long, bool); // native42
 virtual void Initialise(); // native43
 virtual bool HasState(unsigned long); // native44
 virtual bool HasStateInChildren(unsigned long); // native45
 virtual void ChangeState(unsigned long); // native48, MSVC newoverload group reversesdeclarationorder
 virtual void ChangeState(unsigned long, float); // native47
 virtual void ChangeState(NUISystem::CUIState &); // native46
 virtual bool ChangedState(); // native49
 virtual NUISystem::CUIState * GetCurrentState(); // native50
 virtual void SetParent(NUISystem::CComponent *); // native51
 virtual NUISystem::CComponent * GetParent() const; // native52
 virtual std::vector<CCountedPointer<NUISystem::CComponent>, std::allocator<CCountedPointer<NUISystem::CComponent> > > & GetChildren(); // native53
 virtual float GetUpdateTime() const; // native54
 virtual C2DVector GetTargetPosition() const; // native55
 virtual CRGBColour GetTargetColour() const; // native56
 virtual C2DVector GetTargetZoom() const; // native57
 virtual void ProcessChangeState(); // native58
 virtual void AddChild(CCountedPointer<NUISystem::CComponent> &); // native59
 virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &); // native60
 virtual void AddChildFront(CCountedPointer<NUISystem::CComponent> &); // native61
 virtual void SetDeletion(NUISystem::CDeletion); // native62
 virtual NUISystem::CDeletion & GetDeletion(); // native63
 virtual void RemoveChild(unsigned long); // native64
 virtual NUISystem::EType GetType() const; // native65
 virtual void AddAction(CCountedPointer<NUISystem::CAction> &); // native66
 virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction> &); // native67
 virtual void AddActionOnBack(CCountedPointer<NUISystem::CAction> &); // native68
 virtual void AddPreAction(CCountedPointer<NUISystem::CAction> &); // native69
 virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction> &); // native70
 virtual void AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &); // native71
 virtual void AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &); // native72
 virtual void AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &); // native73
 virtual void AddActionOnDropped(CCountedPointer<NUISystem::CAction> &); // native74
 virtual void AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &); // native75
 virtual void AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &); // native76
 virtual void AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &); // native77
 virtual void AddActionOnHovered(CCountedPointer<NUISystem::CAction> &); // native78
 virtual void AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &); // native79
 virtual void AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &); // native80
 virtual void Die(); // native81
 virtual std::map<unsigned long, NUISystem::CUIState, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, NUISystem::CUIState> > > & GetStates(); // native82
 virtual void SetName(CCharString const &); // native83
 virtual CCharString GetName(); // native84
 virtual NUISystem::CComponent * FindComponent(NUISystem::CComponent *); // native85
 virtual unsigned long GetCurrentStateIndex(); // native86
 virtual unsigned long GetTargetStateIndex() const; // native87
 virtual float CanScroll(bool, C2DBoxF &); // native88
 virtual void FakeChildDying(NUISystem::CComponent *); // native89
 virtual void SetParentViewport(NUISystem::CComponent *); // native90
 virtual void ClearHandle(); // native91
 virtual void MakeUnavailable(); // native92
 virtual void MakeUnavailableDynamically(); // native93
 virtual void MakeAvailable(); // native94
 virtual void MakeAvailableDynamically(); // native95
 virtual void AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &); // native96
 virtual NUISystem::CComponent * FindFirstChildOfTypeNonRecursive(NUISystem::EType); // native97
 virtual bool IsAllowedToScroll() const; // native98
 virtual NUISystem::CComponent * GetParentViewport(); // native99
 virtual bool IsBastardChild() const; // native100
 virtual bool IsIndependent() const; // native101
 virtual bool IsPositionIndependent() const; // native102
 virtual bool IsZoomIndependent() const; // native103
 virtual bool IsLayerIndependent() const; // native104
 virtual bool IsDrawFromViewport() const; // native105
 virtual bool IsPositionIsCenter() const; // native106
 virtual long GetDefIndex() const; // native107
 virtual CDefPointer<NUISystem::CUIDef const> LoadAndReturnDef() const; // native108
 virtual void OnObserverRemove(); // native109
 virtual C2DVector CanMove(C2DVector const &); // native110
 virtual C2DVector GetDynamicSize() const; // native111
 virtual C2DVector GetTargetRenderPosition() const; // native112
 virtual bool IsPCStyle() const; // native113
 virtual void SetRelativeParentPosition(C2DVector &); // native114
 virtual void SetRelativeParentZoom(C2DVector &); // native115
 virtual bool IsUsingRelativeZoom() const; // native116
 virtual bool IsUsingRelativePosition() const; // native117
 virtual C2DVector GetRelativeRenderPosition(); // native118
 virtual C2DVector GetRelativeRenderZoom(); // native119
 virtual void SetRelativeRenderPosition(C2DVector); // native120
 virtual C2DVector SetRelativeRenderZoom(C2DVector); // native121
 virtual C2DVector GetTargetRelativeRenderPosition() const; // native122
 virtual C2DVector GetTargetRelativeRenderZoom() const; // native123
 virtual void AddPositionChild(NUISystem::CComponent *); // native124
 virtual void SetPositionParent(NUISystem::CComponent *); // native125
 virtual NUISystem::CComponent * GetPositionParent(); // native126
 virtual void PositionChildDying(NUISystem::CComponent *); // native127
 virtual void PositionParentDying(NUISystem::CComponent *); // native128
 virtual long GetRenderLayer() const; // native129
 virtual void AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &); // native130
 virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>, std::allocator<CCountedPointer<NUISystem::CAction> > > *); // native131
 virtual bool IsClickable() const; // native132
 virtual void RemoveFromHoveredList(); // native133
 virtual void ProcessEvent(NUISystem::EEvent); // actualObserver secondaryslot1, not aprimaryslot; native0052e830

};

class CChangingStateComponent:public CComponent{
public:
 virtual CUIState*GetState(unsigned long); //135, MSVC reversesoverloadgroup
 virtual const CUIState*GetState(unsigned long)const; //134
 virtual void UpdateStateChange(); //136
 virtual bool ChangedStateLastUpdate(); //137
 virtual bool InternalChanged(); //138
protected:
 virtual bool AllChildrenChangedState(); //139
 virtual void OnActivate(); //140
 virtual void OnDeactivate(); //141
};
enum ETextAlignement{};
class CText:public CChangingStateComponent{
public:
 virtual const CWideString&GetValue(); //142
 virtual void SetValue(const CCharString&,bool,bool); //145, MSVC reversesoverloadgroup
 virtual void SetValue(const CWideString&,bool,bool); //144
 virtual void SetValue(unsigned long,const CWideString&,bool); //143
 virtual CCountedPointer<CFontBank>&GetFont(); //146
 virtual void SetTextWindow(C2DBoxF&); //147
 virtual C2DBoxF&GetTextWindow(); //148
 virtual void ForceWidth(float); //149
 virtual ETextAlignement GetAlignment()const; //150
};
class CObservable{public:virtual void SetEvent(EEvent);~CObservable();};
class CManager:public CObservable{public:static CManager*__fastcall GetInstance();CComponent*CreateComponent(const CCharString&,bool);};
}
class CBaseClass{public:CBaseClass();virtual~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual~CBaseClassNonCopyable();};
class CThing;class CTCBase:public CBaseClassNonCopyable{public:virtual~CTCBase();CThing*ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;};
namespace NInventory{class CTCInventoryBase:public CTCBase{};}
namespace NHeroInformationScreens{class CBase{};}
class CDeviceResetCallback{public:virtual void OnPreDeviceReset()=0;virtual bool OnPostDeviceReset()=0;};
class CGame;class CGameComponent:public CBaseClassNonCopyable,public CDeviceResetCallback{public:virtual~CGameComponent();bool Quit,Running;CGame*Game;};
class CBankFile:public CBaseClassNonCopyable{};class CBankFileAsync:public CBankFile{};class CResourceBank:public CBaseClassNonCopyable{};
namespace NGameText{class CDataBank:public CBankFileAsync,public CResourceBank{public:const CWideString GetTextBySymbol(const CCharString&)const;};}
class CASoundBank;class CMainGameComponent:public CGameComponent{public:CASoundBank*PSampleBank;CCountedPointer<NGameText::CDataBank>PTextBank;};
extern CMainGameComponent*CUnprovenGlobal_013b86a0;

#define CTCBoastUI CTCBoastUIFields
#include "engine/CTCBoastUI.h"
#undef CTCBoastUI
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
namespace std{
template<class T,class A>class _Vector_base{public:typedef typename _Alloc_traits<T,A>::allocator_type allocator_type;_Vector_base(const A&);~_Vector_base();
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,allocator_type>_M_end_of_storage;};
template<class T,class A>class vector:public _Vector_base<T,A>{
public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T*iterator;typedef const T*const_iterator;typedef T&reference;typedef const T&const_reference;typedef size_t size_type;typedef ptrdiff_t difference_type;
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 reference operator[](size_type n){return *(begin()+n);}const_reference operator[](size_type n)const{return *(begin()+n);}
};
}
class CQuestManager{public:class CBoast{public:long ID,Cost,Reward;CCharString ScriptName;CWideString Text,Title;bool Evil,Failed,Completed;long GroupIndex;};};
namespace NUISystem{class CMenuEntry;}
class CInterpolationInfo{public:class CInterpolationInfoSet{public:float GTPredictedTimeSinceLastRenderFrame,WFInterpolate;};CInterpolationInfoSet Current,Last,PausedCurrent,PausedLast,BulletTimeCurrent,BulletTimeLast;long WFServerCurrentTime;};
// Exactobserved info prefixthroughEngine34. LaterEdit2DEngine/Camera data is
// unobserved here andnofullsizeofCEnginePrimitivePrepareInfo isclaimed.
class CEnginePrimitivePrepareInfo{public:CInterpolationInfo Interpolate;CIEngine*Engine;};
class CTCBoastUI:public CTCBase{public:virtual~CTCBoastUI();std::map<NUISystem::CMenuEntry*,CQuestManager::CBoast>BoastGroups;bool Active;std::vector<NUISystem::CComponent*>Components;bool Quitting,AllowedToOpen,ActivationRequested,ActivateLater;
 virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseFields));
FABLE_STATIC_ASSERT(sizeof(CTCBoastUI)==sizeof(CTCBoastUIFields));
void CTCBoastUI::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&info){
 if(Active&&!Quitting){
  float dt=info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame;
  for(unsigned int i=0;i<Components.size();++i){
   Components[i]->Update(dt);
   Components[i]->Draw(*info.Engine,0,0,0,0);
  }
 }
}