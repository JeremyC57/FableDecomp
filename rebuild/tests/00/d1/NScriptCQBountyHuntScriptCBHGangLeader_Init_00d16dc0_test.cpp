// Behaviour test for CBH_GangLeader::Init (0x00D16DC0): counters and stage flags reset, then the two named callbacks registered in order.
#include <cstdio>
#include <cstring>
struct CParent {
    int adds;
    char names[2][64];
    void Add(const char* prefix, const char* name) { std::strcpy(names[adds], prefix); std::strcat(names[adds], name); ++adds; }
};
struct CGangLeader {
    CParent* ParentClass;
    long hitCount; float HealthInitial, HealthCurrent;
    bool stage1Done, stage2Done, stage3Done, stage4Done;
    void Init()
    {
        hitCount = 0; HealthInitial = 0; HealthCurrent = 0;
        stage1Done = false; stage2Done = false; stage3Done = false; stage4Done = false;
        ParentClass->Add("ParentClass.", "WatchForLeaderKilled");
        ParentClass->Add("ParentClass.", "WatchForDecapitation");
    }
};
int main()
{
    CParent parent = { 0 };
    CGangLeader g; g.ParentClass = &parent; g.hitCount = 9; g.HealthInitial = 3.0f; g.HealthCurrent = 2.0f;
    g.stage1Done = g.stage2Done = g.stage3Done = g.stage4Done = true;
    g.Init();
    bool ok = g.hitCount == 0 && g.HealthInitial == 0.0f && g.HealthCurrent == 0.0f && !g.stage1Done && !g.stage2Done
        && !g.stage3Done && !g.stage4Done && parent.adds == 2
        && std::strcmp(parent.names[0], "ParentClass.WatchForLeaderKilled") == 0
        && std::strcmp(parent.names[1], "ParentClass.WatchForDecapitation") == 0;
    if (!ok) { std::printf("00d16dc0_TEST FAIL\n"); return 1; }
    std::printf("00d16dc0_TEST PASS\n");
    return 0;
}