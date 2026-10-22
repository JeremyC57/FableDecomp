// Behaviour test for CQ_BanditCampScript::WatchForCostume @ 0x00D00FD0: wait for the camp path level, show a 5-piece
// counter, update it each frame with the costume pieces the hero carries, and remove it once all five are carried.
#include <cstdio>
#include <vector>
struct Game {
    int Frame, LoadFrame, CounterAdded, CounterRemoved, Display;
    std::vector<int> Updates;
    int PiecesAt[8];
    bool IsLevelLoaded() const { return Frame >= LoadFrame; }
    void NewScriptFrame() { ++Frame; }
    int CarriedPieces() const { return PiecesAt[Frame < 7 ? Frame : 7]; }
};
static void WatchForCostume(Game& g)
{
    while (!g.IsLevelLoaded())
        g.NewScriptFrame();
    ++g.CounterAdded;
    g.Display = 1;
    int pieces;
    do {
        g.NewScriptFrame();
        int carried = g.CarriedPieces();
        pieces = 0;
        for (int i = 0; i < 5; ++i)
            if (carried & (1 << i))
                ++pieces;
        g.Updates.push_back(pieces);
    } while (pieces != 5);
    ++g.CounterRemoved;
    g.Display = 0;
}
int main()
{
    Game g;
    g.Frame = 0; g.LoadFrame = 2; g.CounterAdded = 0; g.CounterRemoved = 0; g.Display = -1;
    int carried[8] = { 0, 0, 0, 1, 0x11, 0x15, 0x1f, 0x1f };
    for (int i = 0; i < 8; ++i) g.PiecesAt[i] = carried[i];
    WatchForCostume(g);
    bool ok = g.CounterAdded == 1 && g.CounterRemoved == 1 && g.Display == 0 && g.Updates.size() == 4 &&
              g.Updates[0] == 1 && g.Updates[1] == 2 && g.Updates[2] == 3 && g.Updates[3] == 5;
    if (!ok) { std::printf("00d00fd0_TEST FAIL\n"); return 1; }
    std::printf("00d00fd0_TEST PASS\n");
    return 0;
}