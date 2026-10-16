// behaviour test for the cover groups' projectile-weapon runnable checks (0x00979e90)
#include <cstdio>
struct CThing { bool dead; bool incap; bool hasCarry; CThing* primary; bool hasWeapon; bool projectile; bool melee; };
static bool alive(CThing* t) { return t && !t->dead; }
static bool ProjectileInPrimary(CThing* t)
{
    if (t->hasCarry) { CThing* w = t->primary; if (w) { if (w->hasWeapon && w->projectile) return true; } }
    return false;
}
static bool leafOk = true;
static bool Check(CThing* target, CThing* cover)
{
    const char* a = "00979e90";
    if (a[5] == 'e') return !target->incap && ProjectileInPrimary(target) && leafOk;          // IsRunnableLeaf
    if (a[5] == 'f') return alive(target) && !target->incap && ProjectileInPrimary(target);   // AdvanceInCover::IsStillRunnable
    return alive(target) && alive(cover) && !target->melee && ProjectileInPrimary(target);    // TakeCover::IsStillRunnable
}
int main()
{
    CThing bow = { false, false, false, 0, true, true, false };
    CThing sword = { false, false, false, 0, true, false, false };
    CThing t = { false, false, true, &bow, false, false, false };
    CThing cover = { false };
    bool ok = Check(&t, &cover);
    t.primary = &sword; ok = ok && !Check(&t, &cover);
    t.primary = 0; ok = ok && !Check(&t, &cover);
    t.primary = &bow; t.incap = true;
    bool r = Check(&t, &cover);
    const char* a = "00979e90";
    ok = ok && (a[5] == '9' ? r : !r);
    if (!ok) { std::printf("00979e90_TEST FAIL\n"); return 1; }
    std::printf("00979e90_TEST PASS\n");
    return 0;
}
