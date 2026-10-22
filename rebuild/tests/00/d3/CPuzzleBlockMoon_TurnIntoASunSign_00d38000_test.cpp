// Behaviour test for TurnIntoASunSign @ 0x00D38000: guarded by PreventRoundEnd; bails if the round already ended;
// replaces the effect, creates the sun tile, counts it, and on the last pad ends the round as a good round.
#include <cstdio>
#include <string>
#include <vector>
struct Puzzle {
    bool PreventRoundEnd, RoundEnded, RoundBeginning, EffectAlive, MeRemoved;
    int NumberOfSuns, NumberOfPads, GoodRounds, PuzzleRound; bool ProphetsReleased[4];
    std::vector<std::string> Log;
    void TurnIntoASunSign(bool createEffect)
    {
        PreventRoundEnd = true;
        if (RoundEnded) { RoundEnded = false; return; }
        if (EffectAlive) { Log.push_back("remove effect"); EffectAlive = false; }
        if (createEffect) { Log.push_back("PadSunFX"); EffectAlive = true; }
        Log.push_back("OBJECT_LPDD_TILE_SUN_01");
        ++NumberOfSuns;
        if (NumberOfSuns == NumberOfPads && !RoundBeginning) {
            ++GoodRounds; Log.push_back("frame fx"); Log.push_back("sound"); Log.push_back("pause 0.2");
            RoundEnded = true; ProphetsReleased[PuzzleRound] = true;
        }
        MeRemoved = true;
        PreventRoundEnd = false;
    }
};
int main()
{
    Puzzle p = { false, false, false, true, false, 2, 3, 0, 1, { false, false, false, false } };
    p.TurnIntoASunSign(true);
    Puzzle q = { false, true, false, false, false, 0, 3, 0, 0, { false, false, false, false } };
    q.TurnIntoASunSign(true);
    bool ok = p.NumberOfSuns == 3 && p.GoodRounds == 1 && p.RoundEnded && p.ProphetsReleased[1] && p.MeRemoved &&
              !p.PreventRoundEnd && p.Log.size() == 6 && p.Log[0] == "remove effect" &&
              !q.RoundEnded && q.PreventRoundEnd && q.Log.empty() && q.NumberOfSuns == 0;
    if (!ok) { std::printf("00d38000_TEST FAIL\n"); return 1; }
    std::printf("00d38000_TEST PASS\n");
    return 0;
}