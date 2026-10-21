#include <cstdio>
#include <cstring>
// Behaviour copy of CAS_Buddy::Init (0x00E2BFA0): the interface calls in order, the flags cleared, the start position latched.
struct V3 { float X, Y, Z; };
struct Log { int n; const char* calls[16]; float wanderMax; long stateGroup; V3 centre; bool follow; bool info[3];
    void add(const char* c) { calls[n++] = c; } };
struct Buddy {
    Log* game; V3 myPos; bool a, b, c; V3 startPos; float defDistance;
    void Init() {
        game->add("ally(me,hero)"); game->add("ally(hero,me)");
        game->add("follow"); game->follow = true;
        a = false; b = false; c = false;
        game->add("info"); game->info[0] = true; game->info[1] = false; game->info[2] = false;
        startPos = myPos;
        game->add("centre"); game->centre = startPos;
        game->add("min");
        game->add("max"); game->wanderMax = defDistance;
        game->add("group"); game->stateGroup = 4;
    }
};
int main()
{
    Log g; std::memset(&g, 0, sizeof g);
    V3 p = { 1.5f, -2.0f, 3.25f };
    Buddy bd; bd.game = &g; bd.myPos = p; bd.a = bd.b = bd.c = true; bd.defDistance = 12.5f;
    bd.Init();
    const char* expect[] = { "ally(me,hero)", "ally(hero,me)", "follow", "info", "centre", "min", "max", "group" };
    if (g.n != 8) { std::printf("00e2bfa0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) if (std::strcmp(g.calls[i], expect[i]) != 0) { std::printf("00e2bfa0_TEST FAIL\n"); return 1; }
    if (bd.a || bd.b || bd.c || bd.startPos.X != 1.5f || bd.startPos.Z != 3.25f || g.centre.Y != -2.0f || g.wanderMax != 12.5f || g.stateGroup != 4 || !g.follow || !g.info[0] || g.info[1]) { std::printf("00e2bfa0_TEST FAIL\n"); return 1; }
    std::printf("00e2bfa0_TEST PASS\n"); return 0;
}