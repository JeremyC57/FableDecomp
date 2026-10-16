// CTCOwnedEntity::OnInitialActivate() -- while the owner thing is alive, copy its drawable state and its limbo state onto this entity
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E83B0, length 97 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (POwner, four times), CThing virtual slot 27 DrawIsDrawable (+0x6C,
// 0x004C7790) on the owner and slot 28 SetAsDrawable(bool) (+0x70, 0x004C8E50) on ParentThing, CThing::SetInLimbo(bool) @ 0x004C8CF0.
// Layout: PDB CTCOwnedEntity (0x20) = retail: SwitchableNavigationTCAdded +0x0C, POwner +0x0D (CIntelligentPointer<CThing> at an
// odd offset: the pointer class is byte-aligned, declared under pack(1)), OwnerUID +0x18; CThing flag byte +0x91 (PDB +0xA1):
// bit 0 Dead, bit 5 InLimbo.
// Shape: IS_THING_ALIVE inlined; the owner and ParentThing are named locals for the drawable copy (retail keeps the parent in edi
// and its vtable in ebp across the owner's virtual call); SetInLimbo re-reads ParentThing; the InLimbo bit field is pushed as
// `movzx ; shr al,5 ; and eax,0FFFFFF01h`. CTCBase is a real base: a flat vptr + __int64 class pads the vptr to 8.
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool DrawIsDrawable() const;            // slot 27 (+0x6c)
    virtual void SetAsDrawable(bool drawable);      // slot 28 (+0x70)
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                         // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;                      // +0x91 bit 5
    void SetInLimbo(bool inLimbo);                  // 0x004C8CF0
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};
#pragma pack(pop)
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->Dead)
            return true;
    }
    return false;
}
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
};
struct CTCOwnedEntity : CTCBase {                   // CTCBase as a base: a flat vptr + __int64 class would pad the vptr to 8
    bool SwitchableNavigationTCAdded;               // +0x0c
    CBaseIntelligentPointer POwner;                 // +0x0d
    unsigned __int64 OwnerUID;                      // +0x18
    void OnInitialActivate();
};

void CTCOwnedEntity::OnInitialActivate()
{
    if (IS_THING_ALIVE(POwner)) {
        CThing* owner = POwner.GetPItem();
        CThing* parent = ParentThing;
        parent->SetAsDrawable(owner->DrawIsDrawable());
        ParentThing->SetInLimbo(POwner.GetPItem()->InLimbo);
    }
}
