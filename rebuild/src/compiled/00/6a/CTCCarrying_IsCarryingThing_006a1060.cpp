// CTCCarrying::IsCarryingThing(CThing* thing) -- linear search of the carry-slot map for an entry whose intelligent pointer resolves to the thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1060, length 58 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (landed). 24 callers.
// Layout: Ego_r.pdb CTCCarrying: CarrySlots (CVectorMap<long, CIntelligentPointer<CThing>>) at +0xc -- the
// retail sorted vector {first +0xc, last +0x10, end +0x14} of 12-byte {slot, pointer} pairs, as the landed
// CTCCarrying::HasCarrySlot 0x006A11B0.
// Shape: an inline FindThing loop (must be `__forceinline`: VC7.1 does not inline loops by default) whose
// not-found path returns end() (`mov esi,[edi+10h]`), then `!= end()` re-reads `last` for the bool
// (`xor eax,eax ; cmp ; setne al`). The loop re-reads `last` each iteration.
class CThing;

struct CBaseIntelligentPointer {                    // 8 bytes: vptr, item block
    void* m_vtable;
    void* m_pBlock;
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct SCarrySlotEntry {                            // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                    first;
    CBaseIntelligentPointer second;
};

struct CCarrySlotMap {                              // retail CVectorMap<long,CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    bool             m_compare;
    bool             m_dirty;
    SCarrySlotEntry* begin() { return m_pFirst; }
    SCarrySlotEntry* end() { return m_pLast; }
};

class CTCCarrying { public:
    virtual void v00();
    unsigned char _pad_0x04[0x0c - 0x04];           // +0x04 .. +0x0c: CTCBase state
    CCarrySlotMap CarrySlots;                       // +0x0c
    __forceinline SCarrySlotEntry* FindThing(CThing* thing)
    {
        for (SCarrySlotEntry* it = CarrySlots.begin(); it != CarrySlots.end(); ++it)
            if (it->second.GetPItem() == thing)
                return it;
        return CarrySlots.end();
    }
    bool IsCarryingThing(CThing* thing);
};

bool CTCCarrying::IsCarryingThing(CThing* thing)
{
    return FindThing(thing) != CarrySlots.end();
}