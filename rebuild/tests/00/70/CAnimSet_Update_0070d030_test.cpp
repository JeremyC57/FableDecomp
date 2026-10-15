// Behaviour test for CAnimSet::Update (0x0070D030): per-mode driver dispatch, the percentage modes
// shifting Current into Last, and the alpha roll on every path.
#include <cstdio>

struct CAnimSetUpdateInfo { float TimePassed, DistanceMoved, PercentageComplete; };

struct CAnimSet {
    int UpdateMode;
    float CurrentPercentageComplete, LastPercentageComplete;
    float timeSeen, distanceSeen;
    int rolls;
    void UpdateTimePassed(float t) { timeSeen = t; }
    void UpdateDistanceMoved(float d) { distanceSeen = d; }
    void UpdateInternalAlphas() { ++rolls; }
    void SetPercentageComplete(float p) { LastPercentageComplete = CurrentPercentageComplete; CurrentPercentageComplete = p; }
    void Update(const CAnimSetUpdateInfo& info)
    {
        switch (UpdateMode) {
        case 1: UpdateTimePassed(info.TimePassed); break;
        case 2: UpdateDistanceMoved(info.DistanceMoved); break;
        case 3:
        case 4: SetPercentageComplete(info.PercentageComplete); break;
        }
        UpdateInternalAlphas();
    }
};

int main()
{
    CAnimSetUpdateInfo info = { 0.5f, 2.0f, 0.75f };
    CAnimSet s = { 1, 0.25f, 0.0f, 0.0f, 0.0f, 0 };
    s.Update(info);
    if (s.timeSeen != 0.5f || s.distanceSeen != 0.0f || s.rolls != 1) { std::printf("0070d030_TEST FAIL\n"); return 1; }
    s.UpdateMode = 2; s.Update(info);
    if (s.distanceSeen != 2.0f || s.rolls != 2) { std::printf("0070d030_TEST FAIL\n"); return 1; }
    s.UpdateMode = 4; s.Update(info);
    if (s.CurrentPercentageComplete != 0.75f || s.LastPercentageComplete != 0.25f || s.rolls != 3) { std::printf("0070d030_TEST FAIL\n"); return 1; }
    s.UpdateMode = 0; s.Update(info);
    if (s.CurrentPercentageComplete != 0.75f || s.rolls != 4) { std::printf("0070d030_TEST FAIL\n"); return 1; }
    std::printf("0070d030_TEST PASS\n");
    return 0;
}