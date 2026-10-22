// Behaviour test for TurnIntoAMoonSign @ 0x00D387A0: mirror of the sun variant; decrements the sun count and on zero
// (outside RoundBeginning) ends the round as an evil round and marks the prophet killed.
#include <cstdio>
struct Puzzle {
    bool PreventRoundEnd, RoundEnded, RoundBeginning, EffectAlive, MeRemoved;
    int NumberOfSuns, EvilRounds, PuzzleRound, Tiles; bool ProphetsKilled[4];
    void TurnIntoAMoonSign(bool createEffect)
    {
        PreventRoundEnd = true;
        if (RoundEnded) { RoundEnded = false; return; }
        if (EffectAlive) EffectAlive = false;
        if (createEffect) EffectAlive = true;
        ++Tiles;
        --NumberOfSuns;
        if (NumberOfSuns == 0 && !RoundBeginning) { ++EvilRounds; RoundEnded = true; ProphetsKilled[PuzzleRound] = true; }
        MeRemoved = true;
        PreventRoundEnd = false;
    }
};
int main()
{
    Puzzle p = { false, false, false, false, false, 1, 0, 2, 0, { false, false, false, false } };
    p.TurnIntoAMoonSign(false);
    Puzzle q = { false, false, true, false, false, 1, 0, 2, 0, { false, false, false, false } };
    q.TurnIntoAMoonSign(true);
    bool ok = p.NumberOfSuns == 0 && p.EvilRounds == 1 && p.ProphetsKilled[2] && !p.EffectAlive && p.Tiles == 1 &&
              q.NumberOfSuns == 0 && q.EvilRounds == 0 && !q.RoundEnded && q.EffectAlive && q.MeRemoved;
    if (!ok) { std::printf("00d387a0_TEST FAIL\n"); return 1; }
    std::printf("00d387a0_TEST PASS\n");
    return 0;
}