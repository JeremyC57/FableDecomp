// Test for 0x005EB9E0 CTCTavernGame::FrameUpdate: self-contained behaviour model of the per-frame driver and the
// inlined SetState (quit request -> 47 -> LeaveGame; 5 marks the game played), checked on the quit and state paths.
#include <cstdio>

static bool g_quit = false;
struct Game {
    bool played, gm_alive, hostile, hit, displaying, prize_before, prize_after, update_ok, draw, won, best;
    int state, prev, next, mode, base_bet, money, min_bet, left, newgame, newround, endround, midquit, sound, rendered;
    int ok_next, yn_next, alt_next, instr_next, text_calls;
    float timer;
    void LeaveGame() { ++left; }
    void SetState(int s)
    {
        if (g_quit) { g_quit = false; s = 47; }
        if (s == 5) played_flag = true;
        else if (s == 47) { LeaveGame(); return; }
        prev = state; state = s;
    }
    bool played_flag;
    bool UpdateValues() { prize_before = prize_after; return update_ok; }
    void FrameUpdate()
    {
        if (played) {
            if (gm_alive) {
                if (!displaying && mode != 26) mode = 26;
                if (hostile || hit) { SetState(47); return; }
            } else { SetState(47); return; }
        }
        switch (state) {
        case 1: ok_next = 2; break;
        case 2:
            if (base_bet > money) base_bet = money == 0 ? min_bet : money;
            if (money > 0) alt_next = 4; else ok_next = 10;
            break;
        case 3: instr_next = 2; break;
        case 4: yn_next = 5; break;
        case 5: ++sound; ++newgame; break;
        case 7: ++newround; break;
        case 6: ++endround; break;
        case 8: ok_next = 9; break;
        case 9: {
            bool given = prize_before;
            if (UpdateValues()) ok_next = given != prize_before ? 12 : 2;
            else SetState(2);
            break;
        }
        case 10: ok_next = 47; break;
        case 14: if (timer <= 0.0f) SetState(next); break;
        case 11: ++midquit; break;
        case 12: SetState(2); break;
        }
        if (timer > 0.0f) timer -= 1.0f / 30;
        rendered = 4;
    }
};

static int fail(int n) { std::printf("005eb9e0_TEST FAIL %d\n", n); return 1; }

int main()
{
    Game g = Game();
    g.played = true; g.gm_alive = false; g.state = 2;
    g.FrameUpdate();
    if (g.left != 1 || g.state != 2 || g.rendered != 0) return fail(1);

    g = Game(); g.played = true; g.gm_alive = true; g.hit = true; g.state = 3;
    g.FrameUpdate();
    if (g.left != 1 || g.mode != 26 || g.instr_next != 0) return fail(2);

    g = Game(); g.played = true; g.gm_alive = true; g.displaying = true; g.state = 3;
    g.FrameUpdate();
    if (g.mode != 0 || g.instr_next != 2 || g.rendered != 4) return fail(3);

    g = Game(); g.state = 2; g.base_bet = 50; g.money = 0; g.min_bet = 5;
    g.FrameUpdate();
    if (g.base_bet != 5 || g.ok_next != 10 || g.alt_next != 0) return fail(4);

    g = Game(); g.state = 2; g.base_bet = 50; g.money = 20;
    g.FrameUpdate();
    if (g.base_bet != 20 || g.alt_next != 4) return fail(5);

    g = Game(); g.state = 9; g.update_ok = true; g.prize_after = true;
    g.FrameUpdate();
    if (g.ok_next != 12) return fail(6);

    g = Game(); g.state = 9; g.update_ok = false;
    g.FrameUpdate();
    if (g.state != 2 || g.prev != 9) return fail(7);

    g = Game(); g.state = 14; g.next = 5; g.timer = 0.0f;
    g.FrameUpdate();
    if (g.state != 5 || !g.played_flag) return fail(8);

    g = Game(); g.state = 12; g_quit = true;
    g.FrameUpdate();
    if (g_quit || g.left != 1 || g.state != 12) return fail(9);

    g = Game(); g.state = 5; g.timer = 1.0f;
    g.FrameUpdate();
    if (g.sound != 1 || g.newgame != 1 || !(g.timer < 1.0f && g.timer > 0.9f)) return fail(10);

    std::printf("005eb9e0_TEST PASS\n");
    return 0;
}