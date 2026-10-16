#include <cstdio>
struct TC { void* m_pActive; };
struct Phys { bool Moving; };
struct Thing { TC* tc47; Phys* PhysicsTC; bool baseUpdated;
    bool GetInterface(TC** o) const { if (tc47) { *o = tc47; return true; } return false; }
    bool BaseIsFrameUpdated() const { return baseUpdated; }
    bool IsFrameUpdated() const { TC* t; bool active = GetInterface(&t) && t->m_pActive != 0; return BaseIsFrameUpdated() || active || PhysicsTC->Moving; } };
int main()
{
    int x;
    TC tc = { 0 };
    Phys p = { false };
    Thing t = { &tc, &p, false };
    bool ok = !t.IsFrameUpdated();
    tc.m_pActive = &x; ok = ok && t.IsFrameUpdated();
    tc.m_pActive = 0; p.Moving = true; ok = ok && t.IsFrameUpdated();
    p.Moving = false; t.baseUpdated = true; ok = ok && t.IsFrameUpdated();
    t.baseUpdated = false; t.tc47 = 0; ok = ok && !t.IsFrameUpdated();
    if (!ok) { std::printf("007040e0_TEST FAIL\n"); return 1; }
    std::printf("007040e0_TEST PASS\n");
    return 0;
}
