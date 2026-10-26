// Behaviour test for 0095ebf0: body copied from src/0095ebf0.cpp (CAIStateGroup_BoastingCrowdMember::OnReplace), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
static int g_getPItem = 0;
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CThing* GetPItem() const { ++g_getPItem; return (CThing*)m_pItem; } };
static int g_removed = 0; static CThing* g_removedCreature = 0; static void* g_removeThis = 0;
struct CTCBoastingCrowd {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {} virtual void v24() {}
    virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {}
    virtual void RemoveCrowdMember(CThing* creature) { ++g_removed; g_removedCreature = creature; g_removeThis = this; }
};
class CAIBrain { public: unsigned char _pad[0x18]; CThing* Creature; };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain; long ScriptAIPriority; void* Name_PStringData; unsigned char Terminated; unsigned char Priority; unsigned char Enabled; unsigned char _pad_0x13;
};
struct CAIStateGroup_BoastingCrowdMember : CAIStateGroupBase {
    CBaseIntelligentPointer PBoaster;
    void OnReplace();
};
void CAIStateGroup_BoastingCrowdMember::OnReplace()
{
    if (PBoaster.GetPItem() != 0 && !PBoaster.GetPItem()->IsDead()) {
        CThing* boaster = PBoaster.GetPItem();
        if (boaster->HasInterface(0x74)) {
            CTCBoastingCrowd* crowd = boaster->GetTC<CTCBoastingCrowd>(0x74);
            crowd->RemoveCrowdMember(PBrain->Creature);
        }
    }
}
int main()
{
    CThing boaster; memset(&boaster, 0, sizeof(boaster)); CTCBoastingCrowd crowd; CThing member;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x74, (CTCBase*)&crowd } };
    boaster.m_interfaces.first = entries; boaster.m_interfaces.last = entries + 2;
    CAIBrain brain; brain.Creature = &member;
    CAIStateGroup_BoastingCrowdMember g; g.PBrain = &brain; g.PBoaster.m_pItem = 0;
    // 1. no boaster
    g.OnReplace(); CHECK(g_getPItem == 1 && g_removed == 0);
    // 2. dead boaster
    g.PBoaster.m_pItem = &boaster; boaster.m_flags91 = 1; g.OnReplace(); CHECK(g_getPItem == 3 && g_removed == 0); boaster.m_flags91 = 0;
    // 3. boaster without a crowd component
    g.OnReplace(); CHECK(g_getPItem == 6 && g_removed == 0);
    // 4. crowd present: the brain's creature is removed from it
    boaster.m_presentBits[0x74 >> 5] = 1UL << (0x74 & 31); g.OnReplace();
    CHECK(g_removed == 1 && g_removedCreature == &member && g_removeThis == &crowd);
    if (fails == 0) printf("0095ebf0_TEST PASS\n"); else printf("0095ebf0_TEST FAIL\n");
    return fails;
}