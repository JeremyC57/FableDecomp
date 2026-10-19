#include <cstdio>
struct Thing { bool dead; unsigned long bits; bool targetDead; unsigned char targetType; bool extended; float awareness; };
static bool aware(const Thing& t, float zero)
{
    if (t.dead || !(t.bits & 0x100000)) return false;
    if (t.extended) return true;
    if (t.awareness == zero) return false;
    return !t.targetDead && t.targetType == 2;
}
int main()
{
    Thing t = {false, 0x100000, false, 2, false, 1.0f};
    if (!aware(t, 0.0f)) { std::printf("008d0ba0_TEST FAIL\n"); return 1; }
    t.dead = true;
    if (aware(t, 0.0f)) { std::printf("008d0ba0_TEST FAIL\n"); return 1; }
    t.dead = false; t.extended = true;
    if (!aware(t, 0.0f)) { std::printf("008d0ba0_TEST FAIL\n"); return 1; }
    std::printf("008d0ba0_TEST PASS\n"); return 0;
}
