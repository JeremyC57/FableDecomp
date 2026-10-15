#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CTCBase { long type; };
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
    CInterfaceMap::SEntry m_entries[4];
    long m_entryCount;
    CThing() : m_entryCount(0) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_entries; m_interfaces.last = m_entries; }
    void Add(long type, CTCBase* component)
    {
        m_presentBits[type >> 5] |= (1UL << (type & 31));
        int i = m_entryCount;
        while (i > 0 && m_entries[i - 1].type > type) { m_entries[i] = m_entries[i - 1]; --i; }
        m_entries[i].type = type; m_entries[i].component = component;
        ++m_entryCount; m_interfaces.last = m_entries + m_entryCount;
    }
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
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
};
struct CTCInventory : CTCBase {
    CThing* m_added; bool m_real; int m_calls;
    CTCInventory() : m_added(0), m_real(false), m_calls(0) { }
    void AddItemToInventory(CThing* item, bool real) { m_added = item; m_real = real; ++m_calls; }
};
struct CObjectThing : CThing { bool m_isBeingRemoved; CObjectThing() : m_isBeingRemoved(false) { } };
struct CCreatureActionBase {
    virtual ~CCreatureActionBase() { }
    CBaseIntelligentPointer PCreature;
};
struct CCreatureAction_AddRealObjectToInventory : CCreatureActionBase {
    CBaseIntelligentPointer PObject;
    void PerformAction();
};
void CCreatureAction_AddRealObjectToInventory::PerformAction()
{
    if (PObject.GetPItem()) {
        if (!((CObjectThing*)PObject.GetPItem())->m_isBeingRemoved) {
            CTCInventory* pInventory;
            if (PCreature.GetPItem()->GetInterface(&pInventory, 203))
                pInventory->AddItemToInventory(PObject.GetPItem(), true);
        }
    }
}
int main()
{
    CThing creature; CTCInventory inventory; creature.Add(5, 0); creature.Add(203, &inventory); creature.Add(240, 0);
    CObjectThing object;
    CCreatureAction_AddRealObjectToInventory action;
    action.PCreature.m_pItem = &creature; action.PObject.m_pItem = &object;
    action.PerformAction();
    bool ok = inventory.m_calls == 1 && inventory.m_added == &object && inventory.m_real;
    object.m_isBeingRemoved = true; action.PerformAction();
    ok = ok && inventory.m_calls == 1;
    object.m_isBeingRemoved = false; action.PObject.m_pItem = 0; action.PerformAction();
    ok = ok && inventory.m_calls == 1;
    CThing bare; action.PCreature.m_pItem = &bare; action.PObject.m_pItem = &object; action.PerformAction();
    ok = ok && inventory.m_calls == 1;
    if (!ok) { std::printf("00844b40_TEST FAIL\n"); return 1; }
    std::printf("00844b40_TEST PASS\n");
    return 0;
}
