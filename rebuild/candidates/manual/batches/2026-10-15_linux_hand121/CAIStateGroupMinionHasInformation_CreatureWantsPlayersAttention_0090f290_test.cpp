#include <cstdio>
// CreatureWantsPlayersAttention: flag bit2 set, bit5 clear, hero alive, perceive component sees the player
struct Thing { unsigned char flags1c8; unsigned char flags91; bool hasPerceive; bool extendedLOS;
    bool HasInformationFlag() { return (flags1c8 >> 2) & 1; } bool InformationGivenFlag() { return (flags1c8 >> 5) & 1; } };
static Thing* g_hero = 0;
struct Group {
    Thing* creature;
    bool Run()
    {
        if (creature->HasInformationFlag()) {
            if (!creature->InformationGivenFlag()) {
                Thing* hero = g_hero;
                if (hero != 0 && !(hero->flags91 & 1)) {
                    if (creature->hasPerceive) return creature->extendedLOS;
                }
            }
        }
        return false;
    }
};
int main()
{
    Thing c; c.flags1c8 = 4; c.flags91 = 0; c.hasPerceive = true; c.extendedLOS = true;
    Thing h; h.flags1c8 = 0; h.flags91 = 0; h.hasPerceive = false; h.extendedLOS = false; g_hero = &h;
    Group g; g.creature = &c;
    if (!g.Run()) { std::printf("0090f290_TEST FAIL\n"); return 1; }
    c.extendedLOS = false; if (g.Run()) { std::printf("0090f290_TEST FAIL\n"); return 1; }
    c.extendedLOS = true; c.hasPerceive = false; if (g.Run()) { std::printf("0090f290_TEST FAIL\n"); return 1; }
    c.hasPerceive = true; h.flags91 = 1; if (g.Run()) { std::printf("0090f290_TEST FAIL\n"); return 1; }
    h.flags91 = 0; c.flags1c8 = 4 | 32; if (g.Run()) { std::printf("0090f290_TEST FAIL\n"); return 1; }
    c.flags1c8 = 0; if (g.Run()) { std::printf("0090f290_TEST FAIL\n"); return 1; }
    c.flags1c8 = 4; g_hero = 0; if (g.Run()) { std::printf("0090f290_TEST FAIL\n"); return 1; }
    std::printf("0090f290_TEST PASS\n"); return 0;
}
