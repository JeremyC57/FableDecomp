// Behaviour test for CTCCreatureExpression::UpdateBlinking (0x00728FF0): the seed step, the 1-in-50 start and the stop.
#include <cstdio>

static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Next(unsigned long* s) { *s = *s * 0x24a1 + 0x24df; *s = Ror13(*s); return *s; }
struct Anim { bool present; bool finished; int started; int removed; };
struct Expr {
    bool Blinking; bool hasBlink;
    void UpdateBlinking(Anim& a, unsigned long* seed)
    {
        if (Next(seed) % 50 == 0 && !Blinking) {
            if (hasBlink) { ++a.started; a.present = true; a.finished = false; Blinking = true; }
        }
        if (Blinking) {
            if (a.present) {
                if (!a.finished) return;
                ++a.removed; a.present = false;
            }
            Blinking = false;
        }
    }
};

int main()
{
    unsigned long s = 1;
    unsigned long v = Next(&s);
    if (v != Ror13(1 * 0x24a1 + 0x24df) || s != v) { std::printf("00728ff0_TEST FAIL\n"); return 1; }
    // find a seed whose next value is a multiple of 50
    unsigned long seed = 0;
    for (unsigned long t = 0; t < 100000; ++t) { unsigned long c = t; if (Next(&c) % 50 == 0) { seed = t; break; } }
    Anim a = { false, false, 0, 0 };
    Expr e = { false, true };
    unsigned long s2 = seed; e.UpdateBlinking(a, &s2);
    if (!e.Blinking || a.started != 1) { std::printf("00728ff0_TEST FAIL\n"); return 1; }
    unsigned long s3 = 7; e.UpdateBlinking(a, &s3);
    if (!e.Blinking || a.removed) { std::printf("00728ff0_TEST FAIL\n"); return 1; }
    a.finished = true; e.UpdateBlinking(a, &s3);
    if (e.Blinking || a.removed != 1) { std::printf("00728ff0_TEST FAIL\n"); return 1; }
    std::printf("00728ff0_TEST PASS\n");
    return 0;
}
