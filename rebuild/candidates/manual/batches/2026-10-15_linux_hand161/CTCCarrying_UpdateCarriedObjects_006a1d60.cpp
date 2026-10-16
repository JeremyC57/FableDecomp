// CTCCarrying::UpdateCarriedObjects() -- per carried thing, update its CTCCarryable component (attachment) without forcing
//
// Retail 0x006A1D60, 158 bytes (plain `ret`). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (this module's copy),
// CTCCarryable::UpdateCarriedPosition(bool) @ 0x00705B60 (passed false).
// Layout: CarrySlots at +0xc as in the landed 0x006A1060 / OnKill 0x006A0FF0 models.
// Shape: the OnKill index loop; the CTCCarryable out pointer (interface 0x47: bitset word 2 bit 7,
// `mov cl,[eax+28h] ; test cl,cl ; jns`) is declared OUTSIDE the loop and never initialised, so a
// thing without the interface reuses the previous value (edi, first loaded from the frame word
// that also homes the lookup key).
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCCarryable {
    void UpdateCarriedPosition(bool force);                      // 0x00705B60
};

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
    SCarrySlotEntry* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    SCarrySlotEntry& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CTCCarrying {
    virtual void v00();
    unsigned char _pad_0x04[0x0c - 0x04];           // +0x04 .. +0x0c: CTCBase state
    CCarrySlotMap CarrySlots;                       // +0x0c
    void UpdateCarriedObjects();
};

void CTCCarrying::UpdateCarriedObjects()
{
    CTCCarryable* carryable;
    for (unsigned long i = 0; i < CarrySlots.size(); ++i)
    {
        CThing* thing = CarrySlots[i].second.GetPItem();
        if (thing)
        {
            thing->GetInterface(&carryable, 0x47);          // TC_CARRYABLE
            carryable->UpdateCarriedPosition(false);
        }
    }
}
