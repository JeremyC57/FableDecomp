// Behaviour test for 0x006994F0 CGameCameraManager::IsObservedThingInMelee.
#include <cstdio>
struct CTCCombat { int m_meleeTarget; };
struct CThing {
    unsigned int m_presentBits[9]; CTCCombat* m_combat; unsigned char m_statusFlags;
    template<class T> bool GetInterface(T** ppOut, int type)
    {
        if (m_presentBits[type >> 5] & (1u << (type & 31))) { *ppOut = (T*)m_combat; return true; }
        return false;
    }
};
struct M {
    CThing* PObservedThing;
    bool IsObservedThingInMelee()
    {
        if (PObservedThing && !(PObservedThing->m_statusFlags & 1)) {
            CTCCombat* combat;
            if (PObservedThing->GetInterface(&combat, 0x36)) { if (combat->m_meleeTarget >= 0) return true; }
        }
        return false;
    }
};
static int Fail() { std::printf("006994f0_TEST FAIL\n"); return 1; }
int main()
{
    CTCCombat c; c.m_meleeTarget = -1;
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_combat = &c; t.m_statusFlags = 0;
    M m; m.PObservedThing = 0;
    if (m.IsObservedThingInMelee()) return Fail();
    m.PObservedThing = &t;
    if (m.IsObservedThingInMelee()) return Fail();
    t.m_presentBits[1] = 0x400000u;
    if (m.IsObservedThingInMelee()) return Fail();
    c.m_meleeTarget = 0;
    if (!m.IsObservedThingInMelee()) return Fail();
    t.m_statusFlags = 1;
    if (m.IsObservedThingInMelee()) return Fail();
    std::printf("006994f0_TEST PASS\n");
    return 0;
}
