// behaviour test for 0x008B9BF0 CCreatureAction_ReleaseWillSummonSpell::FinishAction() (model; the creature, item and action are recording stubs)
#include <cstdio>
#include <cstring>
static int g_base = 0, g_set = 0; static long g_prio = 0, g_time = 0; static char g_anim[64];
struct CThing { unsigned char dead; bool SetCurrentAction(const void*) { ++g_set; return true; } };
struct CUnsheathe { CUnsheathe(CThing*, const char* anim, CThing*, long prio, long timeout, void*) { std::strcpy(g_anim, anim); g_prio = prio; g_time = timeout; } };
struct CAction {
    CThing* creature; CThing* item;
    void BaseFinish() { ++g_base; }
    void FinishAction()
    {
        BaseFinish();
        if (item != 0 && !(item->dead & 1)) {
            CUnsheathe u(creature, "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE", item, 100, 250, 0);
            creature->SetCurrentAction(&u);
        }
    }
};
int main()
{
    CThing c = { 0 }, i = { 0 };
    CAction a; a.creature = &c; a.item = &i;
    a.FinishAction();
    int ok = g_base == 1 && g_set == 1 && g_prio == 100 && g_time == 250 && std::strcmp(g_anim, "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE") == 0;
    i.dead = 1; a.FinishAction();
    ok = ok && g_base == 2 && g_set == 1;
    a.item = 0; a.FinishAction();
    ok = ok && g_base == 3 && g_set == 1;
    if (!ok) { std::printf("008b9bf0_TEST FAIL\n"); return 1; }
    std::printf("008b9bf0_TEST PASS\n");
    return 0;
}
