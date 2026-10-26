// CTCSinglePersonOccupiable::CTCSinglePersonOccupiable(CThing& thing) -- CTCBase(thing), then this class's vtable and an empty POccupier (CIntelligentPointer<CThingCreatureBase>)
// Leaf: ??0CTCSinglePersonOccupiable@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8896, 42 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123ED54.
// Identity (proven): CTCSinglePersonOccupiable::CTCSinglePersonOccupiable(CThing&); evidence: it stores vtable 0x0123ED54 whose RTTI is .?AVCTCSinglePersonOccupiable@@ (: CTCBase : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb holds
//   ??0CTCSinglePersonOccupiable@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004DB0AD.
// Label: the manifest's "CInputProcessControlSpirit" is propagated (family H074: six TC constructors whose only constructed member is an intelligent
//   pointer).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, then the member's vtable 0x0123E7D8
//   (RTTI .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@).
// Layout: Ego_r.pdb CTCSinglePersonOccupiable::POccupier at retail +0x0C; the members before it have trivial constructors. CTCBase and the
//   intelligent pointers are modelled locally as the landed TC constructors do (engine/CTCBase.h has no constructor), Review at landing.
#pragma optimize("s", on)
class CThing;
class CThingCreatureBase;
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
class CTCSinglePersonOccupiable : public CTCBase {
public:
    CTCSinglePersonOccupiable(CThing& thing);
    CIntelligentPointer<CThingCreatureBase> POccupier;          // +0x0C
};
CTCSinglePersonOccupiable::CTCSinglePersonOccupiable(CThing& thing)
    : CTCBase(thing)
{
}