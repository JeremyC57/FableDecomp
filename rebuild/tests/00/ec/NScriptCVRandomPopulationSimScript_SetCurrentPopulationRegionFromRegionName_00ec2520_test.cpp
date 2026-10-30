// Behaviour test for NScript::CV_RandomPopulationSimScript::SetCurrentPopulationRegionFromRegionName 0x00EC2520
#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* PData;                                  // 0 = the null string
    bool operator==(const char* text) const
    {
        if (PData == 0)
            return std::strcmp(text, "") == 0;
        return std::strcmp(PData, text) == 0;
    }
};
struct CV_RandomPopulationSimScript {
    bool Terminating;
    long CurrentPopulationRegion;
    int  TermCalls;
    bool IsActiveThreadTerminating() { ++TermCalls; return Terminating; }
    void SetCurrentPopulationRegionFromRegionName(CCharString& region)
    {
        if (region == "GreatwoodEntrance" || region == "GreatwoodLake" || region == "Greatwood" || region == "GreatwoodBanditToll") {
            if (IsActiveThreadTerminating())
                return;
            CurrentPopulationRegion = 0;
        } else if (region == "Darkwood1" || region == "Darkwood2" || region == "Darkwood3" || region == "Darkwood5"
                   || region == "Darkwood6") {
            if (IsActiveThreadTerminating())
                return;
            CurrentPopulationRegion = 1;
        } else if (region == "Witchwood2" || region == "Witchwood4") {
            if (IsActiveThreadTerminating())
                return;
            CurrentPopulationRegion = 2;
        } else if (region == "LookoutPoint") {
            if (IsActiveThreadTerminating())
                return;
            CurrentPopulationRegion = 3;
        } else {
            if (IsActiveThreadTerminating())
                return;
        }
    }
};
static long Run(const char* name, bool term, int* calls)
{
    CV_RandomPopulationSimScript s;
    s.Terminating = term;
    s.CurrentPopulationRegion = -1;
    s.TermCalls = 0;
    CCharString r;
    r.PData = name;
    s.SetCurrentPopulationRegionFromRegionName(r);
    if (calls)
        *calls = s.TermCalls;
    return s.CurrentPopulationRegion;
}
int main()
{
    int calls = 0;
    const char* g[] = { "GreatwoodEntrance", "GreatwoodLake", "Greatwood", "GreatwoodBanditToll" };
    const char* d[] = { "Darkwood1", "Darkwood2", "Darkwood3", "Darkwood5", "Darkwood6" };
    int i;
    for (i = 0; i < 4; ++i)
        if (Run(g[i], false, 0) != 0) { std::printf("00ec2520_TEST FAIL\n"); return 1; }
    for (i = 0; i < 5; ++i)
        if (Run(d[i], false, 0) != 1) { std::printf("00ec2520_TEST FAIL\n"); return 1; }
    if (Run("Darkwood4", false, &calls) != -1 || calls != 1) { std::printf("00ec2520_TEST FAIL\n"); return 1; }
    if (Run("Witchwood2", false, 0) != 2 || Run("Witchwood4", false, 0) != 2 || Run("Witchwood1", false, 0) != -1) {
        std::printf("00ec2520_TEST FAIL\n"); return 1;
    }
    if (Run("LookoutPoint", false, 0) != 3 || Run("LookoutPoint", true, 0) != -1 || Run(0, false, 0) != -1) {
        std::printf("00ec2520_TEST FAIL\n"); return 1;
    }
    std::printf("00ec2520_TEST PASS\n");
    return 0;
}