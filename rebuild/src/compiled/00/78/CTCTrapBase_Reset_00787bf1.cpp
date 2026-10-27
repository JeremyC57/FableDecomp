// CTCTrapBase::Reset() -- put the trap opener into its closing state and let the derived trap reset itself
// Leaf: ?Reset@CTCTrapBase@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00787BF1, 13 bytes, tail jump through vtable slot 31 (unaligned start: the trap TU is
//   size-optimised and packs its functions).
// Identity: Ego_r.pdb `?Reset@CTCTrapBase@@IAEXXZ` (protected, void()); its callers are the trap's manual controls:
//   ManualReset 0x00787C0C (`if (Active) Reset();`, landed) and ManualDeactivate 0x00787BFE (`Active = false; Reset();`),
//   plus 0x007885E8 / 0x0078886A in the trap update code. The body ends in the derived ResetTrap. Catalog label
//   `CAIStateGroup_MinionBehaviourCrowdBase::Cleanup` was propagated (the landed ManualReset still calls it as an
//   extern "C" `Cleanup`).
// Layout: retail CTCTrapBase (engine/CTCTrapBase.h offsets: TrapOpener CFadeInFadeOutBase +0x38, Active +0x58), modelled
//   locally because the generated header carries the vptr as a data member and this body makes a virtual call (slot 31);
//   CFadeInFadeOutBase from Ego_r.pdb (Closing +0x0c, Opening +0x0d). Virtual slots 28..31 = InitTrap / UpdateTrap /
//   TriggerTrap / ResetTrap: the derived vtables CTCMeleeTrap 0x0126B884 / CTCProjectileTrap 0x0126B90C fill them with the
//   landed InitTrap, UpdateTrap, TriggerTrap and ResetTrap bodies (Ego_r.pdb `?ResetTrap@CTCMeleeTrap@@MAEXXZ` etc.); in the
//   CTCTrapBase vtable 0x0126B7FC slots 29..31 are `__purecall`.
// Shape: two byte stores (Opening then Closing) BEFORE the vptr load, then the virtual call as a sibling `jmp [eax+7Ch]`.
//   The stores come from the opener's inline Close() (Ego_r.pdb `?Close@CFadeInFadeOutBase@@QAEXXZ`; no out-of-line body is
//   known in retail, its body here is exactly the two stores): written as direct member stores VC hoists the vptr load above
//   them. `optimize("s")` as the TU.
#pragma optimize("s", on)
class CThing;

class CFadeInFadeOutBase { public:                       // 0x1c bytes (Ego_r.pdb)
    bool          Active;                                // +0x00
    float         FadeInTime;                            // +0x04
    float         FadeOutTime;                           // +0x08
    bool          Closing;                               // +0x0c
    bool          Opening;                               // +0x0d
    float         OpenTimer;                             // +0x10
    float         CloseTimer;                            // +0x14
    unsigned char ToColour[4];                           // +0x18 CRGBColour (not touched here)
    void Close() { Opening = false; Closing = true; }
};

class CTCTrapBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
protected:
    virtual void InitTrap();                             // slot 28 (+0x70), base body 0x007891F7
    virtual void UpdateTrap() = 0;                       // slot 29 (+0x74)
    virtual void TriggerTrap() = 0;                      // slot 30 (+0x78)
    virtual void ResetTrap() = 0;                        // slot 31 (+0x7c)
public:
    CThing*            ParentThing;                      // +0x04 (CTCBase)
    unsigned char      _pad08[0x38 - 0x08];              // CTCBase flags, HitThings +0x0c, ImmuneThings +0x18,
                                                         // PObstruction +0x24, TriggerThings +0x2c (retail sizes)
    CFadeInFadeOutBase TrapOpener;                       // +0x38
    void*              PTrapDef;                         // +0x54 CDefPointer<const CTrapDef>
    bool               Active;                           // +0x58
    bool               Triggered;                        // +0x59
    bool               LastTriggerOnProximity;           // +0x5a
protected:
    void Reset();
};

void CTCTrapBase::Reset()
{
    TrapOpener.Close();
    ResetTrap();
}