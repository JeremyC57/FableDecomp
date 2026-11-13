// Behaviour test for CTCTavernGame::ProcessInput 0x005EC270: a copy of the state machine over a plain model.
#include <cstdio>
struct Def { long MinBet, MaxBet; };
struct Input { bool pressed; int action; bool stick; float x, y; int type; };
struct Game {
    int state, next, yes, no, alt; long base, last, money; bool finished, won; int performed, setStates, bets, clears, yesno;
    void SetState(int s) { state = s; ++setStates; }
    void ChangeBet(bool up) { bets += up ? 1 : -1; }
    void Answer(int s) { SetState(s); alt = 0; no = 0; yes = 0; ++clears; ++performed; }
    void ProcessInput(const Input& in, const Def& def)
    {
        switch (state) {
        case 5:
            if (in.pressed && (in.action == 0x18 || in.action == 0x53)) { finished = true; won = false; ++yesno; ++performed; }
            break;
        case 46:
            if (in.pressed) {
                if (in.action == 0x16) Answer(yes); else if (in.action == 0x17) Answer(no); else if (in.action == 0x18) Answer(alt);
            }
            break;
        case 45:
            if (yes == 5 && in.stick && in.type == 2) {
                if (in.y < -0.2f) ChangeBet(false); else if (in.y > 0.2f) ChangeBet(true);
                if (in.x < -0.2f) base = def.MinBet; else if (in.x > 0.2f) base = last;
                if (base > money) base = money ? money : def.MinBet;
                if (base > def.MaxBet) base = def.MaxBet;
                ++performed;
            }
            if (in.pressed) {
                if ((base != 0 || yes != 5) && in.action == 0x16) Answer(yes);
                else if (in.action == 0x17) Answer(no);
            }
            break;
        case 44:
            if (in.pressed && (in.action == 0x14 || in.action == 6)) Answer(yes);
            break;
        case 47:
            ++performed;
            break;
        }
        if (state == 13 && in.pressed && in.action == 0x16) { SetState(next); ++performed; }
    }
};
int main()
{
    Def def = { 10, 100 };
    Game g = {};
    g.state = 45; g.yes = 5; g.no = 9; g.last = 40; g.money = 30; g.base = 20;
    Input stick = { false, 0x72, true, 0.5f, 0.0f, 2 };
    g.ProcessInput(stick, def);                      // right -> LastBet 40, clamped to money 30
    bool ok = g.base == 30 && g.performed == 1 && g.state == 45;
    Input left = { false, 0x72, true, -0.5f, 0.5f, 2 };
    g.money = 0; g.ProcessInput(left, def);           // MinBet; up -> ChangeBet(true)
    ok = ok && g.base == 10 && g.bets == 1;
    Input yesPress = { true, 0x16, false, 0, 0, 0 };
    g.ProcessInput(yesPress, def);                    // answer yes -> state 5, prompts cleared
    ok = ok && g.state == 5 && g.yes == 0 && g.no == 0 && g.clears == 1;
    Input quit = { true, 0x53, false, 0, 0, 0 };
    g.ProcessInput(quit, def);
    ok = ok && g.finished && !g.won && g.yesno == 1;
    g.state = 13; g.next = 7; g.ProcessInput(yesPress, def);
    ok = ok && g.state == 7;
    g.state = 46; g.alt = 3; g.ProcessInput(quit, def);   // 0x53 is not an answer on 46
    ok = ok && g.state == 46;
    Input altPress = { true, 0x18, false, 0, 0, 0 };
    g.ProcessInput(altPress, def);
    ok = ok && g.state == 3 && g.alt == 0;
    if (!ok) { std::printf("005ec270_TEST FAIL\n"); return 1; }
    std::printf("005ec270_TEST PASS\n");
    return 0;
}