// CTCTavernTankard::CTCTavernTankard(CThing& thing) -- CTCBase(thing), then this class's vtable and an empty PBarmaid (CIntelligentPointer<CThingAICreature>)
// Leaf: ??0CTCTavernTankard@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8B3C, 42 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123EDCC.
// Identity (proven): CTCTavernTankard::CTCTavernTankard(CThing&); evidence: it stores vtable 0x0123EDCC whose RTTI is .?AVCTCTavernTankard@@ (: CTCBase : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb holds
//   ??0CTCTavernTankard@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004DB124.
// Label: the manifest's "CInputProcessControlSpirit" is propagated (family H074: six TC constructors whose only constructed member is an intelligent
//   pointer).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, then the member's vtable 0x0123E7E0
//   (RTTI .?AV?$CIntelligentPointer@VCThingAICreature@@@@).
// Layout: Ego_r.pdb CTCTavernTankard::PBarmaid at retail +0x34; the members before it have trivial constructors. CTCBase and the
//   intelligent pointers are modelled locally as the landed TC constructors do (engine/CTCBase.h has no constructor), Review at landing.
#pragma optimize("s", on)
class CThing;
class CThingAICreature;
class C3DVector { public: float X, Y, Z; };               // 12 bytes, trivial
class CRightHandedSet { public: C3DVector Axes[2]; };    // 24 bytes, trivial
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
class CTCTavernTankard : public CTCBase {
public:
    CTCTavernTankard(CThing& thing);
    float           Fullness;                              // +0x0C
    CRightHandedSet RHSet;                                 // +0x10
    C3DVector       Pos;                                   // +0x28
    CIntelligentPointer<CThingAICreature> PBarmaid;          // +0x34
};
CTCTavernTankard::CTCTavernTankard(CThing& thing)
    : CTCBase(thing)
{
}