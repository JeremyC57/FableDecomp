#include <cstdio>
#include <cstring>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CTCBase { public: unsigned char _pad[0x79]; bool m_bPushableByHero; };
class CThingAICreature { public: unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    bool GetInterface(long type, CTCBase** out) {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type) it = map->last;
            *out = it->component; return true;
        }
        return false;
    } };
class CAIBrain { public: void* __vftable; unsigned char _pad[0x14]; CThingAICreature* Creature; };
class CAIStateGroupBase { public: void* __vftable; CAIBrain* PBrain; void SetPushableByHero(bool pushable); };
void CAIStateGroupBase::SetPushableByHero(bool pushable) { CTCBase* component; if (PBrain->Creature->GetInterface(0xaa, &component)) component->m_bPushableByHero = pushable; }
int main()
{
    CTCBase tc[2]; tc[0].m_bPushableByHero = false; tc[1].m_bPushableByHero = false; CInterfaceMap::SEntry entries[2] = { { 5, &tc[1] }, { 0xaa, &tc[0] } };
    CThingAICreature c; std::memset(c.m_presentBits, 0, sizeof(c.m_presentBits)); c.m_interfaces.first = entries; c.m_interfaces.last = entries + 2;
    CAIBrain b; b.Creature = &c; CAIStateGroupBase g; g.PBrain = &b;
    g.SetPushableByHero(true); if (tc[0].m_bPushableByHero) { std::printf("0097e330_TEST FAIL\n"); return 1; }
    c.m_presentBits[5] = 0x400; g.SetPushableByHero(true); if (!tc[0].m_bPushableByHero || tc[1].m_bPushableByHero) { std::printf("0097e330_TEST FAIL\n"); return 1; }
    std::printf("0097e330_TEST PASS\n");
    return 0;
}