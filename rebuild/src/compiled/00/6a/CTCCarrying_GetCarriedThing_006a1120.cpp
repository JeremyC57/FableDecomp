// CTCCarrying::GetCarriedThing(long defIndex) -- the first carried thing whose def index (+0x8c) matches, or null
// Retry 10b (getsw sweep, FIXES B-15): the def index is read through an inline getter `GetDefIndex()`; the plain member read gives the parked register/scheduling difference.
//
// Retail 0x006A1120, 129 bytes (`ret 4`). Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (twice per slot, a third time for the hit).
// Layout: CarrySlots at +0xc as in the landed 0x006A1060 / OnKill 0x006A0FF0 models; the thing's
// def index is the unsigned short at +0x8c (`movzx`).
// Shape: the IsCarryingThing 0x006A10A0 loop returning `GetPItem()` of the hit (pointer address in
// ebx, offset in ebp) and `xor eax,eax` after the loop.
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short m_defIndex;                      // +0x8c (the thing's def/type index)
    unsigned short GetDefIndex() const { return m_defIndex; }
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
    CThing* GetCarriedThing(long defIndex);
};

CThing* CTCCarrying::GetCarriedThing(long defIndex)
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i)
    {
        CIntelligentPointer<CThing>& carried = CarrySlots[i].second;
        if (carried.GetPItem() && carried.GetPItem()->GetDefIndex() == defIndex)
            return carried.GetPItem();
    }
    return 0;
}