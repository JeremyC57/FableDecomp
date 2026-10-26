// CTCBuyableHouse::CountMoneyBags() -- catalog label; `this` is the thing: true when its interface-0x21 component's thing list holds at least one live thing that carries interface 0x26
// Leaf: CountMoneyBags
//
// Retail 0x0082DD90, 132 bytes (plain `ret`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound @ 0x004365B0, CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times per
// element: the null test, the dead bit +0x91 & 1, the interface-0x26 bit word 1 & 0x40).
// Shape: `this` is a CThing (the bitset test `test byte [ecx+24h],2` = interface 0x21 sits on ecx
// itself), so the catalog's CTCBuyableHouse label is propagated; the body is kept under the
// catalog leaf name on a local CThing model. The component (in ebp) owns a
// list<CIntelligentPointer<CThing>> whose header node pointer is at +0x0c (end re-read every pass,
// begin read once). The count lives in ebx and the result is `count > 0` (`setg`); a clear
// interface bit takes the separate `xor al,al` exit.

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
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
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

class CThing { public:
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
    bool CountMoneyBags();
};

inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !(p.Get()->m_stateFlags & 1);
}

bool CThing::CountMoneyBags()
{
    CTCThingList_0x21* list;
    if (GetInterface(&list, 0x21)) {
        long count = 0;
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next)
            if (IsThingAlive(node->thing) && node->thing.Get()->HasInterface(0x26))
                ++count;
        return count > 0;
    }
    return false;
}