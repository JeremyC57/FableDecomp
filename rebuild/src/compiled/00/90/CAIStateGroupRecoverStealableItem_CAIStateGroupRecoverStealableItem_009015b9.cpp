// CAIStateGroup_RecoverStealableItem::CAIStateGroup_RecoverStealableItem() -- base ctor, own vtable, three intelligent pointers, NeedsCleanup = false
// Leaf: ??0CAIStateGroup_RecoverStealableItem
// Retail 0x009015B9, 0x47 bytes. Manifest label "CAIStateGroup_TrollLarge_Constructor" (_global) is wrong: the vtable
// stored at +0 is 0x0128CC9C, whose RTTI type descriptor reads `.?AVCAIStateGroup_RecoverStealableItem@@`.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255 (out of line), CBaseIntelligentPointer::
// CBaseIntelligentPointer @ 0x00A01B10 (base of each inlined CIntelligentPointer<T> constructor).
// Member vtables (RTTI): CIntelligentPointer<CThing> 0x01238C6C (+0x14, +0x24, held in ebx and reused),
// CIntelligentPointer<const CThing> 0x01237F1C (+0x1c).
// Layout from engine/CAIStateGroup_RecoverStealableItem.h (PDB names): PStealableItem +0x14, PTargetPoint +0x1c,
// PPreviouslyCarriedItem +0x24, GotActionPoint +0x2c, ActionPointIndex +0x30, ReachedDestination +0x34,
// PickUpActionID +0x38, NeedsCleanup +0x3c. The header flattens the bases into bytes, so the class is modelled
// locally with the real inheritance (as the landed CAIStateGroup_GoToBed 0x008FEB6B does).
// Shape: base ctor, own vtable store (scheduled after the first member's `lea`), the three pointer members in
// declaration order, and the one initialised scalar (NeedsCleanup) last -- an init-list entry in declaration order.
// Pragma: optimize("s") -- the size TU loads a vtable constant used twice or more into a callee-saved register.
#pragma optimize("s", on)

class CThing;

class CAIStateGroupBase { public:
    virtual void v00();
    unsigned char _base[0x10];                  // +0x04 PBrain, ScriptAIPriority, Name, Terminated/Priority/Enabled
    CAIStateGroupBase();                        // 0x008FD255
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CAIStateGroup_RecoverStealableItem : CAIStateGroupBase {
    CIntelligentPointer<CThing>       PStealableItem;           // +0x14
    CIntelligentPointer<const CThing> PTargetPoint;             // +0x1c
    CIntelligentPointer<CThing>       PPreviouslyCarriedItem;   // +0x24
    bool          GotActionPoint;                               // +0x2c
    long          ActionPointIndex;                             // +0x30
    bool          ReachedDestination;                           // +0x34
    long          PickUpActionID;                               // +0x38
    bool          NeedsCleanup;                                 // +0x3c
    virtual void v00();
    CAIStateGroup_RecoverStealableItem();
};

CAIStateGroup_RecoverStealableItem::CAIStateGroup_RecoverStealableItem()
    : CAIStateGroupBase()
    , PStealableItem()
    , PTargetPoint()
    , PPreviouslyCarriedItem()
    , NeedsCleanup(false)
{
}