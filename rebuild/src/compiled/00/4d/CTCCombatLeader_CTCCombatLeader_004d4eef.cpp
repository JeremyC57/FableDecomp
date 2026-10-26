// CTCCombatLeader::CTCCombatLeader(CThing& thing) -- CTCBase(thing), then this class's vtable and mLastEventReactionFrame = 0
// Leaf: ??0CTCCombatLeader@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4EEF, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123C5AC.
// Identity (proven): CTCCombatLeader::CTCCombatLeader(CThing&); evidence: it stores vtable 0x0123C5AC whose RTTI is .?AVCTCCombatLeader@@ (: CTCBase : CBaseClassNonCopyable : CBaseClass); FableWin.pdb holds
//   ??0CTCCombatLeader@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004D8942.
// Label: the manifest's "CCrimeStatBar" is propagated (family H073: six TC constructors that zero one member, the shape of the landed
//   family G056, e.g. CTCCreatureHive::CTCCreatureHive 0x004D45C7).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Layout: Ego_r.pdb CTCCombatLeader::mLastEventReactionFrame (unsigned long) at retail +0x0c; CTCBase modelled locally as the landed G056 rows do (engine/CTCBase.h
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
class CTCCombatLeader : public CTCBase {
public:
    CTCCombatLeader(CThing& thing);
    unsigned long mLastEventReactionFrame;                    // +0x0c
};
CTCCombatLeader::CTCCombatLeader(CThing& thing)
    : CTCBase(thing), mLastEventReactionFrame(0)
{
}