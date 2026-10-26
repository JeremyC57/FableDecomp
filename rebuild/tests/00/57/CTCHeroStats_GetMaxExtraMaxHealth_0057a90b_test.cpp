#include <cstdio>
// Behaviour of CTCHeroStats::GetMaxExtraMaxHealth (0x0057A90B): reads the cap on bonus hit points through a counted copy of the
// definition holder's def pointer; the copy's add-ref is undone, and the def is destroyed only by the last release.
static int g_destroyed = 0;
class CHeroStatsDef { public:
    long RefCount;
    float MaxExtraMaxHitPoints;
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
    float GetMaxExtraMaxHealth() const
    {
        CHeroStatsDefPointer def = GetDefinitions()->GetHeroStatsDef();
        return def->MaxExtraMaxHitPoints;
    }
};
int main()
{
    CHeroStatsDef def; def.RefCount = 1;
    def.MaxExtraMaxHitPoints = 125.5f;
    {
        CDefinitionHolder holder(&def); g_holder = &holder;
        CTCHeroStats_Retail s;
        if (s.GetMaxExtraMaxHealth() != 125.5f || def.RefCount != 1 || g_destroyed != 0) { std::printf("0057a90b_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1) { std::printf("0057a90b_TEST FAIL\n"); return 1; }   // the holder's own reference was the last
    std::printf("0057a90b_TEST PASS\n");
    return 0;
}