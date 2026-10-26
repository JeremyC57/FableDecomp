// Behaviour test for CAIStateGroup_WorkBase::IsInWorkHours @ 0x0094bef0: interface 0x90 -> GetShopTC -> CTCShop::IsShopKeeperAvailable.
#include <cstdio>
class CTCBase { public: };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
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
class CTCShop { public: bool m_answer; bool IsShopKeeperAvailable() { return m_answer; } };
struct CComponent : CTCBase { CTCShop* m_pOwner; };
CTCShop* __fastcall GetShopTC(CComponent* c) { return c->m_pOwner; }
class CAIBrain { public: unsigned char _pad[0x18]; CThing* Creature; };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() { } CAIBrain* PBrain; long a; void* b; unsigned char c, d, e, f; };
struct CAIStateGroup_WorkBase : CAIStateGroupBase {
    bool IsInWorkHours()
    {
        CComponent* pComponent;
        if (PBrain->Creature->GetInterface(&pComponent, 0x90)) {
            CTCShop* pOwner = GetShopTC(pComponent);
            if (pOwner != 0) { if (pOwner->IsShopKeeperAvailable()) return true; }
        }
        return false;
    }
};
int main()
{
    CTCShop owner; owner.m_answer = true;
    CComponent comp; comp.m_pOwner = &owner;
    CInterfaceMap::SEntry entries[2]; entries[0].type = 0x90; entries[0].component = &comp; entries[1].type = 0x200; entries[1].component = 0;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CAIBrain brain; brain.Creature = &thing;
    CAIStateGroup_WorkBase g; g.PBrain = &brain;
    bool ok = g.IsInWorkHours() == false;                                   // interface bit clear
    thing.m_presentBits[0x90 >> 5] |= 1UL << (0x90 & 31);
    if (g.IsInWorkHours() != true) ok = false;                               // present, owner says yes
    owner.m_answer = false; if (g.IsInWorkHours() != false) ok = false;     // owner says no
    comp.m_pOwner = 0; owner.m_answer = true; if (g.IsInWorkHours() != false) ok = false;   // no owner
    if (!ok) { std::printf("0094bef0_TEST FAIL\n"); return 1; }
    std::printf("0094bef0_TEST PASS\n");
    return 0;
}