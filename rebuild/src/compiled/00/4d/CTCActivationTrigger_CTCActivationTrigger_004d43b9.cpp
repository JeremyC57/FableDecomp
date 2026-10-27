// CTCActivationTrigger::CTCActivationTrigger(CThing& thing) -- thing-component constructor: the CTCBase base, then the members zeroed
// Leaf: ??0CTCActivationTrigger@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D43B9, 38 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): stores vtable 0x0123BBAC (RTTI .?AVCTCActivationTrigger@@); the zeroed members match Ego_r.pdb CTCActivationTrigger; its caller is CTCActivationTrigger::Alloc 0x004D8638.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Shape (family M095, representative 0x004D43B9): `: CTCBase(thing), PTCReceptor(0), ReceptorUID(0), Activated(false) {}` (the landed CTCGold 0x007CBDA0 model);
//   `optimize("s")`: the thing reference pushed straight from its slot.
// Header debt: engine/CTCBase.h flattens the vptr, so the base is modelled locally (as the landed CTCGold).
#pragma optimize("s", on)
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb)
public:
    CTCBase(CThing& thing);                              // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
class CTCActivationReceptorBase;
class CTCActivationTrigger : public CTCBase {
public:
    CTCActivationTrigger(CThing& thing);
    CTCActivationReceptorBase* PTCReceptor;              // +0x0C
    unsigned __int64 ReceptorUID;                        // +0x10
    bool Activated;                                      // +0x18
};
CTCActivationTrigger::CTCActivationTrigger(CThing& thing)
    : CTCBase(thing), PTCReceptor(0), ReceptorUID(0), Activated(false)
{
}