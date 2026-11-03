#ifndef FABLE_ENGINE_NUISYSTEM_CSCROLLBAR_H
#define FABLE_ENGINE_NUISYSTEM_CSCROLLBAR_H
// NUISystem::CDraggable and NUISystem::CScrollBar (new, chunk 726). Ego_r.pdb layouts (struct_layouts_egor.tsv):
// CDraggable 476 bytes: PositionBeforeDrag +452, WasDragged +460, PActionsOnDropped +464, PActionsOnDroppedNowhere +468,
// SnapBackIfDroppedNowhere +472, ChangeLayer +473; CScrollBar 524 bytes: PActionsOnDraggedUp +476, PActionsOnDraggedDown
// +480, DeltaBeforeAction +484, DeltaForActionUp +492, DeltaForActionDown +500, AmountActionsUp +508, AmountActionsDown
// +512, MaxAmountActionsUp +516, MaxAmountActionsDown +520. Retail is the PDB minus 0x30 (the 0x194-byte retail CClickable):
// IsDragged 0x0055A330 reads +0x19C, SetPositionBeforeDrag 0x0055A340 stores +0x194, SetSnapBackIfDroppedNowhere 0x0055A360
// +0x1A8, SetDeltaForActionUp / Down 0x00559650 / 0x00559670 +0x1BC / +0x1C4, SetAmountActionsUp / Down 0x00559690 /
// 0x005596A0 +0x1CC / +0x1D0, SetMaxAmountActionsUp / Down 0x005596B0 / 0x005596C0 +0x1D4 / +0x1D8.
// New virtuals in CScrollBar vtable 0x0124B7E4 slot order: CDraggable IsDragged 150, SetPositionBeforeDrag 151,
// SetSnapBackIfDroppedNowhere 152, UpdateMouseCursor 153 (protected); CScrollBar SetDeltaForActionUp 154,
// SetDeltaForActionDown 155, SetAmountActionsUp 156, SetAmountActionsDown 157, SetMaxAmountActionsUp 158,
// SetMaxAmountActionsDown 159 (name sets: Ego_r / FableWin ?IsDragged@CDraggable@NUISystem@@UAE_NXZ ...
// ?SetMaxAmountActionsDown@CScrollBar@NUISystem@@UAEXK@Z).
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CDraggable : public CClickable {
public:
    CDraggable(long);
    CDraggable(const CDraggable&);
    virtual ~CDraggable();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ProcessEvent(EEvent);
    virtual void ChangePosition(const C2DVector&,float,bool);
    virtual void OnLeftClicked();
    virtual void OnLeftUnclicked();
    virtual bool IsDragged();                                       // slot 150
    virtual void SetPositionBeforeDrag(C2DVector&);                 // slot 151
    virtual void SetSnapBackIfDroppedNowhere(bool);                 // slot 152
protected:
    virtual void OnHovered();
    virtual void OnUnhovered();
    virtual void UpdateMouseCursor();                               // slot 153
    C2DVector PositionBeforeDrag;                                   // +0x194
    bool WasDragged;                                                // +0x19c
    std::list<CCountedPointer<CAction> >* PActionsOnDropped;        // +0x1a0
    std::list<CCountedPointer<CAction> >* PActionsOnDroppedNowhere; // +0x1a4
    bool SnapBackIfDroppedNowhere;                                  // +0x1a8
    bool ChangeLayer;                                               // +0x1a9
};
class CScrollBar : public CDraggable {
public:
    CScrollBar(long);
    CScrollBar(const CScrollBar&);
    virtual ~CScrollBar();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ProcessEvent(EEvent);
    virtual void OnLeftClicked();
    virtual void SetDeltaForActionUp(C2DVector&);                   // slot 154
    virtual void SetDeltaForActionDown(C2DVector&);                 // slot 155
    virtual void SetAmountActionsUp(unsigned long);                 // slot 156
    virtual void SetAmountActionsDown(unsigned long);               // slot 157
    virtual void SetMaxAmountActionsUp(unsigned long);              // slot 158
    virtual void SetMaxAmountActionsDown(unsigned long);            // slot 159
protected:
    std::list<CCountedPointer<CAction> >* PActionsOnDraggedUp;      // +0x1ac
    std::list<CCountedPointer<CAction> >* PActionsOnDraggedDown;    // +0x1b0
    C2DVector DeltaBeforeAction;                                    // +0x1b4
    C2DVector DeltaForActionUp;                                     // +0x1bc
    C2DVector DeltaForActionDown;                                   // +0x1c4
    unsigned long AmountActionsUp;                                  // +0x1cc
    unsigned long AmountActionsDown;                                // +0x1d0
    unsigned long MaxAmountActionsUp;                               // +0x1d4
    unsigned long MaxAmountActionsDown;                             // +0x1d8
};
FABLE_STATIC_ASSERT(sizeof(CDraggable)==0x1ac);
FABLE_STATIC_ASSERT(sizeof(CScrollBar)==0x1dc);
}
#endif
