// CTCBuyableHouse::CountMoneyBags() const -- count the live things in the house's interface-0x21 thing list that carry the money-bag interface (0xCC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C04D0, length 164 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x21 = bit 1 of
// CThing+0x24), CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times per element: null test, the dead bit
// +0x91 & 1, the interface-0xCC bit = word 6 (+0x38) & 0x1000).
// Shape: the component-side twin of the landed thing-side CountMoneyBags (0x0082DD90): `this` is the CTCBuyableHouse
// component (ParentThing at +4); the component owns a list<CIntelligentPointer<CThing>> whose header node pointer is at +0x0C
// (end re-read every pass); the interface mask 0x1000 is hoisted into ebx and the count lives in the frame.
struct CTCBase;
struct CBaseObject;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CBaseObject* m_pItem;                           // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

struct CThing;

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer
{
    T* Get() const { return (T*)GetPItem(); }
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct SThingNode {                                 // list<CIntelligentPointer<CThing>> node
    SThingNode* next;
    SThingNode* prev;
    CIntelligentPointer<CThing> thing;              // +0x08
};

struct CTCThingList_0x21 {                          // the interface-0x21 component
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                           // +0x0c list header node
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                     // +0x91 (bit 0 = dead)

    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
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

inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !(p.Get()->m_stateFlags & 1);
}

class CTCBuyableHouse {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    long CountMoneyBags() const;
};

long CTCBuyableHouse::CountMoneyBags() const
{
    long count = 0;
    CTCThingList_0x21* list;
    if (ParentThing->GetInterface(&list, 0x21)) {
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next)
            if (IsThingAlive(node->thing) && node->thing.Get()->HasInterface(0xcc))
                ++count;
    }
    return count;
}
