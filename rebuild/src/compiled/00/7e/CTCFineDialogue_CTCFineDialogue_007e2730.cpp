// CTCFineDialogue::CTCFineDialogue(CThing* thing) -- CTCBase(thing), own vtable, give the player the chance to pay, not waiting for confirmation, then join the global update list
// Leaf: ??0CTCFineDialogue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2730, length 40 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite", propagated),
// CTCBase::AddToGlobalTCs() @ 0x00686ED0 (named by the landed CTCActionUseBuyHouse constructor 0x0072E690).
// Vtable 0x01270274 = CTCFineDialogue (rtti-vtables.tsv): the chunk label "CTCQuickAccessMenu::CTCQuickAccessMenu" is propagated.
// Layout = Ego_r.pdb CTCFineDialogue (0x1c): Fine +0x0c, Money +0x10, GivePlayerChanceToPayFine +0x14,
// WaitingForConfirmation +0x15, Village +0x18 (Fine, Money and Village are left uninitialised).
// Shape: speed TU; `mov ecx,esi` for the body call is hoisted above the vtable store, the two bool initialisers are byte
// immediates after it.
class CThing;

class CTCBase { public:                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                           // 0x00686800
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

class CTCVillage;

class CTCFineDialogue : public CTCBase { public:                    // vtable 0x01270274
    long        Fine;                                       // +0x0c
    long        Money;                                      // +0x10
    bool        GivePlayerChanceToPayFine;                  // +0x14
    bool        WaitingForConfirmation;                     // +0x15
    CTCVillage* Village;                                    // +0x18
    virtual void v00();
    CTCFineDialogue(CThing* thing);
};

CTCFineDialogue::CTCFineDialogue(CThing* thing)
    : CTCBase(thing)
    , GivePlayerChanceToPayFine(true)
    , WaitingForConfirmation(false)
{
    AddToGlobalTCs();
}