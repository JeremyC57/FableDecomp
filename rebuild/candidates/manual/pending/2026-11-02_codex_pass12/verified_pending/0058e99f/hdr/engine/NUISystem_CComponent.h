#ifndef FABLE_ENGINE_NUISYSTEM_CCOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CCOMPONENT_H
#include "rebuild_abi.h"
#include "engine/NUISystem_enums.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
class CIEngine;
class CEnginePrimitiveHandle;
class CRGBColour;
class C2DBoxF;
template<class T> class CDefPointer;
namespace std { template<class T> struct less; template<class K,class T,class C,class A> class map; }
namespace NUISystem {
class CComponent;
class CAction;
class CUIDef;
class CUIState;
class CDeletion;
// Layout: original public polymorphic base, native primary size4.
class CUIInterface {
public:
    virtual ~CUIInterface(); // slot 0
    virtual void Update(float); // slot 1
    virtual void Draw(CIEngine &, CEnginePrimitiveHandle *, long, long *, NUISystem::CComponent *); // slot 2
};
// Layout: raw PDB base plus native ctor0052d9e0: size20, set+4, bool+10.
class CObserver {
public:
    CObserver();
    virtual ~CObserver();
    virtual void ProcessEvent(EEvent)=0;
    virtual bool IsObservingEvent(EEvent);
    virtual void ObserveEvent(EEvent);
    virtual void IgnoreEvent(EEvent);
    virtual void IgnoreAllEvents();
    virtual void ObserveAllEvents();
    virtual void SetPreventObserving(bool);
protected:
    unsigned char EventsToObserve[12]; // std::set<EEvent>, native+4.
    bool PreventObservation; // +10
};
// Layout: original second secondary base, size8 in donor and retail.
class CActionParam {
public:
#ifdef FABLE_RETAIL_UI_ACTIONS
    CActionParam() : ActionParamType(UI_ACTION_PARAM_TYPE_NONE) { }
    CActionParam(EActionParamType type) : ActionParamType(type) { }
    virtual ~CActionParam() { }
#else
    CActionParam();
    virtual ~CActionParam();
#endif
    EActionParamType ActionParamType;
};
// Layout: complete native0x134. PDB fields checked against ctor005334a0;
// real bases at+0,+4,+18. Unused composites retain their original storage.
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    CComponent(long);
    virtual ~CComponent();
    virtual void Update(float);
    virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);
    virtual NUISystem::CComponent * FindChild(CCharString); // slot 3
    virtual NUISystem::CComponent * FindChildForward(CCharString); // slot 4
    virtual void FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &); // slot 5
    virtual void FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &); // slot 6
    virtual void SetPAcceptButton(NUISystem::CComponent *); // slot 7
    virtual NUISystem::CComponent * FindChildNonRecursive(CCharString); // slot 8
    virtual NUISystem::CComponent * FindFirstChildOfType(NUISystem::EType); // slot 9
    virtual bool HasChild(NUISystem::CComponent *) const; // slot 10
    virtual void SetZoom(const C2DVector &); // slot 11
    virtual C2DVector GetZoom() const; // slot 12
    virtual void SetColour(const CRGBColour &); // slot 13
    virtual CRGBColour GetColour() const; // slot 14
    virtual void SetPosition(const C2DVector &); // slot 15
    virtual C2DVector GetPosition() const; // slot 16
    virtual C2DVector GetParentPosition() const; // slot 17
    virtual void SetParentPosition(C2DVector &); // slot 18
    virtual C2DVector GetParentZoom() const; // slot 19
    virtual void SetParentZoom(C2DVector &); // slot 20
    virtual CRGBColour GetParentColour() const; // slot 21
    virtual void SetParentColour(CRGBColour &); // slot 22
    virtual C2DVector GetSize() const; // slot 23
    virtual C2DBoxF GetBoundingBox() const; // slot 24
    virtual const C2DVector & GetRenderPosition() const; // slot 25
    virtual const C2DVector & GetRenderZoom() const; // slot 26
    virtual const CRGBColour & GetRenderColour() const; // slot 27
    virtual void SetRenderPosition(C2DVector &); // slot 28
    virtual void SetRenderZoom(C2DVector &); // slot 29
    virtual void SetRenderColour(CRGBColour &); // slot 30
    virtual void ChangePosition(const C2DVector &, float, bool); // slot 31
    virtual void ChangePositionDelta(const C2DVector &, float, bool); // slot 32
    virtual void ChangeTargetPositionDelta(const C2DVector &, float, bool); // slot 33
    virtual void UpdatePosition(float); // slot 34
    virtual void ChangeZoom(const C2DVector &, float, bool); // slot 35
    virtual void ChangeZoomDelta(const C2DVector &, float, bool); // slot 36
    virtual void UpdateZoom(float); // slot 37
    virtual void ChangeColour(const CRGBColour &, float, bool); // slot 38
    virtual void ChangeColourDelta(const CRGBColour &, float, bool); // slot 39
    virtual void UpdateColour(float); // slot 40
    virtual void ResetState(); // slot 41
    virtual void ForceState(unsigned long, bool); // slot 42
    virtual void Initialise(); // slot 43
    virtual bool HasState(unsigned long); // slot 44
    virtual bool HasStateInChildren(unsigned long); // slot 45
    virtual void ChangeState(unsigned long); // slot 48
    virtual void ChangeState(unsigned long, float); // slot 47
    virtual void ChangeState(NUISystem::CUIState &); // slot 46
    virtual bool ChangedState(); // slot 49
    virtual NUISystem::CUIState * GetCurrentState(); // slot 50
    virtual void SetParent(NUISystem::CComponent *); // slot 51
    virtual NUISystem::CComponent * GetParent() const; // slot 52
    virtual std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & GetChildren(); // slot 53
    virtual float GetUpdateTime() const; // slot 54
    virtual C2DVector GetTargetPosition() const; // slot 55
    virtual CRGBColour GetTargetColour() const; // slot 56
    virtual C2DVector GetTargetZoom() const; // slot 57
    virtual void ProcessChangeState(); // slot 58
    virtual void AddChild(CCountedPointer<NUISystem::CComponent> &); // slot 59
    virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &); // slot 60
    virtual void AddChildFront(CCountedPointer<NUISystem::CComponent> &); // slot 61
    virtual void SetDeletion(NUISystem::CDeletion); // slot 62
    virtual NUISystem::CDeletion & GetDeletion(); // slot 63
    virtual void RemoveChild(unsigned long); // slot 64
    virtual NUISystem::EType GetType() const; // slot 65
    virtual void AddAction(CCountedPointer<NUISystem::CAction> &); // slot 66
    virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction> &); // slot 67
    virtual void AddActionOnBack(CCountedPointer<NUISystem::CAction> &); // slot 68
    virtual void AddPreAction(CCountedPointer<NUISystem::CAction> &); // slot 69
    virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction> &); // slot 70
    virtual void AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &); // slot 71
    virtual void AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &); // slot 72
    virtual void AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &); // slot 73
    virtual void AddActionOnDropped(CCountedPointer<NUISystem::CAction> &); // slot 74
    virtual void AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &); // slot 75
    virtual void AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &); // slot 76
    virtual void AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &); // slot 77
    virtual void AddActionOnHovered(CCountedPointer<NUISystem::CAction> &); // slot 78
    virtual void AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &); // slot 79
    virtual void AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &); // slot 80
    virtual void Die(); // slot 81
    virtual std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > & GetStates(); // slot 82
    virtual void SetName(const CCharString &); // slot 83
    virtual CCharString GetName(); // slot 84
    virtual NUISystem::CComponent * FindComponent(NUISystem::CComponent *); // slot 85
    virtual unsigned long GetCurrentStateIndex(); // slot 86
    virtual unsigned long GetTargetStateIndex() const; // slot 87
    virtual float CanScroll(bool, C2DBoxF &); // slot 88
    virtual void FakeChildDying(NUISystem::CComponent *); // slot 89
    virtual void SetParentViewport(NUISystem::CComponent *); // slot 90
    virtual void ClearHandle(); // slot 91
    virtual void MakeUnavailable(); // slot 92
    virtual void MakeUnavailableDynamically(); // slot 93
    virtual void MakeAvailable(); // slot 94
    virtual void MakeAvailableDynamically(); // slot 95
    virtual void AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &); // slot 96
    virtual NUISystem::CComponent * FindFirstChildOfTypeNonRecursive(NUISystem::EType); // slot 97
    virtual bool IsAllowedToScroll() const; // slot 98
    virtual NUISystem::CComponent * GetParentViewport(); // slot 99
    virtual bool IsBastardChild() const; // slot 100
    virtual bool IsIndependent() const; // slot 101
    virtual bool IsPositionIndependent() const; // slot 102
    virtual bool IsZoomIndependent() const; // slot 103
    virtual bool IsLayerIndependent() const; // slot 104
    virtual bool IsDrawFromViewport() const; // slot 105
    virtual bool IsPositionIsCenter() const; // slot 106
    virtual long GetDefIndex() const; // slot 107
    virtual CDefPointer<NUISystem::CUIDef const > LoadAndReturnDef() const; // slot 108
    virtual void OnObserverRemove(); // slot 109
    virtual C2DVector CanMove(const C2DVector &); // slot 110
    virtual C2DVector GetDynamicSize() const; // slot 111
    virtual C2DVector GetTargetRenderPosition() const; // slot 112
    virtual bool IsPCStyle() const; // slot 113
    virtual void SetRelativeParentPosition(C2DVector &); // slot 114
    virtual void SetRelativeParentZoom(C2DVector &); // slot 115
    virtual bool IsUsingRelativeZoom() const; // slot 116
    virtual bool IsUsingRelativePosition() const; // slot 117
    virtual C2DVector GetRelativeRenderPosition(); // slot 118
    virtual C2DVector GetRelativeRenderZoom(); // slot 119
    virtual void SetRelativeRenderPosition(C2DVector); // slot 120
    virtual C2DVector SetRelativeRenderZoom(C2DVector); // slot 121
    virtual C2DVector GetTargetRelativeRenderPosition() const; // slot 122
    virtual C2DVector GetTargetRelativeRenderZoom() const; // slot 123
    virtual void AddPositionChild(NUISystem::CComponent *); // slot 124
    virtual void SetPositionParent(NUISystem::CComponent *); // slot 125
    virtual NUISystem::CComponent * GetPositionParent(); // slot 126
    virtual void PositionChildDying(NUISystem::CComponent *); // slot 127
    virtual void PositionParentDying(NUISystem::CComponent *); // slot 128
    virtual long GetRenderLayer() const; // slot 129
    virtual void AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &); // slot 130
    virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *); // slot 131
    virtual bool IsClickable() const; // slot 132
    virtual void RemoveFromHoveredList(); // slot 133
    virtual void ProcessEvent(EEvent);
