// CTCFireRangedWeaponOnLeavingStateGroup::CTCFireRangedWeaponOnLeavingStateGroup(CThing& thing) -- CTCBase(thing), then this class's vtable and QuitOut = false
// Leaf: ??0CTCFireRangedWeaponOnLeavingStateGroup@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D656F, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123E464.
// Identity: CTCFireRangedWeaponOnLeavingStateGroup::CTCFireRangedWeaponOnLeavingStateGroup(CThing&) -- proven; evidence: it stores vtable 0x0123E464 whose RTTI is .?AVCTCFireRangedWeaponOnLeavingStateGroup@@ : CTCBase : CBaseClassNonCopyable : CBaseClass;
//   FableWin.pdb name set holds ??0CTCFireRangedWeaponOnLeavingStateGroup@@QAE@AAVCThing@@@Z; its only caller is the landed type factory 0x004D9723 (CTCFireRangedWeaponOnLeavingStateGroup_Alloc). Family G056 (seven such constructors; the propagated label
//   "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is right only for 0x004DCA62).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800 (the TC base constructor, bound under this name).
// Layout: Ego_r.pdb CTCFireRangedWeaponOnLeavingStateGroup::QuitOut (bool) at retail +0x0c.
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
class CTCFireRangedWeaponOnLeavingStateGroup : public CTCBase {
public:
    CTCFireRangedWeaponOnLeavingStateGroup(CThing& thing);
    bool QuitOut;                    // +0x0c
};
CTCFireRangedWeaponOnLeavingStateGroup::CTCFireRangedWeaponOnLeavingStateGroup(CThing& thing)
    : CTCBase(thing), QuitOut(false)
{
}