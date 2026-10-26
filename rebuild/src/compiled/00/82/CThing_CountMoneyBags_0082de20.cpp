// CThing::CountMoneyBags() const -- catalog label; `this` is the thing: true when its interface-0x21 thing list holds at least one live thing with an interface-0x26 component and none of those components has any of its three flags set
// Leaf: CountMoneyBags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082DE20, length 203 bytes (+ int3 padding). Called from seven AI sites
// (0x00926D9F .. 0x00931A71).
// Callees: the const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the owner's interface 0x21, bit 1 of +0x24),
// CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times per element), the non-const LowerBound @ 0x0040F020 (the
// element's interface 0x26, bit 6 of +0x24).
// Label: like the landed sibling 0x0082DD90, `this` is a CThing (the bitset test sits on ecx itself), so the catalog's
// CTCBuyableHouse class is propagated; kept under the catalog leaf on a local CThing model.
// Shape: the 0x0082DD90 loop with a checked out-parameter GetInterface on each element; the count is bumped before the three
// flag bytes are tested (+0x12, +0x13, +0x10; any set returns false), the result is `count > 0` (`setg`).
class CTCBase;
class CBaseObject;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CBaseObject* m_pItem;                           // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

class CThing;

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    T* Get() const { return (T*)GetPItem(); }
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct SThingNode {                                 // list<CIntelligentPointer<CThing>> node
    SThingNode* next;
    SThingNode* prev;
    CIntelligentPointer<CThing> thing;              // +0x08
};

struct CTCThingList_0x21 {                          // the interface-0x21 component
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                           // +0x0c list header node
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                     // +0x91 (bit 0 = dead)

    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    bool CountMoneyBags() const;
};

struct CTCInterface26 {                              // the element's interface-0x26 component
    unsigned char _pad00[0x10];
    bool m_flag10;                                  // +0x10
    bool m_flag11;                                  // +0x11
    bool m_flag12;                                  // +0x12
    bool m_flag13;                                  // +0x13
};

inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !(p.Get()->m_stateFlags & 1);
}

bool CThing::CountMoneyBags() const
{
    CTCThingList_0x21* list;
    CTCInterface26* component;
    if (GetInterface(&list, 0x21)) {
        long count = 0;
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next) {
            if (IsThingAlive(node->thing) && node->thing.Get()->GetInterface(&component, 0x26)) {
                ++count;
                if (component->m_flag12 || component->m_flag13 || component->m_flag10)
                    return false;
            }
        }
        return count > 0;
    }
    return false;
}