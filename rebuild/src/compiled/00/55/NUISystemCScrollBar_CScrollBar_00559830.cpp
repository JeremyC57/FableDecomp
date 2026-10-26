// NUISystem::CScrollBar::CScrollBar(long owner) -- CDraggable(owner), the three vtables of this class, the two dragged-action list pointers and the four action counters cleared, then GetDataFromDef()
// Retail: Fable.exe (TLC, VC7.1) @ 0x00559830, 84 bytes.
// Callees: NUISystem::CDraggable::CDraggable(long) @ 0x0055A9C0 (landed; manifest "CClickable" label is
// propagated), GetDataFromDef @ 0x00559700 (on `this`, in the body; manifest "CScrollBar::Initialize").
// Vtables 0x0124B7E4 / 0x0124B7BC / 0x0124B7B4 = NUISystem::CScrollBar (rtti-vtables.tsv).
//
// Shape: base constructor, `xor eax,eax`, the three vtable stores (primary at +0, the CObserver base at
// +4, the third interface at +0x18), then the zero stores from the shared register in declaration order
// -- the NUISystem module schedules init-list members after the vtable stores (as the landed CDraggable
// 0x0055A9C0 and CParticleEffect 0x00544010) -- and the body call. The three C2DVector deltas between the
// list pointers and the counters (+0x1B4..+0x1CB) have no initialiser (plain POD members).
// Retail layout = Ego_r.pdb minus 0x30 (the retail CHoverable is 0x16C bytes, the PDB's 0x19C): CScrollBar
// PDB 0x20C -> retail 0x1DC; members PActionsOnDraggedUp +0x1AC .. MaxAmountActionsDown +0x1D8. No engine
// header exists for the NUISystem chain at retail offsets, so the bases are modelled as in the landed
// CNavButton constructor (0x00558B90).
// Leaf: ??0CScrollBar

namespace NUISystem {

struct CComponentInterface {                     // primary vtable at +0
    virtual void v000();
};

struct CObserver_Retail {                        // vtable at +4, then its 0x10 bytes of state
    virtual void ProcessEvent();
    unsigned char _body[0x10];
};

struct CEventSinkInterface {                     // vtable at +0x18
    virtual void v0();
};

struct CActionList;                              // std::list<CCountedPointer<NUISystem::CAction>>

struct C2DVector {
    float X, Y;
};

struct CDraggable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x1AC bytes
    unsigned char _body[0x1ac - 0x1c];
    CDraggable_Retail(long owner);               // 0x0055A9C0
};

class CScrollBar : public CDraggable_Retail { public:          // vtables 0x0124B7E4 / 0x0124B7BC / 0x0124B7B4
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v0();
    CActionList*  PActionsOnDraggedUp;           // +0x1AC
    CActionList*  PActionsOnDraggedDown;         // +0x1B0
    C2DVector     DeltaBeforeAction;             // +0x1B4
    C2DVector     DeltaForActionUp;              // +0x1BC
    C2DVector     DeltaForActionDown;            // +0x1C4
    unsigned long AmountActionsUp;               // +0x1CC
    unsigned long AmountActionsDown;             // +0x1D0
    unsigned long MaxAmountActionsUp;            // +0x1D4
    unsigned long MaxAmountActionsDown;          // +0x1D8
    CScrollBar(long owner);
    void GetDataFromDef();                       // 0x00559700
};

CScrollBar::CScrollBar(long owner)
    : CDraggable_Retail(owner)
    , PActionsOnDraggedUp(0)
    , PActionsOnDraggedDown(0)
    , AmountActionsUp(0)
    , AmountActionsDown(0)
    , MaxAmountActionsUp(0)
    , MaxAmountActionsDown(0)
{
    GetDataFromDef();
}

}