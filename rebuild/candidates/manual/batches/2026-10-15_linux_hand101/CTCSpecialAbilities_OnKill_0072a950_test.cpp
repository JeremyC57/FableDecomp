// Behaviour test for CTCSpecialAbilities::OnKill (0x0072A950): the bullet-time component (type 0x92) is removed only
// when the thing has it.
#include <cstdio>

static int g_removeCalls = 0;
static long g_removedType = 0;

struct CThing {
    unsigned long m_presentBits[9];
    void RemoveTC(long interfaceType) { ++g_removeCalls; g_removedType = interfaceType; }
};

struct CTCSpecialAbilities {
    CThing* ParentThing;
    void OnKill();
};

void CTCSpecialAbilities::OnKill()
{
    if (ParentThing->m_presentBits[0x92 >> 5] & (1UL << (0x92 & 31)))
        ParentThing->RemoveTC(0x92);
}

int main()
{
    CThing t;
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_presentBits[4] = ~0x40000UL;
    CTCSpecialAbilities s; s.ParentThing = &t;
    s.OnKill();
    if (g_removeCalls != 0) { std::printf("0072a950_TEST FAIL\n"); return 1; }
    t.m_presentBits[4] = 0x40000;
    s.OnKill();
    if (g_removeCalls != 1 || g_removedType != 0x92) { std::printf("0072a950_TEST FAIL\n"); return 1; }
    std::printf("0072a950_TEST PASS\n");
    return 0;
}
