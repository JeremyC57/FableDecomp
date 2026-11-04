#ifndef FABLE_ENGINE_NUISYSTEM_CFRONTENDLIST_H
#define FABLE_ENGINE_NUISYSTEM_CFRONTENDLIST_H
// NUISystem::CFrontEndList (new, chunk 753): RTTI .?AVCFrontEndList@NUISystem@@, single direct base NUISystem::CList (the landed
// ??_G 0x0054DA70 calls ~CList 0x0053B7F0; the ctor 0x0054C3A0 calls CList(long) 0x0053B63E). Vtables 0x01249224 (+0, 190 slots),
// 0x012491FC (+4: slot 1 ProcessEvent 0x0054C420), 0x012491F4 (+0x18). Overrides read from the primary vtable: 0 ??_G 0x0054DA70,
// 1 Update 0x0054C430, 43 Initialise 0x0054D660, 48 ChangeState(unsigned long) 0x0054CBF0, 152 AddScrollingChildAndSetOffsets
// 0x0054D170, 156 InitialiseOffsets 0x0054C480; new protected virtuals 188 ScrollUp() 0x0054C4C0, 189 ScrollDown() 0x0054C810.
// Name set: Ego_r.pdb / FableWin.pdb (??0..QAE@J@Z, ??0..QAE@ABV01@@Z, ??1..UAE@XZ, ??_G, AddScrollingChildAndSetOffsets,
// ChangeState UAEXK@Z, Initialise, InitialiseOffsets UAEXK@Z, ProcessEvent, ScrollDown MAEXXZ, ScrollUp MAEXXZ, Update UAEXM@Z).
// No own members: CManager::CreateComponent 0x0041D21B news 0x1FC bytes, the retail sizeof CList (FABLE_RETAIL_CLIST).
// The destructor is empty and inline: the ??_G stores the three vptrs and calls ~CList.
#include "engine/NUISystem_CList.h"
namespace NUISystem {
class CFrontEndList : public CList {
public:
    CFrontEndList(long);
    CFrontEndList(const CFrontEndList&);
    virtual ~CFrontEndList() { }
    virtual void Initialise();
    virtual void Update(float);
    virtual void ChangeState(unsigned long);
    virtual void ProcessEvent(EEvent);
    virtual void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>&);
    virtual void InitialiseOffsets(unsigned long);
protected:
    virtual void ScrollUp();                                                // slot 188
    virtual void ScrollDown();                                              // slot 189
};
}
#ifdef FABLE_RETAIL_CLIST
FABLE_STATIC_ASSERT(sizeof(NUISystem::CFrontEndList) == 0x1fc);
#endif
#endif
