// Behaviour test for CTCBuyableHouse::CountMoneyBags (0x006C04D0).
#include <cstdio>

struct CTCBase;
struct CThing;
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { T* Get() const { return (T*)GetPItem(); } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct SThingNode { SThingNode* next; SThingNode* prev; CIntelligentPointer<CThing> thing; };
struct CTCThingList_0x21 { SThingNode* m_head; };
struct CThing {
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char m_stateFlags;
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p) { return p.Get() && !(p.Get()->m_stateFlags & 1); }
class CTCBuyableHouse {
public:
    CThing* ParentThing;
    long CountMoneyBags() const
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
};
static void Clear(CThing& t) { for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_stateFlags = 0; t.m_interfaces.first = t.m_interfaces.last = 0; }

int main()
{
    CThing bags[4];
    for (int i = 0; i < 4; ++i) { Clear(bags[i]); bags[i].m_presentBits[6] = 1UL << 12; }
    bags[2].m_presentBits[6] = 0;            // not a money bag
    bags[3].m_stateFlags = 1;                // dead
    SThingNode head; SThingNode nodes[5];
    SThingNode* prev = &head;
    for (int i = 0; i < 5; ++i) { prev->next = &nodes[i]; nodes[i].prev = prev; nodes[i].thing.m_pItem = i < 4 ? &bags[i] : 0; prev = &nodes[i]; }
    prev->next = &head; head.prev = prev;
    CTCThingList_0x21 list; list.m_head = &head;
    CInterfaceMap::SEntry entries[1] = { { 0x21, (CTCBase*)&list } };
    CThing owner; Clear(owner); owner.m_presentBits[1] = 1UL << 1; owner.m_interfaces.first = entries; owner.m_interfaces.last = entries + 1;
    CTCBuyableHouse house; house.ParentThing = &owner;
    bool ok = house.CountMoneyBags() == 2;
    owner.m_presentBits[1] = 0;
    ok = ok && house.CountMoneyBags() == 0;
    if (!ok) { std::printf("006c04d0_TEST FAIL\n"); return 1; }
    std::printf("006c04d0_TEST PASS\n");
    return 0;
}