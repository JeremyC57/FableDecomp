#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 TUID;
#else
typedef unsigned long long TUID;
#endif
struct CThing { int id; };
static int g_live = 0;
namespace N_0050f3f0 { namespace N_0050f440 { struct CIntelligentPointerCreature {
    CThing* m_pItem;
    CIntelligentPointerCreature(CThing* t) : m_pItem(t) { ++g_live; }
    ~CIntelligentPointerCreature() { --g_live; }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& o) { m_pItem = o.m_pItem; return *this; }
}; } using N_0050f440::CIntelligentPointerCreature; } using N_0050f3f0::CIntelligentPointerCreature;
struct CTCDummyVillager {
    CIntelligentPointerCreature Creature; TUID CreatureUID;
    CTCDummyVillager(CThing* t) : Creature(t), CreatureUID(0) { }
    void Evict() { Creature = (CThing*)0; CreatureUID = 0; }
};
int main()
{
    CThing c;
    CTCDummyVillager d(&c); d.CreatureUID = 0x12345678; d.CreatureUID <<= 4;
    d.Evict();
    if (d.Creature.m_pItem != 0 || d.CreatureUID != 0 || g_live != 1) { std::printf("0065f01d_TEST FAIL\n"); return 1; }
    std::printf("0065f01d_TEST PASS\n");
    return 0;
}