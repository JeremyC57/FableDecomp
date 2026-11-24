#ifndef FABLE_ENGINE_NUISYSTEM_CLIST_H
#define FABLE_ENGINE_NUISYSTEM_CLIST_H
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CUIState.h"
#include "stlport/fable_map.h"
namespace NUISystem {
#ifdef FABLE_RETAIL_QUICKACCESS_MAGIC_TEST_PASS27
struct CListState;
#else
class CListState;
#endif
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
#ifdef FABLE_RETAIL_CLIST_GETDATAFROMDEF
// NF66 opt-in (row 0x0053822B): Ego_r.pdb 0x005B4112 / FableWin.pdb 0x02193548 ?GetDataFromDef@CList@NUISystem@@QAEXXZ (public,
// non-virtual; out of line at 0x0053822B). Declaration only.
public: void GetDataFromDef();
#endif
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
#ifdef FABLE_RETAIL_CLIST
// Retail branch (chunk 726): the visibility flags are a 12-byte STLport vector<bool> with one byte per element (the
// tree's vector<bool> is the primary template): ComputeChildrenVisibility 0x005379F5 reads begin +0x1C8 / end +0x1CC
// and indexes bytes; every later member sits 4 below the default model: GetScrollBar 0x0053CE5F reads +0x1D8,
// SetInitialPositionDummy 0x0053CE6D stores +0x1D4, SetRecomputeOffsets 0x0053CE7A +0x1E8, and the factory at
// 0x0041D4AD allocates `push 1FCh` for the constructor 0x0053B63E.
protected: std::vector<bool,std::allocator<bool> > ScrollingChildrenVisibility;
// The static list of previously selected lists, 0x013B8A98: Ego_r.pdb names the dynamic initializer of
// NUISystem::CList::GPreviousSelected and FableWin.pdb the accessor ?GetPreviousSelected@CList@NUISystem@@SIAAV?$list@PAVCList@NUISystem@@...
// (the type); ChangeState 0x0053A0A1 walks it, reads its front and calls push_front 0x0053CA3C. Access is unproven
// (the landed 0x005E41E0 clears it from outside the class), hence public.
#ifdef FABLE_RETAIL_PASS22_SLIDER_PROCESS_EVENT
// Both raw PDBs: protected static storage and public original fastcall reference getter.
protected: static std::list<NUISystem::CList *,std::allocator<NUISystem::CList *> > GPreviousSelected;
public: static std::list<NUISystem::CList *,std::allocator<NUISystem::CList *> >& FABLE_FASTCALL GetPreviousSelected(){return GPreviousSelected;}
#else
public: static std::list<NUISystem::CList *,std::allocator<NUISystem::CList *> > GPreviousSelected;
#endif
#ifdef FABLE_RETAIL_CLIST_CLEARPREVIOUSSELECTED
// Opt-in (agentFAM07): FableWin.pdb ?ClearPreviousSelected@CList@NUISystem@@SIXXZ (public static __fastcall), retail 0x0048573D
// (`mov ecx, offset GPreviousSelected ; jmp _List_base::clear`, called by CTCQuestCompletionUI::GlobalUpdate 0x005E78F0).
public: static void __fastcall ClearPreviousSelected();
#endif
#else
protected: unsigned char ScrollingChildrenVisibility[16]; // actual STLport bit-vector, unused by this row.
#endif
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
#ifdef FABLE_RETAIL_CLIST
FABLE_STATIC_ASSERT_AT(118, sizeof(NUISystem::CList)==0x1fc);
#else
FABLE_STATIC_ASSERT_AT(120, sizeof(NUISystem::CList)==0x200);
#endif
#endif
