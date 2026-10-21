#include <cstdio>
// Behaviour copy of CAIStateGroup_JackDragon::Update (0x00952E80): latch the enemy, loop the state handlers.
struct Sim { int ticksAlive; bool stopping; long state; int calls[6]; int phaseUps; bool ready; int terminated; int enemy; };
static void Update(Sim& s, int character) {
    s.enemy = character;
    while (s.enemy != 0 && s.ticksAlive > 0 && !s.stopping) {
        if (s.ready) { ++s.phaseUps; s.ready = false; }
        switch (s.state) {
        case 0: ++s.calls[0]; s.state = 1; break;
        case 1: ++s.calls[1]; s.state = 2; break;
        case 2: ++s.calls[2]; s.state = 3; break;
        case 3: ++s.calls[3]; s.state = 4; break;
        case 4: ++s.calls[4]; s.state = 5; break;
        case 5: ++s.calls[5]; s.state = 9; break;
        }
        --s.ticksAlive;
    }
    ++s.terminated;
}
int main() {
    Sim s = { 8, false, 0, { 0, 0, 0, 0, 0, 0 }, 0, true, 0, 0 };
    Update(s, 7);
    for (int i = 0; i < 6; ++i) if (s.calls[i] != 1) { std::printf("00952e80_TEST FAIL\n"); return 1; }
    if (s.phaseUps != 1 || s.terminated != 1 || s.enemy != 7 || s.ticksAlive != 0) { std::printf("00952e80_TEST FAIL\n"); return 1; }
    Sim t = { 5, true, 0, { 0, 0, 0, 0, 0, 0 }, 0, false, 0, 0 };
    Update(t, 3); if (t.calls[0] != 0 || t.terminated != 1) { std::printf("00952e80_TEST FAIL\n"); return 1; }
    std::printf("00952e80_TEST PASS\n"); return 0;
}
