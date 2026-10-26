// CTCCarrying::GetThingInCarrySlot(long slot) -- the thing held in a carry slot, or null when the slot is absent
// Leaf: GetThingInCarrySlot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A11F0, length 59 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap<long,CIntelligentPointer<CThing>>::LowerBound(const long&) const @ 0x006A2A10,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Identity: Ego_r.pdb `?GetThingInCarrySlot@CTCCarrying@@QBEPAVCThing@@J@Z`; the catalog's "CThing_Array_FindByID"
//   is a descriptive auto label. Neighbours: the landed HasCarrySlot 0x006A11B0 (same inline Find).
// Layout: CarrySlots (CVectorMap<long, CIntelligentPointer<CThing>>) at +0xc = the retail sorted vector
//   {first +0xc, last +0x10, end +0x14} of 12-byte {slot, pointer} pairs (the landed CTCCarrying models).
// Shape: inline Find -- lower bound, then "not found" when the bound is end() or the key sorts before the
//   element's key -- compared with end() re-read from +0x10; the hit returns GetPItem() of the pointer
//   (`call ; ret 4`), the miss `xor eax,eax`.
class CThing;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct SCarrySlotEntry {                            // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                    first;
    CIntelligentPointer<CThing> second;
};

struct CCarrySlotMap {                              // retail CVectorMap<long,CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    bool             m_compare;
    bool             m_dirty;
    const SCarrySlotEntry* LowerBound(const long& key) const;   // 0x006A2A10
    const SCarrySlotEntry* End() const { return m_pLast; }
    const SCarrySlotEntry* Find(const long& key) const
    {
        const SCarrySlotEntry* it = LowerBound(key);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};

class CTCCarrying {
public:
    virtual void v00();
    unsigned char _pad_0x04[0x0c - 0x04];           // +0x04 .. +0x0c: CTCBase state
    CCarrySlotMap CarrySlots;                       // +0x0c
    CThing* GetThingInCarrySlot(long slot) const;
};

CThing* CTCCarrying::GetThingInCarrySlot(long slot) const
{
    const SCarrySlotEntry* it = CarrySlots.Find(slot);
    if (it != CarrySlots.End())
        return it->second.GetPItem();
    return 0;
}