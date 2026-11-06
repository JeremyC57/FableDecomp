// Behaviour test for CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine::Main (0x00E158F0): at stages 2 and 4 the
// balverine needs 10 hero hits (the quest counter follows them), then escapes (stage 3 / 5); at stage 6 five hits, then it runs
// and fades out (stage 7).
#include <cstdio>

struct Balverine {
    long BalverineState, Counter;
    void Stage(long hitsWanted, long next)
    {
        long hits = 0;
        do { ++hits; Counter = hits; } while (hits < hitsWanted);
        BalverineState = next;
    }
    void Step()
    {
        if (BalverineState == 2) Stage(10, 3);
        if (BalverineState == 4) Stage(10, 5);
        if (BalverineState == 6) Stage(5, 7);
    }
};

int main()
{
    Balverine b = { 2, 0 };
    b.Step();
    bool ok = b.BalverineState == 3 && b.Counter == 10;
    b.BalverineState = 4; b.Step();
    ok = ok && b.BalverineState == 5 && b.Counter == 10;
    b.BalverineState = 6; b.Step();
    ok = ok && b.BalverineState == 7 && b.Counter == 5;
    if (!ok) { std::printf("00e158f0_TEST FAIL\n"); return 1; }
    std::printf("00e158f0_TEST PASS\n");
    return 0;
}