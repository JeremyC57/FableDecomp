// Behaviour test for CTCHeroExperience::OnSerialise (0x006D8E30): five transfers in order, vectors resized to
// 3 / 10 only when needed, abilities refreshed only when loading.
#include <cstdio>
#include <cstring>

static int g_resizes = 0;
struct CLongVector {
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    long store[16];
    void Init(int n) { _M_start = store; _M_finish = _M_end_of_storage = store + n; }
    unsigned long size() const { return _M_finish - _M_start; }
    void resize(unsigned long n, const long& value) { for (unsigned long i = size(); i < n; ++i) store[i] = value; _M_finish = _M_start + n; ++g_resizes; }
};
static const char* g_names[32]; static int g_count = 0;
struct CPersistContext {
    long Mode;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, long& value, const long& defaultValue) { g_names[g_count++] = name; if (defaultValue != 0) value = -99; }
    void TransferVector(const char* name, CLongVector* value) { g_names[g_count++] = name; (void)value; }
};
static int g_updates = 0; static bool g_notify = false;
struct CTCHeroExperience {
    long FishingLevel, TotalExperiencePoints, ExperiencePointsAvailableToSpend;
    CLongVector StatExperiencePoints, TrainableStatLevels;
    void UpdateAbilitiesForAllStats(bool notify) { ++g_updates; g_notify = notify; }
    void OnSerialise(CPersistContext& context, bool);
};
void CTCHeroExperience::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("FishingLevel", FishingLevel, 0);
    context.Transfer("TotalExperiencePoints", TotalExperiencePoints, 0);
    context.Transfer("ExperiencePointsAvailableToSpend", ExperiencePointsAvailableToSpend, 0);
    context.TransferVector("StatExperiencePoints", &StatExperiencePoints);
    context.TransferVector("TrainableStatLevels", &TrainableStatLevels);
    if (StatExperiencePoints.size() != 3)
        StatExperiencePoints.resize(3, 0);
    if (TrainableStatLevels.size() != 10)
        TrainableStatLevels.resize(10, 0);
    if (!context.IsSaving())
        UpdateAbilitiesForAllStats(true);
}
int main()
{
    CTCHeroExperience e; e.FishingLevel = 1; e.TotalExperiencePoints = 2; e.ExperiencePointsAvailableToSpend = 3;
    e.StatExperiencePoints.Init(3); e.TrainableStatLevels.Init(4);
    CPersistContext save; save.Mode = 1;
    e.OnSerialise(save, false);
    if (g_count != 5 || std::strcmp(g_names[0], "FishingLevel") || std::strcmp(g_names[2], "ExperiencePointsAvailableToSpend") ||
        std::strcmp(g_names[4], "TrainableStatLevels") || g_resizes != 1 || e.TrainableStatLevels.size() != 10 ||
        e.TrainableStatLevels.store[9] != 0 || g_updates != 0 || e.FishingLevel != 1) { std::printf("006d8e30_TEST FAIL\n"); return 1; }
    CPersistContext load; load.Mode = 2; e.StatExperiencePoints.Init(0);
    e.OnSerialise(load, false);
    if (g_resizes != 2 || e.StatExperiencePoints.size() != 3 || g_updates != 1 || !g_notify) { std::printf("006d8e30_TEST FAIL\n"); return 1; }
    CPersistContext save3; save3.Mode = 3; e.OnSerialise(save3, false);
    if (g_updates != 1 || g_resizes != 2) { std::printf("006d8e30_TEST FAIL\n"); return 1; }
    std::printf("006d8e30_TEST PASS\n");
    return 0;
}