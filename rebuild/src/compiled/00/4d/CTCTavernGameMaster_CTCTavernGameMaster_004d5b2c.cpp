// CTCTavernGameMaster::CTCTavernGameMaster(CThing& thing) -- CTCBase(thing), then this class's vtable and TavernGame = 0
// Leaf: ??0CTCTavernGameMaster@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5B2C, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123D664.
// Identity (proven): CTCTavernGameMaster::CTCTavernGameMaster(CThing&); evidence: it stores vtable 0x0123D664 whose RTTI is .?AVCTCTavernGameMaster@@ (: CTCBase : CBaseClassNonCopyable : CBaseClass); FableWin.pdb holds
//   ??0CTCTavernGameMaster@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004D908D.
// Label: the manifest's "CCrimeStatBar" is propagated (family H073: six TC constructors that zero one member, the shape of the landed
//   family G056, e.g. CTCCreatureHive::CTCCreatureHive 0x004D45C7).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Layout: Ego_r.pdb CTCTavernGameMaster::TavernGame (CTCTavernGame*) at retail +0x0c; CTCBase modelled locally as the landed G056 rows do (engine/CTCBase.h
//   has no constructor), Review at landing.
#pragma optimize("s", on)
class CThing;
class CTCTavernGame;
class CTCBase {                                            // retail 0x0C bytes
public:
    CTCBase(CThing& thing);                                // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CTCTavernGameMaster : public CTCBase {
public:
    CTCTavernGameMaster(CThing& thing);
    CTCTavernGame* TavernGame;                    // +0x0c
};
CTCTavernGameMaster::CTCTavernGameMaster(CThing& thing)
    : CTCBase(thing), TavernGame(0)
{
}