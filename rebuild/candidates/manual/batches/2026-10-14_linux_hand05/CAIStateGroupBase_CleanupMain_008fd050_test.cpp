#include <cstdio>
#include <cstring>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static int g_removed = 0, g_replace = 0, g_cleanup = 0;
struct CTCBase { void OnStateGroupRemoved() { ++g_removed; } };
struct CThingAICreature { unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); } };
struct CAIBrain { void* __vftable; unsigned char _pad[0x14]; CThingAICreature* Creature; };
struct CAIStateGroupBase { CAIBrain* PBrain;
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() { ++g_cleanup; } virtual void OnReplace() { ++g_replace; } void CleanupMain(); };
void CAIStateGroupBase::CleanupMain()
{
    OnReplace(); Cleanup();
    CThingAICreature* creature = PBrain->Creature;
    if (creature->HasInterface(7)) {
        long type = 7; CInterfaceMap* map = &creature->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || 7 < it->type) it = map->last;
        it->component->OnStateGroupRemoved();
    }
}
int main()
{
    CTCBase tc[2]; CInterfaceMap::SEntry entries[3] = { { 3, &tc[1] }, { 7, &tc[0] }, { 9, &tc[1] } };
    CThingAICreature c; std::memset(c.m_presentBits, 0, sizeof(c.m_presentBits)); c.m_interfaces.first = entries; c.m_interfaces.last = entries + 3;
    CAIBrain b; b.Creature = &c; CAIStateGroupBase g; g.PBrain = &b;
    g.CleanupMain(); if (g_replace != 1 || g_cleanup != 1 || g_removed != 0) { std::printf("008fd050_TEST FAIL\n"); return 1; }
    c.m_presentBits[0] = 0x80; g.CleanupMain(); if (g_replace != 2 || g_cleanup != 2 || g_removed != 1) { std::printf("008fd050_TEST FAIL\n"); return 1; }
    std::printf("008fd050_TEST PASS\n");
    return 0;
}
