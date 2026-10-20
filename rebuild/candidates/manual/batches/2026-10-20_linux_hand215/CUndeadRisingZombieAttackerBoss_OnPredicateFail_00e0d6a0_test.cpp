// Behaviour test for CUndeadRisingZombieAttackerBoss::OnPredicateFail (00e0d6a0): one death effect at Me's position.
#include <cstdio>
#include <cstring>
struct V { float X, Y, Z; };
struct Thing { V pos; const V& GetPos() const { return pos; } };
static int g_effects = 0; static char g_name[32]; static V g_at; static float g_angle = -1;
struct Handle { int id; ~Handle() { } };
struct Game {
    Handle CreateEffect(const char* name, const V& at, const char* bone, float angle, bool a, bool b)
    { ++g_effects; std::strcpy(g_name, name); g_at = at; g_angle = angle; Handle h; h.id = (bone[0] == 0 && !a && !b) ? 1 : 0; return h; }
};
struct Boss { Game* game; Thing Me; void OnPredicateFail() { Handle effect = game->CreateEffect("ZOMBIE_BOSS1_DEATH", Me.GetPos(), "", 0.0f, false, false); } };
int main()
{
    Game g; Boss b; b.game = &g; b.Me.pos.X = 1; b.Me.pos.Y = 2; b.Me.pos.Z = 3;
    b.OnPredicateFail();
    bool ok = g_effects == 1 && !std::strcmp(g_name, "ZOMBIE_BOSS1_DEATH") && g_at.Y == 2 && g_angle == 0.0f;
    if (!ok) { std::printf("00e0d6a0_TEST FAIL\n"); return 1; }
    std::printf("00e0d6a0_TEST PASS\n");
    return 0;
}
