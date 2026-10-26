// Behaviour test for 0082de20: body copied from src/0082de20.cpp (CThing::CountMoneyBags), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase { public: };
class CBaseObject;
struct CBaseIntelligentPointer
{
    virtual void v00() {}
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const { return m_pItem; }
};
class CThing;
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    T* Get() const { return (T*)GetPItem(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;
    SEntry* LowerBound(const long* type);
};
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
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    return ((const CInterfaceMap*)this)->LowerBound(type);
}
struct SThingNode {
    SThingNode* next;
    SThingNode* prev;
    CIntelligentPointer<CThing> thing;
};
struct CTCThingList_0x21 : CTCBase {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_stateFlags;
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
struct CTCInterface26 : CTCBase {
    unsigned char _pad00[0x10];
    bool m_flag10;
    bool m_flag11;
    bool m_flag12;
    bool m_flag13;
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
static CThing g_owner, g_items[4]; static CInterfaceMap::SEntry g_ownerEntries[2], g_itemEntries[4][2]; static CTCThingList_0x21 g_list; static CTCInterface26 g_bags[4];
static SThingNode g_head, g_nodes[4];
static void MakeItem(int i, bool hasBag, bool dead)
{
    CThing& t = g_items[i]; memset(&t, 0, sizeof(t)); t.m_stateFlags = dead ? 1 : 0;
    g_itemEntries[i][0].type = 0x26; g_itemEntries[i][0].component = &g_bags[i]; g_itemEntries[i][1].type = 0x40;
    t.m_interfaces.first = g_itemEntries[i]; t.m_interfaces.last = g_itemEntries[i] + (hasBag ? 2 : 1);
    if (hasBag) t.m_presentBits[1] |= 1UL << (0x26 & 31);
    memset(&g_bags[i], 0, sizeof(g_bags[i]));
}
static void Link(int n)
{
    g_head.next = n ? &g_nodes[0] : &g_head;
    for (int i = 0; i < n; ++i) { g_nodes[i].next = (i + 1 < n) ? &g_nodes[i + 1] : &g_head; g_nodes[i].thing.m_pItem = (CBaseObject*)&g_items[i]; }
}
int main()
{
    memset(&g_owner, 0, sizeof(g_owner));
    g_ownerEntries[0].type = 0x10; g_ownerEntries[1].type = 0x21; g_ownerEntries[1].component = &g_list;
    g_owner.m_interfaces.first = g_ownerEntries; g_owner.m_interfaces.last = g_ownerEntries + 2; g_owner.m_presentBits[1] |= 1UL << (0x21 & 31);
    g_list.m_head = &g_head;
    // 1: two live bags with clear flags, one dead bag, one non-bag -> true
    MakeItem(0, true, false); MakeItem(1, false, false); MakeItem(2, true, true); MakeItem(3, true, false); Link(4);
    CHECK(g_owner.CountMoneyBags());
    // 2: a bag with flag 0x12 set -> false
    g_bags[3].m_flag12 = true; CHECK(!g_owner.CountMoneyBags());
    g_bags[3].m_flag12 = false; g_bags[0].m_flag10 = true; CHECK(!g_owner.CountMoneyBags());
    g_bags[0].m_flag10 = false; g_bags[0].m_flag13 = true; CHECK(!g_owner.CountMoneyBags());
    g_bags[0].m_flag13 = false; g_bags[0].m_flag11 = true; CHECK(g_owner.CountMoneyBags());
    // 3: only dead or non-bag things -> false
    MakeItem(0, true, true); MakeItem(1, false, false); Link(2); CHECK(!g_owner.CountMoneyBags());
    // 4: a null pointer in the list, empty list
    g_nodes[0].thing.m_pItem = 0; CHECK(!g_owner.CountMoneyBags()); Link(0); CHECK(!g_owner.CountMoneyBags());
    // 5: no list component
    g_owner.m_presentBits[1] = 0; MakeItem(0, true, false); Link(1); CHECK(!g_owner.CountMoneyBags());
    if (fails == 0) printf("0082de20_TEST PASS\n"); else printf("0082de20_TEST FAIL\n");
    return fails;
}