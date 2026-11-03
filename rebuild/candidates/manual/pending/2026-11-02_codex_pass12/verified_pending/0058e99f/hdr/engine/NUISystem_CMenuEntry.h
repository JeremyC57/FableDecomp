#ifdef FABLE_RETAIL_CMENUENTRY
#ifndef FABLE_RETAIL_CMENUENTRY_DEFINED
#define FABLE_RETAIL_CMENUENTRY_DEFINED
// Retail opt-in (Codex pass 12): original CClickable base, original complete fields; native base size0x194 and OnLeftUnclicked slot0x24c.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CMenuEntry : public CClickable {
public:
 CMenuEntry(long);
 CMenuEntry(const CMenuEntry&);
 virtual ~CMenuEntry();
 void GetDataFromDef();
 virtual void Initialise();
 virtual void ProcessEvent(NUISystem::EEvent);
 virtual void ChangeState(unsigned long);
 virtual void Update(float);
 virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &);
 virtual void AddChild(CCountedPointer<NUISystem::CComponent> &);
 virtual void RemoveChild(unsigned long);
 virtual void AddThingActionParam(long);
 virtual void AddUIActionParam(NUISystem::CActionParam *);
 virtual void MakeUnavailable();
 virtual void MakeUnavailableDynamically();
 virtual void MakeAvailable();
 virtual void MakeAvailableDynamically();
 bool IsMenuEntryAvailable() const;
 virtual void AddAction(CCountedPointer<NUISystem::CAction> &);
 virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction> &);
 virtual void AddActionOnUnselected(CCountedPointer<NUISystem::CAction> &);
 virtual void AddActionOnBack(CCountedPointer<NUISystem::CAction> &);
 virtual void AddPreAction(CCountedPointer<NUISystem::CAction> &);
 virtual void AddActionOnVisible(CCountedPointer<NUISystem::CAction> &);
 virtual bool IsClickable() const;
 virtual void OnLeftClicked();
 virtual void OnLeftUnclicked();
 virtual void OnRightClicked();
 virtual void OnRightUnclicked();
 virtual std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * GetActions();
 static void __fastcall GSetForceProcessing(bool);
protected:
 virtual void OnHovered();
 virtual void OnUnhovered();
 virtual void OnObserverRemove();
 virtual void GoForward();
 virtual void GoBackwards();
 float LastInputTime;
 bool IsAvailable;
 std::list<NUISystem::CActionParam *,std::allocator<NUISystem::CActionParam *> > * PUIActionParams;
 std::list<long,std::allocator<long> > * PThingActionParams;
 std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActions;
 std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnSelected;
 std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnBack;
 std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PPreActions;
 std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnUnselected;
 std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnVisible;
 static double GLastInputTime;
 static bool GForceProcessing;
public:
 NUISystem::CMenuEntry & operator=(const NUISystem::CMenuEntry &);
};
FABLE_STATIC_ASSERT(sizeof(CMenuEntry)==0x1bc);
}
#endif
#else

#endif
