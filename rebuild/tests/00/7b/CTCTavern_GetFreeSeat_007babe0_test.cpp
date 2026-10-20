// Behaviour test for 007babe0: body copied from src/007babe0.cpp (CTCTavern::GetFreeSeat), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CThing;
struct CTCBase { };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
    const SEntry* LowerBound(const long* type) const;
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    return ((CInterfaceMap*)this)->LowerBound(type);
}
static unsigned long g_typeFlags = 0; static int g_typeFlagCalls = 0;
class CThing {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual void v17() {}
    virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {} virtual void v29() {}
    virtual void v30() {} virtual void v31() {} virtual void v32() {} virtual void v33() {} virtual void v34() {} virtual void v35() {}
    virtual void v36() {} virtual void v37() {} virtual void v38() {} virtual void v39() {} virtual void v40() {} virtual void v41() {}
    virtual void v42() {} virtual void v43() {} virtual void v44() {} virtual void v45() {} virtual void v46() {} virtual void v47() {}
    virtual unsigned long GetThingTypeFlags() const { ++g_typeFlagCalls; return m_typeFlags; }
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;
    unsigned long m_typeFlags;
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CObjectNode { CObjectNode* _M_next; CObjectNode* _M_prev; CBaseIntelligentPointer _M_data; };
struct CTCThingOwner : CTCBase {
    virtual void v00() {}
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    unsigned char _pad0a[2];
    CObjectNode* ObjectList;
};
static int g_apCalls = 0;
struct CTCUserActionPointBase : CTCBase {
    bool Free;
    bool IsActionPointFree() { ++g_apCalls; return Free; }
};
struct CTCTavern {
    virtual void v00() {}
    CThing* ParentThing;
    CThing* GetFreeSeat(long unused);
};
CThing* CTCTavern::GetFreeSeat(long unused)
{
    CTCThingOwner* owner;
    ParentThing->GetInterface(0x21, &owner);
    for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
        CBaseIntelligentPointer& object = node->_M_data;
        if (object.GetPItem() && !(object.GetPItem()->m_flags91 & 1)) {
            CThing* seat = object.GetPItem();
            if (((unsigned char)(seat->GetThingTypeFlags() >> 16) & 1) && seat->HasTC(0x2d)) {
                if (((CTCUserActionPointBase*)seat->GetTC(0x2d))->IsActionPointFree())
                    return seat;
            }
        }
    }
    return 0;
}
static CThing g_tavern, g_things[5]; static CInterfaceMap::SEntry g_tEntries[2], g_entries[5][2]; static CTCThingOwner g_owner; static CTCUserActionPointBase g_aps[5];
static CObjectNode g_head, g_nodes[5];
static void MakeSeat(int i, bool dead, bool seatFlag, bool hasAP, bool free)
{
    CThing& t = g_things[i]; t.m_presentBits[0] = t.m_presentBits[1] = 0; t.m_flags91 = dead ? 1 : 0; t.m_typeFlags = seatFlag ? 0x10000 : 0x20000;
    g_entries[i][0].type = 0x05; g_entries[i][1].type = 0x2d; g_entries[i][1].component = &g_aps[i];
    t.m_interfaces.first = g_entries[i]; t.m_interfaces.last = g_entries[i] + (hasAP ? 2 : 1);
    if (hasAP) t.m_presentBits[1] |= 1UL << (0x2d & 31);
    g_aps[i].Free = free;
    g_nodes[i]._M_data.m_pItem = &t;
}
int main()
{
    g_tEntries[0].type = 0x21; g_tEntries[0].component = &g_owner; g_tEntries[1].type = 0x30;
    g_tavern.m_interfaces.first = g_tEntries; g_tavern.m_interfaces.last = g_tEntries + 2; g_tavern.m_presentBits[0] = g_tavern.m_presentBits[1] = 0; g_tavern.m_presentBits[1] |= 1UL << (0x21 & 31);
    g_owner.ObjectList = &g_head; g_head._M_next = &g_nodes[0];
    for (int i = 0; i < 5; ++i) g_nodes[i]._M_next = (i + 1 < 5) ? &g_nodes[i + 1] : &g_head;
    MakeSeat(0, true, true, true, true);     // dead
    MakeSeat(1, false, false, true, true);   // not a seat type
    MakeSeat(2, false, true, false, true);   // seat without an action point
    MakeSeat(3, false, true, true, false);   // occupied
    MakeSeat(4, false, true, true, true);    // free
    CTCTavern tavern; tavern.ParentThing = &g_tavern;
    g_typeFlagCalls = 0; g_apCalls = 0;
    CHECK(tavern.GetFreeSeat(7) == &g_things[4]); CHECK(g_typeFlagCalls == 4); CHECK(g_apCalls == 2);
    g_aps[4].Free = false; g_apCalls = 0; CHECK(tavern.GetFreeSeat(7) == 0); CHECK(g_apCalls == 2);
    g_nodes[3]._M_data.m_pItem = 0; g_apCalls = 0; CHECK(tavern.GetFreeSeat(7) == 0); CHECK(g_apCalls == 1);
    g_head._M_next = &g_head; g_typeFlagCalls = 0; CHECK(tavern.GetFreeSeat(7) == 0); CHECK(g_typeFlagCalls == 0);
    if (fails == 0) printf("007babe0_TEST PASS\n"); else printf("007babe0_TEST FAIL\n");
    return fails;
}