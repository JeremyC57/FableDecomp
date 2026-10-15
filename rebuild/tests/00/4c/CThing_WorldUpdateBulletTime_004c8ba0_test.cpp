// Behaviour test for CThing::WorldUpdateBulletTime @ 0x004C8BA0: carried objects updated when interface 0x46 is present, then each live update-list TC.
#include <cstdio>
struct CTCBase {
    bool ToBeRemoved; int updates;
    CTCBase() : ToBeRemoved(false), updates(0) { }
    virtual void UpdateBulletTime() { ++updates; }
};
struct CTCCarrying : CTCBase { int carried; CTCCarrying() : carried(0) { } void UpdateCarriedObjects() { ++carried; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct SComponentListNode { SComponentListNode* m_next; SComponentListNode* m_prev; CTCBase* m_data; };
struct CThing {
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    SComponentListNode* UpdateComponentsList;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void WorldUpdateBulletTime()
    {
        CTCCarrying* carrying;
        if (GetInterface(&carrying, 0x46))
            carrying->UpdateCarriedObjects();
        for (SComponentListNode* node = UpdateComponentsList->m_next; node != UpdateComponentsList; node = node->m_next) {
            CTCBase* tc = node->m_data;
            if (!tc->ToBeRemoved)
                tc->UpdateBulletTime();
        }
    }
};
int main()
{
    CTCCarrying carry; CTCBase other, live, dead; dead.ToBeRemoved = true;
    CInterfaceMap::SEntry entries[3] = { { 0x10, &other }, { 0x46, &carry }, { 0x50, &other } };
    SComponentListNode head, n1, n2;
    head.m_next = &n1; n1.m_next = &n2; n2.m_next = &head; head.m_data = 0; n1.m_data = &live; n2.m_data = &dead;
    CThing t; for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0;
    t.UsingComponents[0x46 >> 5] = 1UL << (0x46 & 31);
    t.Components.first = entries; t.Components.last = entries + 3; t.UpdateComponentsList = &head;
    t.WorldUpdateBulletTime();
    if (carry.carried != 1 || live.updates != 1 || dead.updates != 0) { std::printf("004c8ba0_TEST FAIL\n"); return 1; }
    t.UsingComponents[0x46 >> 5] = 0;
    t.WorldUpdateBulletTime();
    if (carry.carried != 1 || live.updates != 2) { std::printf("004c8ba0_TEST FAIL\n"); return 1; }
    std::printf("004c8ba0_TEST PASS\n");
    return 0;
}