#include <cstdio>
// Behaviour of CTCHeroStats::GetStartingMaxStamina (0x0057A939): reads MaxStaminaPointsPerMagicPowerLevel[0] through a counted copy of the
// definition holder's def pointer; the copy's add-ref is undone, and the def is destroyed only by the last release.
static int g_destroyed = 0;
struct CHeroStatsDef {
    long RefCount;
    long* MaxStaminaPointsPerMagicPowerLevel;
    void Destroy() { ++g_destroyed; }
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CHeroStatsDefPointer {
    CHeroStatsDef* Object;
    explicit CHeroStatsDefPointer(CHeroStatsDef* object) : Object(object) {}   // test set-up: adopts a reference
    CHeroStatsDefPointer(const CHeroStatsDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CHeroStatsDefPointer() { Object->Release(); }
    CHeroStatsDef* operator->() const { return Object; }
};
struct CDefinitionHolder {
    CHeroStatsDefPointer PHeroStatsDef;
    explicit CDefinitionHolder(CHeroStatsDef* def) : PHeroStatsDef(def) {}
    CHeroStatsDefPointer GetHeroStatsDef() const { return PHeroStatsDef; }
};
static CDefinitionHolder* g_holder = 0;
struct CTCHeroStats_Retail {
    CDefinitionHolder* GetDefinitions() const { return g_holder; }
    long GetStartingMaxStamina() const
    {
        CHeroStatsDefPointer def = GetDefinitions()->GetHeroStatsDef();
        return def->MaxStaminaPointsPerMagicPowerLevel[0];
    }
};
int main()
{
    CHeroStatsDef def; def.RefCount = 1;
    long levels[3] = { 60, 80, 100 }; def.MaxStaminaPointsPerMagicPowerLevel = levels;
    {
        CDefinitionHolder holder(&def); g_holder = &holder;
        CTCHeroStats_Retail s;
        if (s.GetStartingMaxStamina() != 60 || def.RefCount != 1 || g_destroyed != 0) { std::printf("0057a939_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1) { std::printf("0057a939_TEST FAIL\n"); return 1; }   // the holder's own reference was the last
    std::printf("0057a939_TEST PASS\n");
    return 0;
}