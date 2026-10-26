// behaviour test for 0x005EDC00 CTCGamePlayer::SetGame
#include <cstdio>
class CThing { public: int id; };
struct Game { CThing* ParentThing; };
struct Player {
    Game* game; CThing* table;
    void SetGame(Game* g) { game = g; if (g) table = g->ParentThing; else table = 0; }
};
int main()
{
    CThing t = { 1 };
    Game g = { &t };
    Player p = { 0, 0 };
    p.SetGame(&g);
    bool ok = p.game == &g && p.table == &t;
    p.SetGame(0);
    ok = ok && p.game == 0 && p.table == 0;
    if (!ok) { std::printf("005edc00_TEST FAIL\n"); return 1; }
    std::printf("005edc00_TEST PASS\n");
    return 0;
}