#include <cstdio>
struct CBitset { unsigned long bits[8]; bool test(unsigned long p) const { return (bits[p >> 5] >> (p & 31)) & 1; } };
struct CThingAICreature { CBitset TypeBits; };
struct CVillageTaskCarryCrate {
    const CThingAICreature* performer;
    virtual ~CVillageTaskCarryCrate() {}
    virtual bool IsThingPerformingTask(const CThingAICreature* v) { return v == performer; }
};
struct SCrateTaskNode { SCrateTaskNode* next; SCrateTaskNode* prev; CVillageTaskCarryCrate* Object; };
struct CTCVillage {
    SCrateTaskNode* CrateTasks;
    bool IsVillagerPerformingVillageTask(const CThingAICreature* villager)
    {
        if (villager->TypeBits.test(0xB4)) return true;
        for (SCrateTaskNode* n = CrateTasks->next; n != CrateTasks; n = n->next)
            if (n->Object->IsThingPerformingTask(villager)) return true;
        return false;
    }
};
int main()
{
    CThingAICreature a, b, c;
    for (int i = 0; i < 8; ++i) { a.TypeBits.bits[i] = 0; b.TypeBits.bits[i] = 0; c.TypeBits.bits[i] = 0; }
    c.TypeBits.bits[0xB4 >> 5] = 1UL << (0xB4 & 31);
    CVillageTaskCarryCrate t1, t2; t1.performer = 0; t2.performer = &b;
    SCrateTaskNode head, n1, n2; head.next = &n1; n1.next = &n2; n2.next = &head; n1.Object = &t1; n2.Object = &t2;
    CTCVillage v; v.CrateTasks = &head;
    if (v.IsVillagerPerformingVillageTask(&a) || !v.IsVillagerPerformingVillageTask(&b) || !v.IsVillagerPerformingVillageTask(&c)) { std::printf("00659104_TEST FAIL\n"); return 1; }
    std::printf("00659104_TEST PASS\n");
    return 0;
}
