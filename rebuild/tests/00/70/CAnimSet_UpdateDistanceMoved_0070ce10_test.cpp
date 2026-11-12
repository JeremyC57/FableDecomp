// Behaviour test for CAnimSet::UpdateDistanceMoved (0x0070CE10): a copy of the logic over a small model.
#include <cstdio>
#include <cmath>
#include <vector>
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct Info { float X, Y, Z; float Distance() const { return (float)sqrt(X * X + Y * Y + Z * Z); } };
static Info g_infos[4];
struct Alpha { float CurrentAlpha, LastAlpha; };
struct Set {
    std::vector<short> Anims; std::vector<Alpha> InternalAlphas;
    float Current, Last, LastUpdateChange, Scale; bool Looping, Finished;
    void UpdateDistanceMoved(float distance)
    {
        std::vector<float> speeds(Anims.size(), 0.0f);
        bool noMovement = true;
        for (unsigned int i = 0; i < Anims.size(); i++)
            if (Anims[i]) { speeds[i] = g_infos[Anims[i]].Distance() * 0.01f; if (speeds[i] > 0.0f) noMovement = false; }
        float change;
        if (noMovement) change = 0.1f;
        else {
            float total = 0.0f;
            for (unsigned int i = 0; i < Anims.size(); i++)
                if (Anims[i]) total += InternalAlphas[i].CurrentAlpha * speeds[i];
            change = total > DELTA_NOTIONAL_ZERO ? distance / (total * Scale) : 0.0f;
        }
        LastUpdateChange = change;
        Last = Current; Current = change + Current;
        if (Looping) {
            if (Last > 1.0f) { Last -= 1.0f; Current -= 1.0f; }
            else if (Last < 0.0f) { Last += 1.0f; Current += 1.0f; }
        } else {
            if (Current < 0.0f) Current = 0.0f; else if (Current > 1.0f) Current = 1.0f;
            if (1.0f - DELTA_NOTIONAL_ZERO <= Current) Finished = true;
        }
    }
};
static bool near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    g_infos[1].X = 300.0f; g_infos[1].Y = 0.0f; g_infos[1].Z = 400.0f;   // distance 500 -> speed 5
    g_infos[2].X = 0.0f; g_infos[2].Y = 0.0f; g_infos[2].Z = 0.0f;       // no movement
    Set s; s.Anims.push_back(1); Alpha a = { 0.5f, 0.0f }; s.InternalAlphas.push_back(a);
    s.Current = 0.2f; s.Last = 0.0f; s.Scale = 2.0f; s.Looping = false; s.Finished = false;
    s.UpdateDistanceMoved(1.0f);   // change = 1 / (0.5*5*2) = 0.2
    if (!near(s.LastUpdateChange, 0.2f) || !near(s.Current, 0.4f) || !near(s.Last, 0.2f) || s.Finished) { std::printf("0070ce10_TEST FAIL\n"); return 1; }
    s.UpdateDistanceMoved(4.0f);   // +0.8 -> clamps to 1, finished
    if (!near(s.Current, 1.0f) || !s.Finished) { std::printf("0070ce10_TEST FAIL\n"); return 1; }
    Set t; t.Anims.push_back(2); t.InternalAlphas.push_back(a);
    t.Current = 0.95f; t.Last = 0.0f; t.Scale = 1.0f; t.Looping = true; t.Finished = false;
    t.UpdateDistanceMoved(10.0f);  // no movement -> 0.1 ; Last 0.95 not > 1, Current 1.05
    if (!near(t.LastUpdateChange, 0.1f) || !near(t.Current, 1.05f)) { std::printf("0070ce10_TEST FAIL\n"); return 1; }
    t.UpdateDistanceMoved(10.0f);  // Last 1.05 > 1 -> wraps: Last 0.05, Current 0.15
    if (!near(t.Last, 0.05f) || !near(t.Current, 0.15f) || t.Finished) { std::printf("0070ce10_TEST FAIL\n"); return 1; }
    std::printf("0070ce10_TEST PASS\n");
    return 0;
}