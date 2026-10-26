// NUISystem::CScrollBarOutside::CScrollBarOutside(long owner) -- CClickable(owner), the three vtables of this class, the scroll-bar pointer and the two clicked-above/under action list pointers cleared, then GetDataFromDef()
// Retail: Fable.exe (TLC, VC7.1) @ 0x00559360, 66 bytes.
// Callees: NUISystem::CClickable::CClickable(long) @ 0x0055B460 (landed), GetDataFromDef @ 0x00559230
// (on `this`, in the body; manifest "CScrollBarOutside::Initialize").
// Vtables 0x0124B54C / 0x0124B528 / 0x0124B520 = NUISystem::CScrollBarOutside (rtti-vtables.tsv).
//
// Shape: base constructor, `xor eax,eax ; mov ecx,esi`, the three vtable stores, the three zero stores
// from the shared register (init-list members follow the vtable stores in this module, as in the landed
// CDraggable 0x0055A9C0), then the body call.
// Retail layout = Ego_r.pdb minus 0x30 (retail CClickable 0x194 bytes): PScrollBar +0x194,
// PActionsOnLeftClickedAbove +0x198, PActionsOnLeftClickedUnder +0x19C (PDB 0x1C4/0x1C8/0x1CC). No engine
// header exists for the NUISystem chain at retail offsets; bases modelled as in the landed CNavButton 0x00558B90.
// Leaf: ??0CScrollBarOutside

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

class CComponent;
struct CActionList;                              // std::list<CCountedPointer<NUISystem::CAction>>

struct CClickable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface {   // retail 0x194 bytes
    unsigned char _body[0x194 - 0x1c];
    CClickable_Retail(long owner);               // 0x0055B460
};

class CScrollBarOutside : public CClickable_Retail { public:   // vtables 0x0124B54C / 0x0124B528 / 0x0124B520
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v0();
    CComponent*  PScrollBar;                     // +0x194
    CActionList* PActionsOnLeftClickedAbove;     // +0x198
    CActionList* PActionsOnLeftClickedUnder;     // +0x19C
    CScrollBarOutside(long owner);
    void GetDataFromDef();                       // 0x00559230
};

CScrollBarOutside::CScrollBarOutside(long owner)
    : CClickable_Retail(owner)
    , PScrollBar(0)
    , PActionsOnLeftClickedAbove(0)
    , PActionsOnLeftClickedUnder(0)
{
    GetDataFromDef();
}

}