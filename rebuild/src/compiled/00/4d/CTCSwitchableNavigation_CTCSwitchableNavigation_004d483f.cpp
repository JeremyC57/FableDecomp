// CTCSwitchableNavigation::CTCSwitchableNavigation(CThing& thing) -- CTCBase(thing), then this class's vtable and MapIndex = 0
// Leaf: ??0CTCSwitchableNavigation@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D483F, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123C02C.
// Identity (proven): CTCSwitchableNavigation::CTCSwitchableNavigation(CThing&); evidence: it stores vtable 0x0123C02C whose RTTI is .?AVCTCSwitchableNavigation@@ (: CTCBase : CBaseClassNonCopyable : CBaseClass); FableWin.pdb holds
//   ??0CTCSwitchableNavigation@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004D877C.
// Label: the manifest's "CCrimeStatBar" is propagated (family H073: six TC constructors that zero one member, the shape of the landed
//   family G056, e.g. CTCCreatureHive::CTCCreatureHive 0x004D45C7).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Layout: Ego_r.pdb CTCSwitchableNavigation::MapIndex (long) at retail +0x0c; CTCBase modelled locally as the landed G056 rows do (engine/CTCBase.h
//   has no constructor), Review at landing.
#pragma optimize("s", on)
class CThing;
class CTCBase {                                            // retail 0x0C bytes
public:
    CTCBase(CThing& thing);                                // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CTCSwitchableNavigation : public CTCBase {
public:
    CTCSwitchableNavigation(CThing& thing);
    long MapIndex;                    // +0x0c
};
CTCSwitchableNavigation::CTCSwitchableNavigation(CThing& thing)
    : CTCBase(thing), MapIndex(0)
{
}