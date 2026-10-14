#include <cstdio>
// Model of CAIStateGroup_WorkAtSchool::OnReplace @ 0x00942C10: clear the village's teacher flag through the villager component.
struct CTCVillage { bool m_teacherPresent; };
struct CTCVillager { CTCVillage* village; };
static CTCVillage g_village; static CTCVillager g_villager; static bool g_hasVillager; static int g_lookups;
CTCVillage* GFGetPVillageInterface(CTCVillager* v) { ++g_lookups; return v->village; }
struct Creature {
    unsigned long bits[9]; CTCVillager* comp;
    bool GetInterface(long type, CTCVillager** out) { if (bits[type / 32] & (1 << (type % 32))) { *out = comp; return true; } return false; }
};
struct Group {
    Creature* creature;
    void OnReplace()
    {
        CTCVillager* villager;
        if (creature->GetInterface(0x23, &villager)) {
            if (GFGetPVillageInterface(villager) != 0) GFGetPVillageInterface(villager)->m_teacherPresent = false;
        }
    }
};
int main()
{
    Creature c = {}; c.comp = &g_villager; g_villager.village = &g_village; g_village.m_teacherPresent = true; g_lookups = 0;
    Group g; g.creature = &c;
    g.OnReplace();
    if (!g_village.m_teacherPresent || g_lookups != 0) { std::printf("00942c10_TEST FAIL\n"); return 1; }
    c.bits[1] |= 8;
    g.OnReplace();
    if (g_village.m_teacherPresent || g_lookups != 2) { std::printf("00942c10_TEST FAIL\n"); return 1; }
    g_villager.village = 0; g_village.m_teacherPresent = true; g.OnReplace();
    if (!g_village.m_teacherPresent || g_lookups != 3) { std::printf("00942c10_TEST FAIL\n"); return 1; }
    std::printf("00942c10_TEST PASS\n"); return 0;
}
