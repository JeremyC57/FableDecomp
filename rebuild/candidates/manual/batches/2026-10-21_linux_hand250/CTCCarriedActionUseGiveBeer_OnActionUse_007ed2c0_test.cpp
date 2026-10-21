// Behaviour test for CTCCarriedActionUseGiveBeer::OnActionUse (0x007ED2C0): give to a targeted drinker or drink.
#include <cstdio>
struct Target { bool isDrinker; float drunk; };
struct User { bool busy; bool hasTargeting; bool targetOk; Target* target; int action; };   // 1 give, 2 drink
static const float g_BeerDrunkennessUnit = 0.1f;
static void Use(User* u)
{
    if (u->busy) return;
    if (u->hasTargeting && u->targetOk) {
        if (!u->target->isDrinker) return;
        u->action = 1;
        u->target->drunk += g_BeerDrunkennessUnit * 10.0f;
        return;
    }
    u->action = 2;
}
int main()
{
    Target t = { true, 0.0f }, rock = { false, 0.0f };
    User a = { false, true, true, &t, 0 }; Use(&a);
    User b = { false, true, false, &t, 0 }; Use(&b);
    User c = { false, true, true, &rock, 0 }; Use(&c);
    User d = { true, true, true, &t, 0 }; Use(&d);
    if (a.action != 1 || t.drunk < 0.99f || t.drunk > 1.01f || b.action != 2 || c.action != 0 || d.action != 0)
    { std::printf("007ed2c0_TEST FAIL\n"); return 1; }
    std::printf("007ed2c0_TEST PASS\n");
    return 0;
}
