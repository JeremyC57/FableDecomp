// CTCCreatureHive::CTCCreatureHive(CThing& thing) -- CTCBase(thing), then this class's vtable and Animating = false
// Leaf: ??0CTCCreatureHive@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D45C7, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123BDCC.
// Identity: CTCCreatureHive::CTCCreatureHive(CThing&) -- proven; evidence: it stores vtable 0x0123BDCC whose RTTI is .?AVCTCCreatureHive@@ : CTCBase : CBaseClassNonCopyable : CBaseClass;
//   FableWin.pdb name set holds ??0CTCCreatureHive@@QAE@AAVCThing@@@Z; its only caller is the landed type factory 0x004D86B7 (CTCCreatureHive_Alloc). Family G056 (seven such constructors; the propagated label
//   "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is right only for 0x004DCA62).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800 (the TC base constructor, bound under this name).
// Layout: Ego_r.pdb CTCCreatureHive::Animating (bool) at retail +0x0c.
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
class CTCCreatureHive : public CTCBase {
public:
    CTCCreatureHive(CThing& thing);
    bool Animating;                    // +0x0c
};
CTCCreatureHive::CTCCreatureHive(CThing& thing)
    : CTCBase(thing), Animating(false)
{
}