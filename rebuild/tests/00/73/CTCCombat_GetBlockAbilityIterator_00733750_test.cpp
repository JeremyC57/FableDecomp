#include <cstdio>
struct CCombatAbilityEntry { long weaponClass; };
typedef CCombatAbilityEntry* CBlockAbilityIterator;
static CCombatAbilityEntry g_entries[2] = { { 3 }, { 5 } };
struct CTCCombat {
    int m_calls;
    bool FindBlockAbility(CBlockAbilityIterator* it, long weaponClass)
    {
        ++m_calls;
        if (*it != 0) return false;            /* the caller must zero the iterator first */
        for (int i = 0; i < 2; ++i) if (g_entries[i].weaponClass == weaponClass) { *it = &g_entries[i]; return true; }
        return false;
    }
    bool GetBlockAbilityIterator(CBlockAbilityIterator* out, long weaponClass)
    {
        CBlockAbilityIterator it = 0;
        if (FindBlockAbility(&it, weaponClass)) {
            *out = it;
            return true;
        }
        return false;
    }
};
int main()
{
    CTCCombat t; t.m_calls = 0;
    CBlockAbilityIterator out = (CBlockAbilityIterator)&t;
    if (!t.GetBlockAbilityIterator(&out, 5) || out != &g_entries[1]) { std::printf("00733750_TEST FAIL\n"); return 1; }
    CBlockAbilityIterator keep = &g_entries[0]; out = keep;
    if (t.GetBlockAbilityIterator(&out, 9) || out != keep || t.m_calls != 2) { std::printf("00733750_TEST FAIL\n"); return 1; }
    std::printf("00733750_TEST PASS\n");
    return 0;
}