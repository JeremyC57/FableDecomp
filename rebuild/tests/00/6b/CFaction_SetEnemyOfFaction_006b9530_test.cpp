#include <cstdio>
#include <cstring>
// Behaviour test for CFaction::SetEnemyOfFaction (006b9530): same logic over a copy of the model (a tiny sorted name set).
struct CCharString { const char* m_pData; };
struct CCharStringSet {
    const char* names[8]; int count;
    CCharStringSet() : count(0) { }
    int find(const CCharString& k) const { for (int i = 0; i < count; ++i) if (std::strcmp(names[i], k.m_pData) == 0) return i; return -1; }
    unsigned long erase(const CCharString& k) { int i = find(k); if (i < 0) return 0; names[i] = names[--count]; return 1; }
    bool insert(const CCharString& k) { if (find(k) >= 0) return false; names[count++] = k.m_pData; return true; }
};
struct CFaction {
    CCharString Name; CCharStringSet AlliedFactions; CCharStringSet EnemyFactions;
    void SetEnemyOfFaction(const CCharString& factionName) { AlliedFactions.erase(factionName); EnemyFactions.insert(factionName); }
};
int main()
{
    CFaction a, b; a.Name.m_pData = "GUARDS"; b.Name.m_pData = "BANDITS";
    a.AlliedFactions.insert(b.Name);
    a.SetEnemyOfFaction(b.Name);
    a.SetEnemyOfFaction(b.Name);
    if (a.AlliedFactions.count != 0 || a.EnemyFactions.count != 1 || a.EnemyFactions.find(b.Name) != 0) { std::printf("006b9530_TEST FAIL\n"); return 1; }
    std::printf("006b9530_TEST PASS\n");
    return 0;
}