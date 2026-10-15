// CTCOwnedEntity::vfunc_19() const -- true while the owner is set and alive (its dead bit, +0x91 bit 0, clear)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8440, length 27 bytes (byte-identical twin: vfunc_23 @ 0x007E8420).
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (once, on POwner at +0x0d: `add ecx,0Dh`).
// Retail CTCOwnedEntity (0x20 bytes, PDB): SwitchableNavigationTCAdded +0x0c, POwner (CIntelligentPointer<CThing>,
// 8 bytes: vptr + item) at the unaligned +0x0d, OwnerUID (unsigned __int64) +0x18 -- modelled under pack(1).
// Shape: the pointer fetched once into a local, `if (p && !dead) return true; return false;` gives `mov al,1` and the
// shared `xor al,al` tail.
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

#pragma pack(push, 1)
struct CTCBase_007e8440 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

struct CTCOwnedEntity : CTCBase_007e8440 {
    bool                    SwitchableNavigationTCAdded;   // +0x0c
    CBaseIntelligentPointer POwner;                 // +0x0d CIntelligentPointer<CThing>
    unsigned char           _pad15[3];
    unsigned __int64        OwnerUID;               // +0x18
    virtual bool vfunc_19() const;
};
#pragma pack(pop)

bool CTCOwnedEntity::vfunc_19() const
{
    CThing* owner = POwner.GetPItem();
    if (owner && !(owner->m_flags91 & 1))
        return true;
    return false;
}
