// CTCGeneratedCreature::CTCGeneratedCreature(CThing& thing) -- CTCBase(thing), then this class's vtable and an empty PGenerator (CIntelligentPointer<const CThing>)
// Leaf: ??0CTCGeneratedCreature@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7FF6, 42 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123EBC4.
// Identity (proven): CTCGeneratedCreature::CTCGeneratedCreature(CThing&); evidence: it stores vtable 0x0123EBC4 whose RTTI is .?AVCTCGeneratedCreature@@ (: CTCBase : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb holds
//   ??0CTCGeneratedCreature@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004DAF85.
// Label: the manifest's "CInputProcessControlSpirit" is propagated (family H074: six TC constructors whose only constructed member is an intelligent
//   pointer).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, then the member's vtable 0x01237F1C
//   (RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@).
// Layout: Ego_r.pdb CTCGeneratedCreature::PGenerator at retail +0x0C; the members before it have trivial constructors. CTCBase and the
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
class CTCGeneratedCreature : public CTCBase {
public:
    CTCGeneratedCreature(CThing& thing);
    CIntelligentPointer<const CThing> PGenerator;          // +0x0C
};
CTCGeneratedCreature::CTCGeneratedCreature(CThing& thing)
    : CTCBase(thing)
{
}