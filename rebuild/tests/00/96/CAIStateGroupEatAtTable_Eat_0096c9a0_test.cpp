#include <cstdio>
// Behaviour copy of CAIStateGroup_EatAtTable::Eat (0x0096C9A0).
struct Group {
    unsigned long seed; long frame; long fps; int results[16]; int next; int loops; int putDown; long endFrame;
    unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
    unsigned long Random(unsigned long range) { seed = seed * 0x24A1U + 0x24DFU; seed = Ror13(seed); return seed % range; }
    int Perform() { return results[next++]; }
    bool Eat() {
        if (Perform() != 2) return false;
        endFrame = frame + (long)Random(7) * fps;
        do { ++loops; frame += 10; if (Perform() != 2) return false; } while (endFrame > frame);
        ++putDown;
        if (Perform() != 2) return false;
        return true;
    }
};
int main() {
    Group g = { 12345, 0, 30, {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, 0, 0, 0, 0 };
    unsigned long s = 12345 * 0x24A1U + 0x24DFU; s = (s >> 13) | (s << 19); long expectEnd = (long)(s % 7) * 30;
    bool ok = g.Eat();
    long expectLoops = expectEnd <= 0 ? 1 : (expectEnd + 9) / 10;
    if (!ok || g.endFrame != expectEnd || g.loops != expectLoops || g.putDown != 1) { std::printf("0096c9a0_TEST FAIL\n"); return 1; }
    Group h = { 1, 0, 30, {3}, 0, 0, 0, 0 };
    if (h.Eat() || h.loops != 0) { std::printf("0096c9a0_TEST FAIL\n"); return 1; }
    Group k = { 1, 0, 30, {2, 0}, 0, 0, 0, 0 };
    if (k.Eat() || k.loops != 1 || k.putDown != 0) { std::printf("0096c9a0_TEST FAIL\n"); return 1; }
    std::printf("0096c9a0_TEST PASS\n"); return 0;
}