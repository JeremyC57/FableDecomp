// CTCExclusiveUse::CTCExclusiveUse(CThing& thing) -- CTCBase(thing), then this class's vtable and an empty User (CIntelligentPointer<CThing>)
// Leaf: ??0CTCExclusiveUse@@QAE@AAVCThing@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7D7E, 42 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123EA5C.
// Identity (proven): CTCExclusiveUse::CTCExclusiveUse(CThing&); evidence: it stores vtable 0x0123EA5C whose RTTI is .?AVCTCExclusiveUse@@ (: CTCBase : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb holds
//   ??0CTCExclusiveUse@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004DAF2E.
// Label: the manifest's "CInputProcessControlSpirit" is propagated (family H074: six TC constructors whose only constructed member is an intelligent
//   pointer).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, then the member's vtable 0x01238C6C
//   (RTTI .?AV?$CIntelligentPointer@VCThing@@@@).
// Layout: Ego_r.pdb CTCExclusiveUse::User at retail +0x0C; the members before it have trivial constructors. CTCBase and the
//   intelligent pointers are modelled locally as the landed TC constructors do (engine/CTCBase.h has no constructor), Review at landing.
#pragma optimize("s", on)
class CThing;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCBase {                                            // retail 0x0C bytes
public:
    CTCBase(CThing& thing);                                // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CTCExclusiveUse : public CTCBase {
public:
    CTCExclusiveUse(CThing& thing);
    CIntelligentPointer<CThing> User;          // +0x0C
};
CTCExclusiveUse::CTCExclusiveUse(CThing& thing)
    : CTCBase(thing)
{
}