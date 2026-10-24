#include <cstdio>
// Behaviour test for NCreatureAction::CActionEventRemoveObject::Dispatch (retail 0x00871A10): with a carrying component
// (interface 70) and a positive hand slot, the thing in that slot is removed and killed if alive.
struct CTCBase { int id; };
struct CCharString { const char* Text; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    int killed;
    void Kill(bool immediately) { killed = immediately ? 2 : 1; }
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
static CThing* g_carried;
static long g_removedSlot;
struct CTCCarrying : CTCBase {
    CThing* RemoveThingInCarrySlot(long slot, bool drop) { g_removedSlot = drop ? -slot : slot; return g_carried; }
};
struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) { return name.Text[0] == 'L' ? 3 : 0; }
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
};
struct CActionEventRemoveObject {
    CThing* PThing;
    CCharString Hand;
    CThing* GetThing() const { return PThing; }
    void Dispatch()
    {
        CTCCarrying* carrying;
        if (GetThing()->GetInterface(&carrying, 70)) {
            long slot = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(Hand);
            if (slot > 0) {
                CThing* thing = carrying->RemoveThingInCarrySlot(slot, false);
                if (thing != 0 && !(thing->m_flags91 & 1))
                    thing->Kill(true);
            }
        }
    }
};

int main()
{
    CTCCarrying carrying;
    carrying.id = 70;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 9; entries[0].component = 0;
    entries[1].type = 70; entries[1].component = &carrying;
    entries[2].type = 0; entries[2].component = 0;
    CThing owner;
    unsigned int w = 0;
    while (w < 9) { owner.m_presentBits[w] = 0; ++w; }
    owner.m_presentBits[70 >> 5] |= 1UL << (70 & 31);
    owner.m_interfaces.first = entries;
    owner.m_interfaces.last = entries + 2;
    owner.m_flags91 = 0;
    owner.killed = 0;
    CThing item;
    item.m_flags91 = 0;
    item.killed = 0;
    g_carried = &item;
    g_removedSlot = 0;
    CActionEventRemoveObject e;
    e.PThing = &owner;
    e.Hand.Text = "LEFT";
    e.Dispatch();
    int pass = item.killed == 2 && g_removedSlot == 3;
    item.killed = 0;
    item.m_flags91 = 1;
    e.Dispatch();
    pass = pass && item.killed == 0;
    item.m_flags91 = 0;
    g_removedSlot = 0;
    e.Hand.Text = "RIGHT";
    e.Dispatch();
    pass = pass && item.killed == 0 && g_removedSlot == 0;
    e.Hand.Text = "LEFT";
    owner.m_presentBits[70 >> 5] = 0;
    e.Dispatch();
    pass = pass && item.killed == 0 && g_removedSlot == 0;
    if (!pass) { std::printf("00871a10_TEST FAIL\n"); return 1; }
    std::printf("00871a10_TEST PASS\n");
    return 0;
}