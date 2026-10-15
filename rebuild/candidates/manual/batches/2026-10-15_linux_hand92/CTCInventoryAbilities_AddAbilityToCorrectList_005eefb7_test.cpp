#include <cstdio>
#include <cstring>
// Behaviour test for CTCInventoryAbilities::AddAbilityToCorrectList (0x005EEFB7): same logic over a copy of the model.
static int g_live = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { ++g_live; }
    CCharString(const CCharString& o) : m_pData(o.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CCharStringVector {
    const char* items[4]; int count;
    void push_back(const CCharString& value) { items[count++] = value.m_pData; }
};
struct SAbilityEntry { long Level; void* PDef; };
struct CAbilityEntryVector { SAbilityEntry* first; SAbilityEntry* last; SAbilityEntry* end_of_storage; SAbilityEntry& operator[](unsigned long n) { return first[n]; } };
static const char* g_names[3] = { "OBJECT_ABILITY_NONE", "OBJECT_ABILITY_FIREBALL", "OBJECT_ABILITY_HEAL" };
struct CTCInventoryAbilities_Retail {
    CAbilityEntryVector Abilities;
    CCharString GetAbilityInventoryThingName(long ability) { return CCharString(g_names[ability]); }
    void AddAbilityToCorrectList(long ability, CCharStringVector* owned, CCharStringVector* notOwned)
    {
        if (Abilities[ability].Level >= 0)
            owned->push_back(GetAbilityInventoryThingName(ability));
        else
            notOwned->push_back(GetAbilityInventoryThingName(ability));
    }
};
int main()
{
    SAbilityEntry e[3] = { { -1, 0 }, { 0, 0 }, { -1, 0 } };
    CTCInventoryAbilities_Retail a; a.Abilities.first = e; a.Abilities.last = e + 3; a.Abilities.end_of_storage = e + 3;
    CCharStringVector owned, notOwned; owned.count = 0; notOwned.count = 0;
    a.AddAbilityToCorrectList(1, &owned, &notOwned);
    a.AddAbilityToCorrectList(2, &owned, &notOwned);
    if (owned.count != 1 || notOwned.count != 1 || std::strcmp(owned.items[0], g_names[1]) != 0
        || std::strcmp(notOwned.items[0], g_names[2]) != 0 || g_live != 0) { std::printf("005eefb7_TEST FAIL\n"); return 1; }
    std::printf("005eefb7_TEST PASS\n");
    return 0;
}
