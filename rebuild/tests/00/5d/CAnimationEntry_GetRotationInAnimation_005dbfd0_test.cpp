// Behaviour test for CAnimationEntry::GetRotationInAnimation @ 0x005dbfd0: the negated rotation of the hero ability entry named by BankIndex.
#include <cstdio>
struct CAbilityEntry { unsigned char _pad[0x24]; float m_rotation; };
static CAbilityEntry g_entries[3];
struct CTCInventoryAbilities { CAbilityEntry* GetAbilityEntry(long ability) { return &g_entries[ability]; } };
struct CGame { unsigned char _pad[0x3c]; CTCInventoryAbilities* m_pHeroAbilities; };
struct CApplication { unsigned char _pad[0x24]; CGame* m_pGame; };
static CApplication* g_pApplication;
struct CAnimationEntry {
    long BankIndex;
    void GetRotationInAnimation(float* pRotation) const
    { CTCInventoryAbilities* abilities = g_pApplication->m_pGame->m_pHeroAbilities; *pRotation = -abilities->GetAbilityEntry(BankIndex)->m_rotation; }
};
int main()
{
    CTCInventoryAbilities abilities; CGame game; game.m_pHeroAbilities = &abilities; CApplication app; app.m_pGame = &game; g_pApplication = &app;
    g_entries[0].m_rotation = 1.0f; g_entries[1].m_rotation = -2.5f; g_entries[2].m_rotation = 0.0f;
    CAnimationEntry e; float r = 99.0f;
    e.BankIndex = 1; e.GetRotationInAnimation(&r); bool ok = r == 2.5f;
    e.BankIndex = 0; e.GetRotationInAnimation(&r); if (r != -1.0f) ok = false;
    e.BankIndex = 2; e.GetRotationInAnimation(&r); if (r != 0.0f) ok = false;
    if (!ok) { std::printf("005dbfd0_TEST FAIL\n"); return 1; }
    std::printf("005dbfd0_TEST PASS\n");
    return 0;
}