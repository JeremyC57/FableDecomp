// Test for 0x005EB180 CTCTavernGame::SetStateAfterInstructions: self-contained behaviour model -- a pending quit leaves the game
// at once; otherwise the selection sound plays, every text of the instruction group is shown as an OK subtitle (with the OK
// text), and the inlined SetState moves on (state 5 marks the game played, 47 or a quit raised meanwhile leaves the game).
#include <cstdio>

static bool g_quit = false;
struct Bank {
    int groups[3][4];   // group id -> up to 3 text ids, 0 terminated
    int groupCount[3];
    bool hasGroup[3];
};
struct Game {
    int state, prev, left, sounds, shown, lastOk, shownIds[8];
    bool played;
    void LeaveGame() { ++left; }
    void SetState(int s)
    {
        if (g_quit) { g_quit = false; s = 47; }
        if (s == 5) played = true;
        else if (s == 47) { LeaveGame(); return; }
        prev = state; state = s;
    }
    void SetStateAfterInstructions(const Bank& bank, unsigned int group, int okText, int s)
    {
        if (g_quit) { g_quit = false; LeaveGame(); return; }
        ++sounds;
        if (group < 3 && bank.hasGroup[group]) {
            for (int i = 0; i < bank.groupCount[group]; ++i) {
                shownIds[shown++] = bank.groups[group][i];
                lastOk = okText;
            }
        }
        SetState(s);
    }
};

static int fail(int n) { std::printf("005eb180_TEST FAIL %d\n", n); return 1; }

int main()
{
    Bank bank = { { { 11, 12, 13, 0 }, { 0, 0, 0, 0 }, { 31, 0, 0, 0 } }, { 3, 0, 1 }, { true, false, true } };
    Game g = { 2, 0, 0, 0, 0, 0, { 0 }, false };
    g_quit = true;
    g.SetStateAfterInstructions(bank, 0, 7, 3);
    if (g.left != 1 || g.sounds != 0 || g.shown != 0 || g.state != 2 || g_quit) return fail(1);
    g.SetStateAfterInstructions(bank, 0, 7, 3);
    if (g.sounds != 1 || g.shown != 3 || g.shownIds[2] != 13 || g.lastOk != 7 || g.state != 3 || g.prev != 2) return fail(2);
    g.SetStateAfterInstructions(bank, 1, 8, 5);
    if (g.shown != 3 || !g.played || g.state != 5) return fail(3);
    g.SetStateAfterInstructions(bank, 2, 9, 47);
    if (g.shown != 4 || g.shownIds[3] != 31 || g.left != 2 || g.state != 5) return fail(4);
    std::printf("005eb180_TEST PASS\n");
    return 0;
}