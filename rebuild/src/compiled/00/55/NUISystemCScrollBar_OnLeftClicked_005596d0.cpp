// NUISystem::CScrollBar::OnLeftClicked() -- at the start of a drag remember where the bar was and clear the delta accumulated since the last action, then the draggable left-click handling
// Leaf: ?OnLeftClicked@CScrollBar@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005596D0, 47 bytes, thiscall, tail `jmp` to the base override.
// Identity (proven): the only reference is slot 146 of the NUISystem::CScrollBar vtable 0x0124B7E4 (0x0124BA2C); slot 146 of
//   the NUISystem::CDraggable vtable 0x0124BA94 is the landed ?OnLeftClicked@CDraggable@NUISystem@@UAEXXZ 0x0055A5D0, the tail
//   target here (qualified base call). Ego_r.pdb name set ?OnLeftClicked@CScrollBar@NUISystem@@UAEXXZ (donor 0x005D2900, 0x30
//   bytes, between SetMaxAmountActionsDown and GetDataFromDef); the members it touches are CScrollBar / CDraggable's own.
//   The catalog label "NUISystem::CScrollBar::vfunc_146" is a placeholder.
// Callee: NUISystem::CDraggable::OnLeftClicked 0x0055A5D0 (landed).
// Layout (retail = Ego_r.pdb - 0x30 past the CComponent containers; as the landed CDraggable::OnLeftClicked 0x0055A5D0 and
//   CScrollBar ctor 0x00559830): CComponent::Position +0x34 (Ego_r +0x3C), CDraggable::PositionBeforeDrag +0x194 (+0x1C4),
//   CDraggable::WasDragged +0x19C (+0x1CC), CScrollBar::DeltaBeforeAction +0x1B4 (+0x1E4).
// Shape: one `xor eax,eax` serves the WasDragged test and both zero stores; the delta is cleared member by member (a
//   `= C2DVector(0.0f, 0.0f)` temporary goes through an 8-byte stack frame under VC7.1).
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

namespace NUISystem {

class CComponent {
public:
    virtual ~CComponent();
    unsigned char _pad004[0x34 - 4];
    C2DVector     Position;                                 // +0x34
    C2DVector     TargetPosition;                           // +0x3C
};

class CChangingStateComponent : public CComponent {
public:
};

class CHoverable : public CChangingStateComponent {
public:
};

class CClickable : public CHoverable {
public:
    virtual void OnLeftClicked();                           // 0x0055AF60
};

class CDraggable : public CClickable {
public:
    unsigned char _pad044[0x194 - 0x44];
    C2DVector     PositionBeforeDrag;                       // +0x194
    bool          WasDragged;                               // +0x19C
    unsigned char _pad19d[0x1ac - 0x19d];
    virtual void OnLeftClicked();                           // 0x0055A5D0
};

class CActionList;                                          // std::list<CCountedPointer<NUISystem::CAction> >

class CScrollBar : public CDraggable {                      // vtable 0x0124B7E4
public:
    CActionList*  PActionsOnDraggedUp;                      // +0x1AC
    CActionList*  PActionsOnDraggedDown;                    // +0x1B0
    C2DVector     DeltaBeforeAction;                        // +0x1B4
    C2DVector     DeltaForActionUp;                         // +0x1BC
    C2DVector     DeltaForActionDown;                       // +0x1C4
    virtual void OnLeftClicked();
};

void CScrollBar::OnLeftClicked()
{
    if (!WasDragged) {
        PositionBeforeDrag = Position;
        DeltaBeforeAction.X = 0.0f;
        DeltaBeforeAction.Y = 0.0f;
    }
    CDraggable::OnLeftClicked();
}

}   // namespace NUISystem