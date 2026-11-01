// 0053dcd0 NUISystem::CMenuEntry::AddThingActionParam
// Leaf: ?AddThingActionParam@CMenuEntry@NUISystem@@UAEXJ@Z
// Flags:
#include "engine/CCountedPointer.h"
#define CComponent CComponentFields
#include "engine/_quarantine/CComponent.h"
#undef CComponent
class CCharString;class CWideString;
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

namespace std {
template <class T> class allocator {
public:
    allocator() throw() {}
    allocator(const allocator&)throw(){}
    template<class U>allocator(const allocator<U>&)throw(){}
    ~allocator()throw(){}
    T* allocate(size_t n,const void* = 0){return n?(T*)malloc(n*sizeof(T)):0;}
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}_List_iterator_base(){}};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{typedef _List_iterator<T,_Nonconst_traits<T> >iterator;_List_iterator(_List_node<T>*n):_List_iterator_base(n){}_List_iterator(){}_List_iterator(const iterator&x):_List_iterator_base(x._M_node){}};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T,class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const A&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class T,class A=allocator<T> > class list : public _List_base<T,A> {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T,_Nonconst_traits<T> > iterator;
    list(const A& a = A()) : _List_base<T,A>(a) {}
    iterator end() { return this->_M_node._M_data; }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = this->_M_node.allocate(1);
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};
}

