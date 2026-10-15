// CTCOwnedEntity::OnForceRecreateUID() -- re-captures the live owner's UID into OwnerUID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8370, length 56 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times on POwner: null test, dead bit, then the read).
// Retail CTCOwnedEntity (PDB): POwner at the unaligned +0x0d (pack(1)), OwnerUID +0x18; the thing's UID at +0x18.
// Shape: the landed IS_THING_ALIVE idiom (two fetches, nested ifs) with &POwner held in edi, then a third fetch and the
// two-word copy.
struct CThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 UID;                           // +0x18
    unsigned char    _pad20[0x91 - 0x20];
    unsigned char    m_flags91;                     // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

#pragma pack(push, 1)
struct CTCBase_007e8370 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

struct CTCOwnedEntity : CTCBase_007e8370 {
    bool                    SwitchableNavigationTCAdded;   // +0x0c
    CBaseIntelligentPointer POwner;                 // +0x0d CIntelligentPointer<CThing>
    unsigned char           _pad15[3];
    unsigned __int64        OwnerUID;               // +0x18
    void OnForceRecreateUID();
};
#pragma pack(pop)

void CTCOwnedEntity::OnForceRecreateUID()
{
    if (IS_THING_ALIVE(POwner))
        OwnerUID = POwner.GetPItem()->UID;
}