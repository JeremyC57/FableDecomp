#include <cstdio>
static int g_step = 0, g_unregisteredAt = -1, g_cleanedAt = -1;
struct CTCBase;
struct CThing {
    CTCBase* m_registered;
    void SetComponentRegistered(bool registered, CTCBase* component) { if (!registered && m_registered == component) m_registered = 0; g_unregisteredAt = g_step++; }
};
static CThing* g_cleaned = 0;
void CleanupRangedWeapon(CThing* creature) { g_cleaned = creature; g_cleanedAt = g_step++; }
struct CTCBase { void* vptrSlot; CThing* ParentThing; };
struct CTCFireRangedWeaponOnLeavingStateGroup : CTCBase {
    void FinalCleanup()
    {
        CThing* creature = ParentThing;
        creature->SetComponentRegistered(false, this);
        CleanupRangedWeapon(creature);
    }
};
int main()
{
    CThing hero; CTCFireRangedWeaponOnLeavingStateGroup c; c.ParentThing = &hero; hero.m_registered = &c;
    c.FinalCleanup();
    if (hero.m_registered != 0 || g_cleaned != &hero || g_unregisteredAt != 0 || g_cleanedAt != 1) { std::printf("007e5cc0_TEST FAIL\n"); return 1; }
    std::printf("007e5cc0_TEST PASS\n");
    return 0;
}
