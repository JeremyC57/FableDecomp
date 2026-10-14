// CCreatureAction_WaspQueenPrimaryAttack::FinishAction() -- base FinishAction, then remove the hit-on-collision component (interface type 200 = CTCHitOnCollision) from the creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4020, length 32 bytes (byte-identical to the LashOutAttack body @ 0x008C40D0 up to the relocations). FinishAction is vtable slot 4 (protected `MAE`); the base is called directly here.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CThing::RemoveTC(long interfaceType) @ 0x004C9840 (named from its body: it looks the type up in the thing's interface map at +0x44 and
// retires the component; the manifest "CBaseObject::OnPropertyChanged" label is propagated). The DiveAttack Initialise @ 0x008C4250 adds
// the same component ("CTCHitOnCollision", bit 8 of the bitset dword at +0x38 = type 200).
// Shape: the argument is pushed before the object expression is evaluated.
struct CThing {
    void RemoveTC(long interfaceType);      // 0x004C9840
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_WaspQueenPrimaryAttack : CCombatActionBase {
    virtual void FinishAction();
};

void CCreatureAction_WaspQueenPrimaryAttack::FinishAction()
{
    CCombatActionBase::FinishAction();
    PCreature.GetPItem()->RemoveTC(200);
}