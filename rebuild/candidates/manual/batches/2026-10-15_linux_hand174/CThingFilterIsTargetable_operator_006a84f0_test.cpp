#include <cstdio>
struct T { bool targetable, z; bool IsTargetable() const { return targetable; } bool IsZTargetable() const { return z; } };
struct Thing { bool has; T* tc; };
struct F { bool ZTargeting, UsingLightning;
    bool operator()(Thing* t) const {
        if (t->has) { T* g = t->tc; if (g->IsTargetable() && (!ZTargeting || g->IsZTargetable() || UsingLightning)) return true; }
        return false; } };
int main()
{
    T tc = { true, false };
    Thing t = { true, &tc };
    F f = { false, false };
    bool ok = f(&t);
    f.ZTargeting = true; ok = ok && !f(&t);
    f.UsingLightning = true; ok = ok && f(&t);
    f.UsingLightning = false; tc.z = true; ok = ok && f(&t);
    tc.targetable = false; ok = ok && !f(&t);
    t.has = false; tc.targetable = true; ok = ok && !f(&t);
    if (!ok) { std::printf("006a84f0_TEST FAIL\n"); return 1; }
    std::printf("006a84f0_TEST PASS\n");
    return 0;
}
