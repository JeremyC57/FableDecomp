// CTCCreatureItemLevitationNymph::CTCCreatureItemLevitationNymph(CThing& thing) -- CTCCreatureItemLevitationBase(thing), then this class's vtable and an empty PTarget (CIntelligentPointer<const CThing>)
// Leaf: ??0CTCCreatureItemLevitationNymph@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE700, 42 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x012415F4.
// Identity (proven): CTCCreatureItemLevitationNymph::CTCCreatureItemLevitationNymph(CThing&); evidence: it stores vtable 0x012415F4 whose RTTI is .?AVCTCCreatureItemLevitationNymph@@ (: CTCCreatureItemLevitationBase : CTCBase : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb holds
//   ??0CTCCreatureItemLevitationNymph@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004E0984.
// Label: the manifest's "CInputProcessControlSpirit" is propagated (family H074: six TC constructors whose only constructed member is an intelligent
//   pointer).
// Callees: CTCCreatureItemLevitationBase::CTCCreatureItemLevitationBase(CThing&) 0x004DE6B6 (landed); CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, then the member's vtable 0x01237F1C
//   (RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@).
// Layout: Ego_r.pdb CTCCreatureItemLevitationNymph::PTarget at retail +0x28 (Ego_r.pdb +0x30: the base's list is 12 bytes there); the members before it have trivial constructors. CTCBase and the
//   intelligent pointers are modelled locally as the landed TC constructors do (engine/CTCBase.h has no constructor), Review at landing.
#pragma optimize("s", on)
class CThing;
class C2DVector { public: float X, Y; };                  // 8 bytes, trivial
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
class CTCCreatureItemLevitationBase : public CTCBase {    // retail 0x18 bytes (a 4-byte list and two words after CTCBase)
public:
    CTCCreatureItemLevitationBase(CThing& thing);          // 0x004DE6B6
    unsigned char _CTCCreatureItemLevitationBase[0x0C];    // +0x0C
};
class CTCCreatureItemLevitationNymph : public CTCCreatureItemLevitationBase {
public:
    CTCCreatureItemLevitationNymph(CThing& thing);
    C2DVector NymphHoverXYDirectionFromTarget;              // +0x18
    float     NymphHoverHeightOffset;                       // +0x20
    float     NymphHoverXYMagnitude;                        // +0x24
    CIntelligentPointer<const CThing> PTarget;          // +0x28
};
CTCCreatureItemLevitationNymph::CTCCreatureItemLevitationNymph(CThing& thing)
    : CTCCreatureItemLevitationBase(thing)
{
}