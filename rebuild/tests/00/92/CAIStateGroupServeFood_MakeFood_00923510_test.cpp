// Behaviour test for CAIStateGroup_ServeFood::MakeFood (0x00923510): same steps over mocks.
#include <cstdio>
#include <cstring>
class CThing { public: unsigned char flags91; int components; CThing* creator; CThing* owner; float x, y, z; long player; long def; };
static CThing g_made;
static long DefIndex(const char* name) { return std::strcmp(name, "OBJECT_VILLAGE_TAVERN_APPLE") == 0 ? 42 : -1; }
static CThing* Create(long def, const float* pos, long player)
{
    std::memset(&g_made, 0, sizeof(g_made));
    g_made.def = def; g_made.x = pos[0]; g_made.y = pos[1]; g_made.z = pos[2]; g_made.player = player;
    return &g_made;
}
static CThing* MakeFood(const float* pos, CThing* owner, long mainPlayer)
{
    long def = DefIndex("OBJECT_VILLAGE_TAVERN_APPLE");
    CThing* food = Create(def, pos, mainPlayer);
    food->flags91 |= 2;
    ++food->components; food->creator = owner;
    ++food->components; food->owner = owner;
    return food;
}
int main()
{
    CThing keeper; std::memset(&keeper, 0, sizeof(keeper));
    float pos[3] = { 1.0f, 2.0f, 3.0f };
    CThing* f = MakeFood(pos, &keeper, 7);
    bool ok = f == &g_made && f->def == 42 && f->player == 7 && f->x == 1.0f && f->z == 3.0f
        && (f->flags91 & 2) && f->components == 2 && f->creator == &keeper && f->owner == &keeper;
    if (!ok) { std::printf("00923510_TEST FAIL\n"); return 1; }
    std::printf("00923510_TEST PASS\n");
    return 0;
}