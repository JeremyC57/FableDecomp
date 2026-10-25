// CTCDRegionEntrance::CTCDRegionEntrance(CThing& thing) -- CTCDriverBase(thing), then this class's vtable and Active = true
// Leaf: ??0CTCDRegionEntrance@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D42B2, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123BA74.
// Identity: CTCDRegionEntrance::CTCDRegionEntrance(CThing&) -- proven; evidence: it stores vtable 0x0123BA74 whose RTTI is .?AVCTCDRegionEntrance@@ : CTCDriverBase : CTCBase : CBaseClassNonCopyable;
//   FableWin.pdb name set holds ??0CTCDRegionEntrance@@QAE@AAVCThing@@@Z; its only caller is the landed type factory 0x004D85FF (CTCDRegionEntrance_Alloc). Family G056 (seven such constructors; the propagated label
//   "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is right only for 0x004DCA62).
// Callees: CTCDriverBase::CTCDriverBase(CThing&) 0x00771200 (landed callers bind ??0CTCDriverBase@@QAE@PAUCThing@@@Z; the PDB takes CThing&).
// Layout: Ego_r.pdb CTCDRegionEntrance::Active (bool) at retail +0x0c.
#pragma optimize("s", on)
class CThing;
class CTCDriverBase {                                            // retail 0x0C bytes
public:
    CTCDriverBase(CThing& thing);                                // 0x00771200
    virtual ~CTCDriverBase();
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CTCDRegionEntrance : public CTCDriverBase {
public:
    CTCDRegionEntrance(CThing& thing);
    bool Active;                    // +0x0c
};
CTCDRegionEntrance::CTCDRegionEntrance(CThing& thing)
    : CTCDriverBase(thing), Active(true)
{
}