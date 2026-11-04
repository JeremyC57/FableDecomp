#ifndef FABLE_ENGINE_NUISYSTEM_CSCROLLABLELIST_H
#define FABLE_ENGINE_NUISYSTEM_CSCROLLABLELIST_H
// NUISystem::CScrollableList (new, chunk 765): RTTI .?AVCScrollableList@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x0124A72C (+0, slot 0 the ??_G 0x005562F0), 0x0124A708 (+4, CObserver) and 0x0124A700 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CScrollableList@NUISystem@@QAE@J@Z, ??0CScrollableList@NUISystem@@QAE@ABV01@@Z, ??1CScrollableList@NUISystem@@UAE@XZ.
// Ego_r.pdb members (struct_layouts_egor.tsv, 428 bytes) TopOfList +396, VisibleListItems +400, PositionOffset +404, ScrollTimer
// +412, PUpArrow +416, PDownArrow +420, UpArrowActive +424, DownArrowActive +425 sit 0x30 lower in retail: the constructor
// 0x00555180 clears +0x16C / +0x170 / +0x174 and the bytes +0x178 / +0x179 (sizeof 0x17C, CRedefinerList's first member +0x17C).
// Overrides are not declared (no row calls them).
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CScrollableList : public CChangingStateComponent {
public:
    CScrollableList(long);
    CScrollableList(const CScrollableList&);
    virtual ~CScrollableList();
protected:
    unsigned long TopOfList;                                        // +0x15c
    unsigned long VisibleListItems;                                 // +0x160
    C2DVector PositionOffset;                                       // +0x164
    float ScrollTimer;                                              // +0x16c
    CComponent* PUpArrow;                                           // +0x170
    CComponent* PDownArrow;                                         // +0x174
    bool UpArrowActive;                                             // +0x178
    bool DownArrowActive;                                           // +0x179
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CScrollableList) == 0x17c);
#endif
