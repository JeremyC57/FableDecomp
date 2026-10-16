// Behaviour test for CTCTavernGameCardSorting::NewGame (0x007D4ED0): NumRounds = number of 72-byte card-order entries.
#include <cstdio>
struct CCardPositionsDef { unsigned char d[0x48]; };
struct Def { CCardPositionsDef* first; CCardPositionsDef* last; };
struct Game {
    long NumRounds; long NumCards; float Counter; int base;
    void NewGame(const Def& d)
    {
        ++base;
        NumCards = 1;
        NumRounds = (long)(d.last - d.first);
        Counter = 0.0f;
    }
};
int main()
{
    CCardPositionsDef orders[5]; Def d; d.first = orders; d.last = orders + 5;
    Game g; g.base = 0; g.Counter = 3; g.NumRounds = 0; g.NumCards = 0; g.NewGame(d);
    if (g.NumRounds != 5 || g.NumCards != 1 || g.Counter != 0.0f || g.base != 1 || sizeof(CCardPositionsDef) != 72) {
        std::printf("007d4ed0_TEST FAIL\n"); return 1;
    }
    std::printf("007d4ed0_TEST PASS\n");
    return 0;
}
