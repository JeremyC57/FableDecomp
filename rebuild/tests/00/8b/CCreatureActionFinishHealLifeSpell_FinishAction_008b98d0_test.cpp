// behaviour test for 0x008B98D0 CCreatureAction_FinishHealLifeSpell::FinishAction (re-equip the removed item when it is alive)
#include <cstdio>
#include <cstring>
class CThing { public: unsigned char Dead : 1; bool IsDead() const { return Dead; } };
static int g_base; static const char* g_anim; static CThing* g_item; static long g_a, g_b, g_c;
struct Action {
    CThing* PItemRemoved;
    void FinishAction()
    {
        ++g_base;
        if (PItemRemoved && !PItemRemoved->IsDead()) {
            g_anim = "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE"; g_item = PItemRemoved; g_a = 100; g_b = 250; g_c = 0;
        }
    }
};
int main()
{
    CThing sword = { 0 }, broken = { 1 };
    Action a = { &sword }, b = { &broken }, c = { 0 };
    b.FinishAction(); c.FinishAction();
    bool ok = g_base == 2 && g_anim == 0;
    a.FinishAction();
    ok = ok && g_base == 3 && g_item == &sword && std::strcmp(g_anim, "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE") == 0 && g_a == 100 && g_b == 250 && g_c == 0;
    std::printf(ok ? "008b98d0_TEST PASS\n" : "008b98d0_TEST FAIL\n");
    return ok ? 0 : 1;
}