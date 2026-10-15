// CTCActivationReceptorBase::CTCActivationReceptorBase(CThing& owner) -- CTCBase(owner), not activated, an empty trigger vector, logic type 1, deactivate after a set time with the global default frame count (150), last activation frame 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F7710, length 62 bytes.
// Leaf: ??0CTCActivationReceptorBase
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800. Vtable 0x01271F7C = CTCActivationReceptorBase (rtti-vtables.tsv). The
// default frames-to-deactivate is the long global @ 0x0138448C (initialised data, 150).
// engine/CTCActivationReceptorBase.h is NOT used: it carries the PDB's 16-byte std::vector (Triggers 0x10..0x20, LogicType
// +0x20 ..), while this retail body stores a 12-byte vector at +0x10/+0x14/+0x18 and LogicType at +0x1c (GOTCHAS: retail
// std::vector is 12 bytes, PDB layouts after one sit 4 bytes high). Retail fields: Activated +0x0c, Triggers +0x10,
// LogicType +0x1c, DeactivateAfterSetTime +0x20, FramesAfterActivationToDeactivate +0x24, FrameAtLastActivation +0x28
// (the landed Activate @ 0x007F76E0 reads +0x0c / +0x20 / +0x28 the same way).
// Shape: the plain bool initialiser is scheduled before the vtable store; the vector is a member object with an inline
// constructor (its zeroes follow the vtable store) and the later initialisers keep member order, the global's load
// pushing its store after the zero of +0x28.
struct CThing;
extern unsigned long g_defaultFramesToDeactivate;   // 0x0138448C

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CTCBase(CThing& owner);                    // 0x00686800
};

struct CThingPtrVector {                       // retail std::vector<CThing*> (12 bytes)
    CThing** m_first;
    CThing** m_last;
    CThing** m_end;
    CThingPtrVector() : m_first(0), m_last(0), m_end(0) { }
};

struct CTCActivationReceptorBase : CTCBase {   // vtable 0x01271F7C
    bool            Activated;                 // +0x0c
    CThingPtrVector Triggers;                  // +0x10
    long            LogicType;                 // +0x1c ELogicType
    bool            DeactivateAfterSetTime;    // +0x20
    unsigned long   FramesAfterActivationToDeactivate;   // +0x24
    unsigned long   FrameAtLastActivation;     // +0x28
    virtual void v00();
    CTCActivationReceptorBase(CThing& owner);
};

CTCActivationReceptorBase::CTCActivationReceptorBase(CThing& owner)
    : CTCBase(owner), Activated(false), LogicType(1), DeactivateAfterSetTime(true),
      FramesAfterActivationToDeactivate(g_defaultFramesToDeactivate), FrameAtLastActivation(0)
{
}
