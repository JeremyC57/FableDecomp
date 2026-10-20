// CTCCarrying::SetAsDrawable(bool drawable) -- pass the drawable state on to every live thing held in a carry slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1370, 200 bytes (`ret 4`; VC7.1 aligns the frame itself, `and esp,-8`,
// for the 8-byte intelligent-pointer local).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90,
// ::~CBaseIntelligentPointer @ 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C; CThing::SetAsDrawable (virtual
// slot 28, +0x70; CThing vtable 0x01239ABC slot 28 = 0x004C8E50).
// Layout: CarrySlots at +0x0c (retail CVectorMap<long, CIntelligentPointer<CThing>>: 12-byte vector of 12-byte pairs), as the
// landed CTCCarrying::UpdateCarriedObjects 0x006A1D60; CThing dead bit +0x91.
// Shape: the index loop re-divides the vector size every pass (signed /12, unsigned compare); each slot's pointer is
// copy-constructed into a local through the inline copy ctor (the slot address is formed from the begin pointer read
// before the base ctor, spilled to the frame), GetPItem is called afresh for the null test, the dead test and the call.
struct CThing {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27)
#undef V
    virtual void SetAsDrawable(bool drawable);          // slot 28 (+0x70)
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _91_1 : 7;
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SCarrySlotEntry {                                // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                first;
    CIntelligentPointer second;
};
struct CCarrySlotMap {                                  // retail CVectorMap<long, CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    SCarrySlotEntry* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    SCarrySlotEntry& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CTCCarrying {
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];                  // CTCBase state
    CCarrySlotMap CarrySlots;                           // +0x0c
    void SetAsDrawable(bool drawable);
};

void CTCCarrying::SetAsDrawable(bool drawable)
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i) {
        CIntelligentPointer thing(CarrySlots[i].second);
        if (thing.GetPItem() && !thing.GetPItem()->Dead)
            thing.GetPItem()->SetAsDrawable(drawable);
    }
}