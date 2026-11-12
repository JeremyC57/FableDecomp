#ifndef FABLE_ENGINE_NUISYSTEM_CSCROLLBAROUTSIDE_H
#define FABLE_ENGINE_NUISYSTEM_CSCROLLBAROUTSIDE_H
// NUISystem::CScrollBarOutside (new, chunk 765): RTTI .?AVCScrollBarOutside@NUISystem@@, single direct base NUISystem::CClickable at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x0124B54C (+0, slot 0 the ??_G 0x005595C0), 0x0124B528 (+4, CObserver) and 0x0124B520 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CScrollBarOutside@NUISystem@@QAE@J@Z, ??0CScrollBarOutside@NUISystem@@QAE@ABV01@@Z, ??1CScrollBarOutside@NUISystem@@UAE@XZ.
// Ego_r.pdb members (464 bytes) PScrollBar +452, PActionsOnLeftClickedAbove +456, PActionsOnLeftClickedUnder +460 sit 0x30 lower
// in retail (on the 0x194-byte retail CClickable): the constructor 0x00559360 clears +0x194..+0x19C; sizeof 0x1A0.
// Overrides are not declared (no row calls them).
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CScrollBarOutside : public CClickable {
public:
    CScrollBarOutside(long);
    CScrollBarOutside(const CScrollBarOutside&);
    virtual ~CScrollBarOutside();
    void GetDataFromDef();                                          // 0x00559230
#ifdef FABLE_RETAIL_CSCROLLBAROUTSIDE_ONLEFTCLICKED
    // Retail opt-in (wf2, row 0x005590C0): Ego_r.pdb / FableWin.pdb ?OnLeftClicked@CScrollBarOutside@NUISystem@@UAEXXZ, the override in
    // slot 146 of the primary vtable 0x0124B54C (rtti-vtables.tsv). Declaration only: no layout or vtable change.
    virtual void OnLeftClicked();                                   // 0x005590C0
#endif
protected:
    CComponent* PScrollBar;                                         // +0x194
    std::list<CCountedPointer<CAction> >* PActionsOnLeftClickedAbove; // +0x198
    std::list<CCountedPointer<CAction> >* PActionsOnLeftClickedUnder; // +0x19c
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CScrollBarOutside) == 0x1a0);
#endif
