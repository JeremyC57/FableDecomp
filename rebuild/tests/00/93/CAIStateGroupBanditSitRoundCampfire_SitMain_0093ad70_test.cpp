// Behaviour test for 0x0093AD70 CAIStateGroup_BanditSitRoundCampfire::SitMain (model copy; activities and RNG are stubs).
#include <cstdio>
struct CTCBase;
struct CInlineInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
struct CThingI {
    unsigned long m_presentBits[9];
    CInlineInterfaceMap m_interfaces;
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            CInlineInterfaceMap::SEntry* last = m_interfaces.last;
            CInlineInterfaceMap::SEntry* it = m_interfaces.first;
            long len = last - it;
            while (len > 0) {
                long half = len / 2;
                if (it[half].type < type) {
                    it += half + 1;
                    len = len - half - 1;
                }
                else
                    len = half;
            }
            if (it == last || type < it->type)
                it = last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCCampfireSitter { bool AtCampfire; };
struct CThing;
struct CAIBrain { CThing* PCreature; };
static float g_rolls[8]; static float g_ranges[8]; static int g_roll = 0;
float GFFloatRandom(float range, unsigned long*) { g_ranges[g_roll] = range; return g_rolls[g_roll++]; }
static char g_log[16]; static int g_logn = 0; static int g_budget = 0;
struct CAIStateGroup_BanditSitRoundCampfire {
    CAIBrain* PBrain;
    unsigned long seed;
    unsigned long* GetPWorldSeed() { return &seed; }
    bool Act(char c) { g_log[g_logn++] = c; return --g_budget > 0; }
    bool Idle() { return Act('I'); }
    bool Eat() { return Act('E'); }
    bool Singsong() { return Act('S'); }
    bool Drink() { return Act('D'); }
    bool SitMain();
};
bool CAIStateGroup_BanditSitRoundCampfire::SitMain()
{
    bool ok;
    do {
        CTCCampfireSitter* sitter;
        if (((CThingI*)PBrain->PCreature)->GetInterface(&sitter, 0xc3) && sitter->AtCampfire) {
            float r = GFFloatRandom(3.0f, GetPWorldSeed());
            if (r < 1.0f)
                ok = Idle();
            else if (r - 1.0f < 1.0f)
                ok = Drink();
            else
                ok = Singsong();
        }
        else {
            float r = GFFloatRandom(5.0f, GetPWorldSeed());
            if (r < 2.0f)
                ok = Idle();
            else if (r - 2.0f < 1.0f)
                ok = Eat();
            else
                ok = Drink();
        }
    } while (ok);
    return ok;
}

int main()
{
    CTCCampfireSitter sitter; sitter.AtCampfire = true;
    CInlineInterfaceMap::SEntry entries[3] = { { 0x10, 0 }, { 0xc3, (CTCBase*)&sitter }, { 0xd0, 0 } };
    CThingI creature; for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_presentBits[0xc3 >> 5] = 1UL << (0xc3 & 31);
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3;
    CAIBrain brain; brain.PCreature = (CThing*)&creature;
    CAIStateGroup_BanditSitRoundCampfire group; group.PBrain = &brain;
    g_rolls[0] = 0.5f; g_rolls[1] = 1.5f; g_rolls[2] = 2.5f; g_budget = 3;
    if (group.SitMain() || g_logn != 3 || g_log[0] != 'I' || g_log[1] != 'D' || g_log[2] != 'S' || g_ranges[0] != 3.0f) {
        std::printf("0093ad70_TEST FAIL\n"); return 1;
    }
    sitter.AtCampfire = false; g_roll = 0; g_logn = 0; g_budget = 3;
    g_rolls[0] = 1.0f; g_rolls[1] = 2.5f; g_rolls[2] = 4.0f;
    if (group.SitMain() || g_log[0] != 'I' || g_log[1] != 'E' || g_log[2] != 'D' || g_ranges[0] != 5.0f) {
        std::printf("0093ad70_TEST FAIL\n"); return 1;
    }
    std::printf("0093ad70_TEST PASS\n");
    return 0;
}