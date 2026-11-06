// Behaviour test for CContextSensitiveItemDef::IsValidOverride (0x00850A80), three of its rules: a dead or missing thing never
// qualifies; a health item (Type 3) applies when the deficit reaches ExtraData1, when ExtraData2 is 0 or when ExtraData2 is at least
// the current health; an opinion expression (Type 6) is refused when the slot repeats the last expression (after more than one) or
// the last but one (after more than two). A model.
#include <cstdio>
bool Health(bool dead, float maxHealth, float health, long extra1, long extra2)
{
    if (dead) return false;
    return maxHealth - health >= extra1 || extra2 == 0 || extra2 >= health;
}
bool Repeat(long count, long last, long lastButOne, long slot)
{
    if (count > 1) {
        if (last == slot) return false;
        if (count > 2 && lastButOne == slot) return false;
    }
    return true;
}
int main()
{
    bool ok = !Health(true, 100.0f, 10.0f, 0, 0) && Health(false, 100.0f, 50.0f, 50, 5) && !Health(false, 100.0f, 60.0f, 50, 5);
    ok = ok && Health(false, 100.0f, 99.0f, 50, 0) && Health(false, 100.0f, 10.0f, 50, 20);
    ok = ok && Repeat(1, 3, 3, 3) && !Repeat(2, 3, 4, 3) && Repeat(2, 4, 3, 3) && !Repeat(3, 4, 3, 3) && Repeat(3, 4, 5, 3);
    std::printf(ok ? "00850a80_TEST PASS\n" : "00850a80_TEST FAIL\n");
    return ok ? 0 : 1;
}
