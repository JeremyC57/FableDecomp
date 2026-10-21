#include <cstdio>
#include <cstring>
// Behaviour copy of CAIStateGroup_ChickenPanic::IsRunnable (0x00911C20).
struct V { float x, y, z; };
struct Thing { V pos; Thing* attacker; long frameAttacked; bool hasEvents; const char* hitEmote; Thing* hitBy; Thing* setAttacker; };
static long g_frame = 100; static long g_memory = 15;
static bool IsRunnable(Thing* c) {
    if (c->attacker) {
        const V& a = c->attacker->pos; const V& p = c->pos; float dx = p.x - a.x, dy = p.y - a.y;
        if (dx * dx + dy * dy < 64.0f) {
            long attacked = c->frameAttacked; long memory = g_memory;
            if ((float)(unsigned long)(g_frame - attacked) < (float)memory * 10.0f) return true;
        }
    }
    if (c->hasEvents && c->hitEmote) {
        bool airGuitar = std::strcmp(c->hitEmote, "EXPRESSION_AIR_GUITAR") == 0;
        if (airGuitar) { c->setAttacker = c->hitBy; return true; }
    }
    return false;
}
int main() {
    Thing att = { {0, 0, 0}, 0, 0, false, 0, 0, 0 };
    Thing c = { {1, 1, 0}, &att, 0, false, 0, 0, 0 };
    if (!IsRunnable(&c)) { std::printf("00911c20_TEST FAIL\n"); return 1; }       // 100 frames < 150
    c.frameAttacked = -100;                                                        // 200 frames: too long ago
    if (IsRunnable(&c)) { std::printf("00911c20_TEST FAIL\n"); return 1; }
    c.frameAttacked = 200;                                                         // negative -> huge unsigned
    if (IsRunnable(&c)) { std::printf("00911c20_TEST FAIL\n"); return 1; }
    c.hasEvents = true; c.hitEmote = "EXPRESSION_AIR_GUITAR"; c.hitBy = &att;
    if (!IsRunnable(&c) || c.setAttacker != &att) { std::printf("00911c20_TEST FAIL\n"); return 1; }
    c.hitEmote = "EXPRESSION_WAVE"; c.setAttacker = 0;
    if (IsRunnable(&c) || c.setAttacker) { std::printf("00911c20_TEST FAIL\n"); return 1; }
    std::printf("00911c20_TEST PASS\n"); return 0;
}
