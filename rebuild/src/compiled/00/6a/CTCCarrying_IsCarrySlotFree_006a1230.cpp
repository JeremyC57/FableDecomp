// CTCCarrying::IsCarrySlotFree(long slot) -- true when the carry slot exists and holds no thing
// Leaf: IsCarrySlotFree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1230, length 64 bytes, `ret 4`.
// Callees: CVectorMap<long,CIntelligentPointer<CThing>>::LowerBound(const long&) const @ 0x006A2A10,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Identity: Ego_r.pdb `?IsCarrySlotFree@CTCCarrying@@QBE_NJ@Z`; the catalog's "InventoryFindItemByIDLowerBound"
//   is a descriptive auto label. Neighbours: the landed HasCarrySlot 0x006A11B0 (same inline Find).
// Layout: CarrySlots (CVectorMap<long, CIntelligentPointer<CThing>>) at +0xc = the retail sorted vector
//   {first +0xc, last +0x10, end +0x14} of 12-byte {slot, pointer} pairs (the landed CTCCarrying models).
// Shape: inline Find (as GetThingInCarrySlot 0x006A11F0) compared with end(); a missing slot returns
//   `xor al,al`, a present one `GetPItem() == 0` as `neg ; sbb ; inc` (a bool built from the pointer test).
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
    bool IsCarrySlotFree(long slot) const;
};

bool CTCCarrying::IsCarrySlotFree(long slot) const
{
    const SCarrySlotEntry* it = CarrySlots.Find(slot);
    if (it == CarrySlots.End())
        return false;
    return it->second.GetPItem() == 0;
}