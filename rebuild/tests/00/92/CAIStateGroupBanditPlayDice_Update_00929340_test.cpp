// Behaviour test for CAIStateGroup_BanditPlayDice::Update (0x00929340): a copy of the model's control flow with scripted callees.
#include <cstdio>
struct Thing { int id; };
struct Pos { Thing* user; void SetUser(Thing* t) { user = t; } };
struct Game { long players; Thing* current; void SetCurrentPlayer(Thing* t) { current = t; } };
struct Group {
    Pos* DiceGamePosition; Game* DiceGame; bool WasCarryingWeapon; Thing creature;
    int moveResult, sheatheResult, unsheatheResult; bool putAway, carrying;
    int terminates, acts, sheathes, unsheathes, lookAts; long playersDuringAct; bool userDuringAct;
    void Terminate() { ++terminates; }
    int MoveToPos() { return moveResult; }
    void LookAtPos() { ++lookAts; }
    void Act() { ++acts; playersDuringAct = DiceGame->players; userDuringAct = DiceGamePosition->user == &creature; }
    void Update()
    {
        WasCarryingWeapon = false;
        Pos* position = DiceGamePosition;
        if (position->user) { DiceGame = 0; Terminate(); return; }
        DiceGamePosition->SetUser(&creature);
        if (MoveToPos() != 2) { DiceGamePosition->SetUser(0); DiceGame = 0; Terminate(); return; }
        DiceGame->players = DiceGame->players + 1;
        LookAtPos();
        WasCarryingWeapon = putAway && carrying;
        if (WasCarryingWeapon && (++sheathes, sheatheResult) != 2) { Terminate(); return; }
        Act();
        --DiceGame->players;
        if (DiceGame->current == &creature) DiceGame->SetCurrentPlayer(0);
        DiceGamePosition->SetUser(0);
        DiceGame = 0;
        if (WasCarryingWeapon && (++unsheathes, unsheatheResult) != 2) { Terminate(); return; }
        WasCarryingWeapon = false;
        Terminate();
    }
};
static void Init(Group& g, Pos& p, Game& d)
{
    p.user = 0; d.players = 2; d.current = 0;
    g.DiceGamePosition = &p; g.DiceGame = &d; g.WasCarryingWeapon = true; g.creature.id = 7;
    g.moveResult = 2; g.sheatheResult = 2; g.unsheatheResult = 2; g.putAway = true; g.carrying = true;
    g.terminates = g.acts = g.sheathes = g.unsheathes = g.lookAts = 0; g.playersDuringAct = 0; g.userDuringAct = false;
}
int main()
{
    Group g; Pos p; Game d; Thing other;
    int bad = 0;
    // occupied position: no game, terminate
    Init(g, p, d); p.user = &other; g.Update();
    if (g.DiceGame != 0 || g.terminates != 1 || g.acts != 0 || g.WasCarryingWeapon || p.user != &other) bad |= 1;
    // move fails: position freed
    Init(g, p, d); g.moveResult = 3; g.Update();
    if (p.user != 0 || g.DiceGame != 0 || g.terminates != 1 || d.players != 2 || g.acts != 0) bad |= 2;
    // full game with weapon, we were current player
    Init(g, p, d); d.current = &g.creature; g.Update();
    if (g.acts != 1 || g.playersDuringAct != 3 || !g.userDuringAct || d.players != 2 || d.current != 0 || p.user != 0
        || g.DiceGame != 0 || g.sheathes != 1 || g.unsheathes != 1 || g.WasCarryingWeapon || g.terminates != 1 || g.lookAts != 1) bad |= 4;
    // unsheathe fails: weapon flag stays set
    Init(g, p, d); d.current = &other; g.unsheatheResult = 3; g.Update();
    if (!g.WasCarryingWeapon || d.current != &other || g.terminates != 1) bad |= 8;
    // sheathe fails: no Act, still counted in
    Init(g, p, d); g.sheatheResult = 3; g.Update();
    if (g.acts != 0 || d.players != 3 || g.DiceGame != &d || !g.WasCarryingWeapon || g.terminates != 1) bad |= 16;
    // not carrying: no sheathe / unsheathe
    Init(g, p, d); g.carrying = false; g.Update();
    if (g.sheathes != 0 || g.unsheathes != 0 || g.acts != 1 || g.WasCarryingWeapon) bad |= 32;
    if (bad) { std::printf("00929340_TEST FAIL\n"); return 1; }
    std::printf("00929340_TEST PASS\n");
    return 0;
}