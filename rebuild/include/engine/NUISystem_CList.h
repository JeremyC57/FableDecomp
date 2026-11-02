#ifndef FABLE_ENGINE_NUISYSTEM_CLIST_H
#define FABLE_ENGINE_NUISYSTEM_CLIST_H
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CUIState.h"
#include "stlport/fable_map.h"
namespace NUISystem {
class CListState;
// Complete original class; native base15c, three vectors12 and bit-vector16.
class CList : public CChangingStateComponent {
public: CList(long); virtual ~CList();
public: virtual void Initialise();
public: virtual void Update(float);
public: virtual void ChangeState(unsigned long);
public: virtual void ProcessEvent(NUISystem::EEvent);
public: virtual long GetSelectedChild();
public: virtual void RemoveChild(unsigned long);
public: virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &);
public: virtual void Die();
public: virtual void AddScrollingChild(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddNonScrollingChild(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddNonScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &);
public: virtual C2DVector GetPositionOffset();
public: virtual float GetAlphaOffset();
public: virtual void SetLastInputTimeToBeNow();
public: virtual void SetNextScrollingChildAlpha(float);
public: virtual void SetNextScrollingChildPosition(C2DVector &);
public: virtual void AddScrollingChildAndSetOffsets(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddScrollingChildDynamicallyAndSetOffsets(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction> &);
public: virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction> &);
public: virtual void SetActionOnScrollUp(CCountedPointer<NUISystem::CAction> &);
public: virtual void SetActionOnScrollDown(CCountedPointer<NUISystem::CAction> &);
public: virtual void InitialiseOffsets(unsigned long);
public: virtual void SetDefaultChild(long);
public: virtual void SetDefaultChild(NUISystem::CComponent *);
public: virtual NUISystem::CListState SaveState();
public: virtual void RestoreState(NUISystem::CListState &);
public: virtual void SetSelectedChild(long);
public: virtual bool AllScrollingChildrenDeleting();
public: virtual std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & GetScrollingChildren();
public: virtual std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & GetNonScrollingChildren();
public: virtual void ScrollUp(bool,bool);
protected: virtual void ScrollUp(unsigned long,unsigned long,bool,bool,bool);
public: virtual void ScrollDown(bool,bool);
protected: virtual void ScrollDown(unsigned long,unsigned long,bool,bool,bool);
public: virtual bool AreThereAnyDeletions();
public: virtual bool IsWrapping() const;
public: virtual bool IsScrolling() const;
public: virtual bool IsInverted() const;
public: virtual void ScrollUntilChild(NUISystem::CComponent *);
public: virtual void ScrollUntilChildContaining(NUISystem::CComponent *);
public: virtual C2DVector CanMove(const C2DVector &);
public: virtual void ChangePosition(const C2DVector &,float,bool);
public: virtual NUISystem::CComponent * GetScrollBar();
public: virtual NUISystem::CComponent * GetScrollBarOutside();
public: virtual void SetInitialPositionDummy(NUISystem::CComponent *);
public: virtual void ScrollToFirstChild();
public: virtual void ScrollToLastChild();
protected: virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *);
protected: virtual void SetNonScrollingChildrenStates();
protected: virtual void SetRecomputeOffsets(bool);
protected: virtual void ScrollUpProcess();
protected: virtual void ScrollDownProcess();
protected: virtual void OnActivate();
protected: virtual void DoRecomputeOffsets();
protected: virtual void ComputeChildrenVisibility();
protected: virtual void ComputeScrollBarPosition();
protected: virtual void ComputeScrollBarZoom();
protected: long SelectedChild;
protected: float LastInputTime;
protected: std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > ScrollingChildren;
protected: std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > NonScrollingChildren;
protected: std::vector<std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > >,std::allocator<std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > > > ScrollingChildrenStates;
protected: C2DVector PositionOffset;
protected: C2DVector PositionOffsetToUse;
protected: float AlphaOffset;
protected: float AlphaOffsetToUse;
protected: float NextScrollingChildAlpha;
protected: C2DVector NextScrollingChildPosition;
protected: std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnInactive;
protected: std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnSelected;
protected: CCountedPointer<NUISystem::CAction> CPActionOnScrollUp;
protected: CCountedPointer<NUISystem::CAction> CPActionOnScrollDown;
protected: long DefaultChild;
protected: unsigned char ScrollingChildrenVisibility[16]; // actual STLport bit-vector, unused by this row.
protected: NUISystem::CComponent * PInitialPositionDummy;
protected: NUISystem::CComponent * PScrollBar;
protected: NUISystem::CComponent * PScrollBarOutside;
protected: NUISystem::CComponent * PListUp;
protected: NUISystem::CComponent * PListDown;
protected: bool RecomputeOffsets;
protected: float InputDelay;
protected: C2DVector PDefPositionOffset;
protected: unsigned char PDefAlphaOffset;
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CList)==0x200);
#endif
