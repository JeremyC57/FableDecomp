// CTCCarrying::RemoveThing(CThing* thing) -- finds the carry slot holding the thing and, when found, removes the thing from that slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A19B0, length 65 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCCarrying::RemoveThingInCarrySlot(long, bool)
// @ 0x006A16A0 (manifest; `ret 8`). 38 callers, among them CTCCarryable::OnKill 0x00705150.
// Layout: CarrySlots at +0xc as 0x006A1060 (retail 12-byte vector of {slot, CIntelligentPointer<CThing>}).
// Shape: the same `__forceinline` FindThing loop as IsCarryingThing, then `!= end()` re-reads `last`
// and the slot key of the hit is passed with a false flag.
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
    void RemoveThingInCarrySlot(long slot, bool dropped);   // 0x006A16A0
    void RemoveThing(CThing* thing);
};

void CTCCarrying::RemoveThing(CThing* thing)
{
    SCarrySlotEntry* it = FindThing(thing);
    if (it != CarrySlots.end())
        RemoveThingInCarrySlot(it->first, false);
}