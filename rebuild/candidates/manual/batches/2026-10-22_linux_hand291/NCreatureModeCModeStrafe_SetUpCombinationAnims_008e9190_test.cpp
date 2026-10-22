// Behaviour test for CModeStrafe::SetUpCombinationAnims (0x008E9190): clear then conditionally re-cache four animation entries.
#include <cstdio>
#include <cstring>

struct Block { long refs; bool destroyed; };
struct Ptr { int object; Block* count; };
static void Clear(Ptr& p)
{
    if (p.count) {
        --p.count->refs;
        if (p.count->refs == 0)
            p.count->destroyed = true;
    }
    p.count = 0;
    p.object = 0;
}
static const char* const kNames[4] = { "PROJECTILE_WEAPON_AIM", "PROJECTILE_WEAPON_BUILDUP", "PROJECTILE_WEAPON_PULLBACK", "STRAFE_UPPER_BODY" };
static bool Has(const char* name, int mask)
{
    for (int i = 0; i < 4; ++i)
        if (std::strcmp(name, kNames[i]) == 0)
            return (mask >> i) & 1;
    return false;
}
static Block g_blocks[8];
static int g_next = 0;
static void SetUp(Ptr slots[4], int mask)
{
    for (int i = 0; i < 4; ++i) {
        Clear(slots[i]);
        if (Has(kNames[i], mask)) {
            slots[i].object = 100 + i;
            g_blocks[g_next].refs = 1;
            g_blocks[g_next].destroyed = false;
            slots[i].count = &g_blocks[g_next++];
        }
    }
}
int main()
{
    Ptr slots[4];
    std::memset(slots, 0, sizeof(slots));
    SetUp(slots, 0x5);   // AIM and PULLBACK only
    int ok = slots[0].object == 100 && slots[1].count == 0 && slots[2].object == 102 && slots[3].object == 0;
    Block* oldAim = slots[0].count;
    oldAim->refs = 2;    // shared elsewhere: survives the clear
    SetUp(slots, 0xa);   // BUILDUP and UPPER_BODY only
    ok = ok && !oldAim->destroyed && oldAim->refs == 1 && slots[0].count == 0 && slots[1].object == 101
         && slots[3].object == 103 && g_blocks[1].destroyed;
    std::printf(ok ? "008e9190_TEST PASS\n" : "008e9190_TEST FAIL\n");
    return ok ? 0 : 1;
}