class C2DBoxI;class C2DBoxF;class C2DVector;class CEnginePrimitiveHandle;class CIEngine;class CRGBColour;class CFontBank;
template<class T>class CDefPointer;
namespace std{template<class T>class allocator;template<class T>struct less;template<class A,class B>struct pair;template<class T,class A>class list;template<class T,class A>class vector;template<class K,class V,class C,class A>class map;}
namespace NUISystem{class CComponent;class CAction;class CDeletion;class CUIState;class CUIDef;enum ETextAlignement{};enum EType{};
class CUIInterface{
public:virtual ~CUIInterface();
public:virtual void Update(float);
public:virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long *,NUISystem::CComponent*);
};
class CComponent : public CUIInterface{
public:virtual NUISystem::CComponent* FindChild(CCharString);
public:virtual NUISystem::CComponent* FindChildForward(CCharString);
public:virtual void FindAllChildren(CCharString,std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >&);
public:virtual void FindAllChildrenOfType(NUISystem::EType,std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >&);
public:virtual void SetPAcceptButton(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* FindChildNonRecursive(CCharString);
public:virtual NUISystem::CComponent* FindFirstChildOfType(NUISystem::EType);
public:virtual bool HasChild(NUISystem::CComponent*) const;
public:virtual void SetZoom(const C2DVector&);
public:virtual C2DVector GetZoom() const;
public:virtual void SetColour(const CRGBColour&);
public:virtual CRGBColour GetColour() const;
public:virtual void SetPosition(const C2DVector&);
public:virtual C2DVector GetPosition() const;
public:virtual C2DVector GetParentPosition() const;
public:virtual void SetParentPosition(C2DVector&);
public:virtual C2DVector GetParentZoom() const;
public:virtual void SetParentZoom(C2DVector&);
public:virtual CRGBColour GetParentColour() const;
public:virtual void SetParentColour(CRGBColour&);
public:virtual C2DVector GetSize() const;
public:virtual C2DBoxF GetBoundingBox() const;
public:virtual const C2DVector& GetRenderPosition() const;
public:virtual const C2DVector& GetRenderZoom() const;
public:virtual const CRGBColour& GetRenderColour() const;
public:virtual void SetRenderPosition(C2DVector&);
public:virtual void SetRenderZoom(C2DVector&);
public:virtual void SetRenderColour(CRGBColour&);
public:virtual void ChangePosition(const C2DVector&,float,bool);
public:virtual void ChangePositionDelta(const C2DVector&,float,bool);
public:virtual void ChangeTargetPositionDelta(const C2DVector&,float,bool);
public:virtual void UpdatePosition(float);
public:virtual void ChangeZoom(const C2DVector&,float,bool);
public:virtual void ChangeZoomDelta(const C2DVector&,float,bool);
public:virtual void UpdateZoom(float);
public:virtual void ChangeColour(const CRGBColour&,float,bool);
public:virtual void ChangeColourDelta(const CRGBColour&,float,bool);
public:virtual void UpdateColour(float);
public:virtual void ResetState();
public:virtual void ForceState(unsigned long,bool);
public:virtual void Initialise();
public:virtual bool HasState(unsigned long);
public:virtual bool HasStateInChildren(unsigned long);
public:virtual void ChangeState(unsigned long);
public:virtual void ChangeState(unsigned long,float);
public:virtual void ChangeState(NUISystem::CUIState&);
public:virtual bool ChangedState();
public:virtual NUISystem::CUIState* GetCurrentState();
public:virtual void SetParent(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* GetParent() const;
public:virtual std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > >& GetChildren();
public:virtual float GetUpdateTime() const;
public:virtual C2DVector GetTargetPosition() const;
public:virtual CRGBColour GetTargetColour() const;
public:virtual C2DVector GetTargetZoom() const;
public:virtual void ProcessChangeState();
public:virtual void AddChild(CCountedPointer<NUISystem::CComponent>&);
public:virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent>&);
public:virtual void AddChildFront(CCountedPointer<NUISystem::CComponent>&);
public:virtual void SetDeletion(NUISystem::CDeletion);
public:virtual NUISystem::CDeletion& GetDeletion();
public:virtual void RemoveChild(unsigned long);
public:virtual NUISystem::EType GetType() const;
public:virtual void AddAction(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnBack(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddPreAction(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnRightClicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDropped(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnHovered(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnUnhovered(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void Die();
public:virtual std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > >& GetStates();
public:virtual void SetName(const CCharString&);
public:virtual CCharString GetName();
public:virtual NUISystem::CComponent* FindComponent(NUISystem::CComponent*);
public:virtual unsigned long GetCurrentStateIndex();
public:virtual unsigned long GetTargetStateIndex() const;
public:virtual float CanScroll(bool,C2DBoxF&);
public:virtual void FakeChildDying(NUISystem::CComponent*);
public:virtual void SetParentViewport(NUISystem::CComponent*);
public:virtual void ClearHandle();
public:virtual void MakeUnavailable();
public:virtual void MakeUnavailableDynamically();
public:virtual void MakeAvailable();
public:virtual void MakeAvailableDynamically();
public:virtual void AddActionIAmParamOf(CCountedPointer<NUISystem::CAction>&);
public:virtual NUISystem::CComponent* FindFirstChildOfTypeNonRecursive(NUISystem::EType);
public:virtual bool IsAllowedToScroll() const;
public:virtual NUISystem::CComponent* GetParentViewport();
public:virtual bool IsBastardChild() const;
public:virtual bool IsIndependent() const;
public:virtual bool IsPositionIndependent() const;
public:virtual bool IsZoomIndependent() const;
public:virtual bool IsLayerIndependent() const;
public:virtual bool IsDrawFromViewport() const;
public:virtual bool IsPositionIsCenter() const;
public:virtual long GetDefIndex() const;
public:virtual CDefPointer<NUISystem::CUIDef const > LoadAndReturnDef() const;
public:virtual void OnObserverRemove();
public:virtual C2DVector CanMove(const C2DVector&);
public:virtual C2DVector GetDynamicSize() const;
public:virtual C2DVector GetTargetRenderPosition() const;
public:virtual bool IsPCStyle() const;
public:virtual void SetRelativeParentPosition(C2DVector&);
public:virtual void SetRelativeParentZoom(C2DVector&);
public:virtual bool IsUsingRelativeZoom() const;
public:virtual bool IsUsingRelativePosition() const;
public:virtual C2DVector GetRelativeRenderPosition();
public:virtual C2DVector GetRelativeRenderZoom();
public:virtual void SetRelativeRenderPosition(C2DVector);
public:virtual C2DVector SetRelativeRenderZoom(C2DVector);
public:virtual C2DVector GetTargetRelativeRenderPosition() const;
public:virtual C2DVector GetTargetRelativeRenderZoom() const;
public:virtual void AddPositionChild(NUISystem::CComponent*);
public:virtual void SetPositionParent(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* GetPositionParent();
public:virtual void PositionChildDying(NUISystem::CComponent*);
public:virtual void PositionParentDying(NUISystem::CComponent*);
public:virtual long GetRenderLayer() const;
public:virtual void AddActionOnDesctruction(CCountedPointer<NUISystem::CAction>&);
public:virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >*);
public:virtual bool IsClickable() const;
public:virtual void RemoveFromHoveredList();
private:unsigned char unreadComponentMembers[sizeof(CComponentFields)-4];
};
class CChangingStateComponent : public CComponent{
public:virtual NUISystem::CUIState* GetState(unsigned long);
public:virtual const NUISystem::CUIState* GetState(unsigned long) const;
public:virtual void UpdateStateChange();
public:virtual bool ChangedStateLastUpdate();
public:virtual bool InternalChanged();
protected:virtual bool AllChildrenChangedState();
protected:virtual void OnActivate();
protected:virtual void OnDeactivate();
protected:unsigned char _pad_changing[40];
};
// Layout: CComponent shared134, CChangingStateComponent40-byte tail gives15c. Original hover/click members follow donor with list shrink48.
class CHoverable:public CChangingStateComponent{protected:virtual bool IsHovered(C2DBoxI&)const;virtual void OnHovered();virtual void OnUnhovered();virtual void UpdateHovering();long StateBeforeHovered;bool WasHovered;std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >*PActionsOnHovered;std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >*PActionsOnUnhovered;};
class CClickable:public CHoverable{public:virtual void OnLeftClicked();virtual void OnLeftUnclicked();virtual void OnRightClicked();virtual void OnRightUnclicked();protected:long StateBeforeLeftClicked;bool WasLeftClicked;std::list<CCountedPointer<CAction> >*PActionsOnLeftClicked;std::list<CCountedPointer<CAction> >*PActionsOnLeftHeld;std::list<CCountedPointer<CAction> >*PActionsOnLeftUnclicked;long StateBeforeRightClicked;bool WasRightClicked;std::list<CCountedPointer<CAction> >*PActionsOnRightClicked;float InputDelay;float LastInputTime;};
class CActionParam;
// Layout: original CMenuEntry extends CClickable194; LastInputTime194, IsAvailable198, list pointers19c/1a0.
class CMenuEntry:public CClickable{protected:float LastInputTime;bool IsAvailable;std::list<CActionParam*>*PUIActionParams;std::list<long>*PThingActionParams;public:virtual void AddThingActionParam(long);};
void CMenuEntry::AddThingActionParam(long param){if(!PThingActionParams)PThingActionParams=new std::list<long>();PThingActionParams->push_back(param);}
}