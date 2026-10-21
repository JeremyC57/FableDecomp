// behaviour test for 00e72f00 CDF_ThreeSuits::DressedAsBandit
#include <cstdio>
#include <cstring>
struct Game {
    unsigned worn; int calls;
    bool IsWearing(const char* item)
    {
        static const char* names[5] = { "OBJECT_HERO_SHIRT_BANDITCAMP", "OBJECT_HERO_TROUSERS_BANDITCAMP",
            "OBJECT_HERO_HAT_BANDITCAMP", "OBJECT_HERO_BOOTS_BANDITCAMP", "OBJECT_HERO_GLOVES_BANDITCAMP" };
        ++calls;
        for (int i = 0; i < 5; ++i)
            if (std::strcmp(names[i], item) == 0) return (worn >> i) & 1;
        return false;
    }
};
static bool DressedAsBandit(Game& g)
{
    return g.IsWearing("OBJECT_HERO_SHIRT_BANDITCAMP") && g.IsWearing("OBJECT_HERO_TROUSERS_BANDITCAMP")
        && g.IsWearing("OBJECT_HERO_HAT_BANDITCAMP") && g.IsWearing("OBJECT_HERO_BOOTS_BANDITCAMP")
        && g.IsWearing("OBJECT_HERO_GLOVES_BANDITCAMP");
}
int main()
{
    int bad = 0;
    Game g; g.worn = 0x1f; g.calls = 0;
    if (!DressedAsBandit(g) || g.calls != 5) bad |= 1;
    g.worn = 0x1d; g.calls = 0;                       // trousers missing: stops after the second check
    if (DressedAsBandit(g) || g.calls != 2) bad |= 2;
    g.worn = 0x0f; g.calls = 0;                       // gloves missing
    if (DressedAsBandit(g) || g.calls != 5) bad |= 4;
    g.worn = 0; g.calls = 0;
    if (DressedAsBandit(g) || g.calls != 1) bad |= 8;
    if (bad) { std::printf("00e72f00_TEST FAIL\n"); return 1; }
    std::printf("00e72f00_TEST PASS\n");
    return 0;
}