protected:
    CCharString Name; // +20
    unsigned char States[12]; // std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > +24
    float Time; // +30
    C2DVector Position; // +34
    C2DVector TargetPosition; // +3c
    C2DVector InitialPosition; // +44
    C2DVector ParentPosition; // +4c
    C2DVector RenderPosition; // +54
    C2DVector Zoom; // +5c
    C2DVector TargetZoom; // +64
    C2DVector InitialZoom; // +6c
    C2DVector ParentZoom; // +74
    C2DVector RenderZoom; // +7c
    CRGBColour Colour; // +84
    CRGBColour TargetColour; // +88
    CRGBColour InitialColour; // +8c
    CRGBColour ParentColour; // +90
    CRGBColour RenderColour; // +94
    float PositionTimeElapsed; // +98
    float PositionTime; // +9c
    float ZoomTimeElapsed; // +a0
    float ZoomTime; // +a4
    float ColourTimeElapsed; // +a8
    float ColourTime; // +ac
    std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > Children; // +b0
    std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > ChildrenToDelete; // +bc
    NUISystem::CComponent * PParent; // +c8
    C2DVector Size; // +cc
    unsigned char Deletion[8]; // NUISystem::CDeletion +d4
    NUISystem::CComponent * PParentViewport; // +dc
    unsigned char ActionsIAmParamOf[4]; // std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > +e0
    std::vector<unsigned long,std::allocator<unsigned long> > ShapeChildren; // +e4
    C2DVector RenderPositionSave; // +f0
    C2DVector RelativeRenderPosition; // +f8
    C2DVector RelativeParentPosition; // +100
    C2DVector RelativeRenderZoom; // +108
    C2DVector RelativeParentZoom; // +110
    NUISystem::CComponent * PPositionParent; // +118
    unsigned char PositionChildren[12]; // std::set<NUISystem::CComponent *,std::less<NUISystem::CComponent *>,std::allocator<NUISystem::CComponent *> > +11c
    std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnDestruction; // +128
    unsigned char Flags; // +12c
    unsigned char Type; // +12d
    unsigned char FlagsSetTwo; // +12e
    char Layer; // +12f
    long DefIndex; // +130
};
FABLE_STATIC_ASSERT(sizeof(CUIInterface)==4);
FABLE_STATIC_ASSERT(sizeof(CObserver)==20);
FABLE_STATIC_ASSERT(sizeof(CActionParam)==8);
FABLE_STATIC_ASSERT(sizeof(CComponent)==0x134);
}
#endif
