// CTCSearchableContainer::CTCSearchableContainer(CThing& thing) -- CTCContainerRewardHero(thing), then this class's vtable and NumberOfSearchesRemaining = 0
// Leaf: ??0CTCSearchableContainer@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E48A7, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x012431F4.
// Identity (proven): CTCSearchableContainer::CTCSearchableContainer(CThing&); evidence: it stores vtable 0x012431F4 whose RTTI is .?AVCTCSearchableContainer@@ (: CTCContainerRewardHero : CTCContainer : CTCBase : CBaseClassNonCopyable : CBaseClass); FableWin.pdb holds
//   ??0CTCSearchableContainer@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004E746D.
// Label: the manifest's "CCrimeStatBar" is propagated (family H073: six TC constructors that zero one member, the shape of the landed
//   family G056, e.g. CTCCreatureHive::CTCCreatureHive 0x004D45C7).
// Callees: CTCContainerRewardHero::CTCContainerRewardHero(CThing&) 0x0074E6B0 (landed).
// Layout: Ego_r.pdb CTCSearchableContainer::NumberOfSearchesRemaining (long) at retail +0x1c (Ego_r.pdb +0x20: the base's vector is 16 bytes there); CTCBase modelled locally as the landed G056 rows do (engine/CTCBase.h
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
class CTCContainerRewardHero : public CTCBase {           // retail 0x1C bytes: CTCContainer's 12-byte vector + Enabled / CreateObjectsThatTimeOut
public:
    CTCContainerRewardHero(CThing& thing);                 // 0x0074E6B0
    unsigned char _CTCContainerRewardHero[0x10];           // +0x0C
};
class CTCSearchableContainer : public CTCContainerRewardHero {
public:
    CTCSearchableContainer(CThing& thing);
    long NumberOfSearchesRemaining;                    // +0x1c
};
CTCSearchableContainer::CTCSearchableContainer(CThing& thing)
    : CTCContainerRewardHero(thing), NumberOfSearchesRemaining(0)
{
}