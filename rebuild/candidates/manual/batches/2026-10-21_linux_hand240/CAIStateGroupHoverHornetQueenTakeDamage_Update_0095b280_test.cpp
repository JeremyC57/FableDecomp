#include <cstdio>
// Behaviour copy of CAIStateGroup_HoverHornetQueenTakeDamage::Update (0x0095B280).
struct Boss { bool ready; int phase; };
struct State { bool rangeAttack; long count; long lastHit; bool combatant; int registered; int readied; int roars; int terminated; };
static void Update(State& s, Boss& b, long frame) {
    if (b.ready) ++b.phase;
    if (s.rangeAttack) {
        long elapsed = frame - s.lastHit;
        s.lastHit = frame;
        if (elapsed < 90) {
            if (++s.count >= 3) {
                if (!s.combatant) { ++s.registered; s.combatant = true; }
                if (s.combatant) ++s.readied;
                ++s.roars; ++s.terminated; return;
            }
        } else s.count = 1;
    }
    ++s.terminated;
}
int main() {
    Boss b = { true, 3 };
    State s = { true, 0, 0, false, 0, 0, 0, 0 };
    Update(s, b, 200); if (s.count != 1 || s.lastHit != 200 || b.phase != 4 || s.terminated != 1) { std::printf("0095b280_TEST FAIL\n"); return 1; }
    b.ready = false;
    Update(s, b, 250); if (s.count != 2 || s.roars != 0) { std::printf("0095b280_TEST FAIL\n"); return 1; }
    Update(s, b, 300); if (s.count != 3 || s.roars != 1 || s.registered != 1 || s.readied != 1 || s.terminated != 3 || b.phase != 4) { std::printf("0095b280_TEST FAIL\n"); return 1; }
    s.rangeAttack = false; Update(s, b, 310); if (s.lastHit != 300 || s.terminated != 4) { std::printf("0095b280_TEST FAIL\n"); return 1; }
    std::printf("0095b280_TEST PASS\n"); return 0;
}
