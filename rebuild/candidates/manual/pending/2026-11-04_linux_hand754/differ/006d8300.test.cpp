// Behaviour test for CTCHeroExperience::AddTrainableStatExperience (0x006D8300): a model of the row.
#include <stdio.h>

struct Hero { bool able; double comboParam, perUnit; float weapon; long stat[3]; long forwarded; };
static void Add(Hero& h, int stat, long amount)
{
    if (!h.able) return;
    long multiplier = (long)(h.comboParam / h.perUnit + 1.0);
    if (multiplier < 1) multiplier = 1;
    long gained = (long)(h.weapon * (multiplier * amount));
    h.stat[stat] += gained;
    if (h.stat[stat] < 0) h.stat[stat] = 0;
    h.forwarded += gained;
}
static int run()
{
    Hero h = { true, 20.0, 10.0, 1.5f, { 0, 0, 0 }, 0 };
    Add(h, 1, 10); int ok = h.stat[1] == 45 && h.forwarded == 45;
    Add(h, 2, -100); ok = ok && h.stat[2] == 0;
    h.able = false; Add(h, 1, 10); ok = ok && h.stat[1] == 45;
    return ok;
}
int main()
{
    int ok = run();
    printf(ok ? "006d8300_TEST PASS\n" : "006d8300_TEST FAIL\n");
    return ok ? 0 : 1;
}
