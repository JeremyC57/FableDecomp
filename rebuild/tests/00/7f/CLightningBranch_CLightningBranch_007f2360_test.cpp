// Behaviour test for 0x007F2360 (CLightningBranch::~CLightningBranch): kill the effect, strip 0x10b from the target, skipping dead/null things.
#include <cstdio>
struct Thing { unsigned char flags; int killed; long removed; void Kill(bool) { ++killed; } void RemoveTC(long t) { removed = t; } };
struct Ptr { Thing* item; Thing* GetPItem() const { return item; } };
struct Branch {
    Ptr m_target, m_effect;
    ~Branch()
    {
        if (m_effect.GetPItem() && !(m_effect.GetPItem()->flags & 1)) m_effect.GetPItem()->Kill(true);
        if (m_target.GetPItem() && !(m_target.GetPItem()->flags & 1)) m_target.GetPItem()->RemoveTC(0x10b);
    }
};
int main()
{
    Thing t = { 0, 0, 0 }, e = { 0, 0, 0 }, dead = { 1, 0, 0 };
    { Branch b; b.m_target.item = &t; b.m_effect.item = &e; }
    int ok = e.killed == 1 && t.removed == 0x10b;
    { Branch b; b.m_target.item = &dead; b.m_effect.item = &dead; }
    ok &= dead.killed == 0 && dead.removed == 0;
    { Branch b; b.m_target.item = 0; b.m_effect.item = 0; }
    if (!ok) { std::printf("007f2360_TEST FAIL\n"); return 1; }
    std::printf("007f2360_TEST PASS\n");
    return 0;
}