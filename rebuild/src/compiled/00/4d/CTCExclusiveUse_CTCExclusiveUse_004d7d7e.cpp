// CTCExclusiveUse::CTCExclusiveUse(CThing& thing) -- CTCBase(thing), then this class's vtable and an empty User (CIntelligentPointer<CThing>)
// Leaf: ??0CTCExclusiveUse@@QAE@AAVCThing@@@Z
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
class CBaseIntelligentPointer {                         // 8 bytes: vptr + PData
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    void* PData;                                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
};
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