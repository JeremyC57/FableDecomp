// CHitLocation::SetEnabled(bool enabled) -- record the enabled flag; for a default hit location also make it (or stop it being) the owner's default hit location in the owner's hit-locations component (kind 0x7f)
// Leaf: SetEnabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00816390, length 107 bytes.
// Callees: the component map's LowerBound @ 0x0040F020 (the inlined `find` of kind 0x7f).
// Layout = Ego_r.pdb CHitLocation (24 bytes): POwnerThing +0, PDef +4 (CDefPointer: the def word), Enabled +0x14; the
// def's Default flag at retail +0x74 (PDB +0x78, Priority's predecessor shifted); CTCHitLocations' PDefaultHitLocation
// at retail +0x18 (PDB +0x1c with the 12-byte retail vector).
// Shape: the component comes through the inline accessor that leaves its local unset when the mask bit is clear
// (retail then reads the dead `enabled` argument slot, `mov eax,[esp+0Ch]`); the mask word test of kind 0x7f is the
// sign test (`test ecx,ecx ; jns`).
class CTCHitLocations;
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long             m_kind;
    CTCHitLocations* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCHitLocations* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20: which component kinds the thing holds
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCHitLocations* GetTCIfPresent(CThing* thing, long kind)
{
    CTCHitLocations* tc;                           // deliberately unset when the component is absent
    if (thing->HasTC(kind))
        tc = thing->m_tcs.Find(kind);
    return tc;
}
class CHitLocationDef { public:
    unsigned char _pad[0x74];
    bool          Default;                         // +0x74
};
class CHitLocation;
class CTCHitLocations { public:
    unsigned char _pad[0x18];
    CHitLocation* PDefaultHitLocation;             // +0x18
};
class CHitLocation {
public:
    CThing*          POwnerThing;                  // +0x00
    CHitLocationDef* PDef;                         // +0x04 CDefPointer<const CHitLocationDef>
    void*            PArmourDef;                   // +0x08
    long             HitResponseAction;            // +0x0c
    long             BlockResponseAction;          // +0x10
    bool             Enabled;                      // +0x14
    void SetEnabled(bool enabled);
};

void CHitLocation::SetEnabled(bool enabled)
{
    Enabled = enabled;
    if (PDef->Default) {
        CTCHitLocations* locations = GetTCIfPresent(POwnerThing, 0x7f);
        if (enabled)
            locations->PDefaultHitLocation = this;
        else if (locations->PDefaultHitLocation == this)
            locations->PDefaultHitLocation = 0;
    }
}