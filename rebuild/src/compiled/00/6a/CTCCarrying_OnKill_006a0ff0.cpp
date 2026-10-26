// CTCCarrying::OnKill() -- kill every thing still held in a carry slot (without immediate removal)
//
// Retail 0x006A0FF0, 107 bytes (plain `ret`). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (twice per slot: the null test, then the call target), CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CTCCarrying: CarrySlots (CVectorMap<long, CIntelligentPointer<CThing>>) at +0xc,
// the retail sorted vector {first +0xc, last +0x10, end +0x14} of 12-byte {slot, pointer} pairs
// (landed IsCarryingThing 0x006A1060 model).
// Shape: an unsigned index loop `i < size()` with size() the signed byte distance / 12 (`imul
// 2AAAAAABh ; sar edx,1`) recomputed every pass; the element is STLport `*(begin() + i)` (begin
// re-read, the scaled offset strength-reduced into ebx), its pointer address held in ebp.
class CThing { public:
    void Kill(bool removeNow);                      // 0x004C9B80
};

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
    SCarrySlotEntry* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    SCarrySlotEntry& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCCarrying { public:
    virtual void v00();
    unsigned char _pad_0x04[0x0c - 0x04];           // +0x04 .. +0x0c: CTCBase state
    CCarrySlotMap CarrySlots;                       // +0x0c
    void OnKill();
};

void CTCCarrying::OnKill()
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i)
    {
        CIntelligentPointer<CThing>& carried = CarrySlots[i].second;
        if (carried.GetPItem())
            carried.GetPItem()->Kill(false);
    }
}