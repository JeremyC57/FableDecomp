#include <cstdio>
// Behaviour copy of CAIStateGroup_TavernServer::InitStateGroup (0x00966930).
struct V { float X, Y, Z; };
struct State { int jug, bar, table, tankard, pub; long serving, barPoint, action; float speed; };
static float WalkSpeed(const V& d) { return 1.25f + d.X + d.Y + d.Z; }
static void Init(State& s) {
    V direction = { 0.0f, 0.0f, 0.0f };
    s.speed = WalkSpeed(direction);
    s.jug = 0; s.bar = 0; s.table = 0; s.pub = 0; s.tankard = 0;
    s.serving = -1; s.barPoint = -1; s.action = -1;
}
int main() {
    State s = { 1, 2, 3, 4, 5, 6, 7, 8, 0.0f };
    Init(s);
    if (s.jug || s.bar || s.table || s.tankard || s.pub || s.serving != -1 || s.barPoint != -1 || s.action != -1 || s.speed != 1.25f) { std::printf("00966930_TEST FAIL\n"); return 1; }
    std::printf("00966930_TEST PASS\n"); return 0;
}