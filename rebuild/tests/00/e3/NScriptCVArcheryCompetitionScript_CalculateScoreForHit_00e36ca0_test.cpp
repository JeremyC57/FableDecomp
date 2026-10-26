// Behaviour test for CV_ArcheryCompetitionScript::CalculateScoreForHit (00e36ca0).
#include <cstdio>
struct FakeGSI { long id, count, target; int calls; void UpdateQuestInfoCounter(long i, long c, long t) { id = i; count = c; target = t; ++calls; } };
struct Master { const float* bands; };
static const float g_bands[3] = { 10.0f, 20.0f, 30.0f };
static Master g_master = { g_bands };
struct Script {
    FakeGSI* GSI; long Score; long ArcheryGameScoreInfoID; bool term;
    bool IsActiveThreadTerminating() { return term; }
    void CalculateScoreForHit(long points, float distance)
    {
        const float* bands = g_master.bands;
        if (distance < bands[0]) { if (IsActiveThreadTerminating()) return; Score += points; }
        else if (distance < bands[1]) { if (IsActiveThreadTerminating()) return; Score += points * 2; }
        else if (distance < bands[2]) { if (IsActiveThreadTerminating()) return; Score += points * 3; }
        else { if (IsActiveThreadTerminating()) return; Score += points * 4; }
        GSI->UpdateQuestInfoCounter(ArcheryGameScoreInfoID, Score, -1);
    }
};
int main()
{
    FakeGSI g = { 0, 0, 0, 0 };
    Script s = { &g, 0, 7, false };
    s.CalculateScoreForHit(5, 5.0f);   // +5
    s.CalculateScoreForHit(5, 15.0f);  // +10
    s.CalculateScoreForHit(5, 25.0f);  // +15
    s.CalculateScoreForHit(5, 35.0f);  // +20
    bool ok = s.Score == 50 && g.calls == 4 && g.id == 7 && g.count == 50 && g.target == -1;
    s.term = true; s.CalculateScoreForHit(5, 5.0f);
    ok = ok && s.Score == 50 && g.calls == 4;
    if (!ok) { std::printf("00e36ca0_TEST FAIL\n"); return 1; }
    std::printf("00e36ca0_TEST PASS\n");
    return 0;
}