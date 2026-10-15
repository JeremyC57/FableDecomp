// Behaviour test for 0x00782A90 CTCBoastingArea::OnKill: only a living hero inside the radius (state 2)
// triggers the leaving-area handling.
#include <cstdio>

struct CThing { unsigned char m_dead : 1; };
static CThing* g_hero;
static CThing* g_left;
static int g_calls;
struct CPlayer { CThing* GetHeroCreature() { return g_hero; } };
static CPlayer g_player;
struct CPlayerManager { CPlayer* GetMainPlayer() { return &g_player; } };
static CPlayerManager g_manager;
static CPlayerManager* GetPlayerManager() { return &g_manager; }
enum EHeroWithinRadiusState { HERO_OUTSIDE = 0, HERO_WITHIN_ANTICIPATION = 1, HERO_WITHIN_RADIUS = 2 };
struct CTCBoastingArea {
    EHeroWithinRadiusState HeroWithinRadiusState;
    void OnHeroLeavingArea(CThing* hero) { ++g_calls; g_left = hero; }
    void OnKill()
    {
        if (HeroWithinRadiusState != HERO_OUTSIDE) {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
            if (hero && !hero->m_dead && HeroWithinRadiusState == HERO_WITHIN_RADIUS)
                OnHeroLeavingArea(hero);
        }
    }
};

static int Run(EHeroWithinRadiusState state, CThing* hero)
{
    CTCBoastingArea a; a.HeroWithinRadiusState = state;
    g_hero = hero; g_calls = 0; g_left = 0;
    a.OnKill();
    return g_calls;
}

int main()
{
    CThing alive; alive.m_dead = 0;
    CThing dead; dead.m_dead = 1;
    if (Run(HERO_WITHIN_RADIUS, &alive) != 1 || g_left != &alive) { std::printf("00782a90_TEST FAIL\n"); return 1; }
    if (Run(HERO_WITHIN_RADIUS, &dead) != 0 || Run(HERO_WITHIN_RADIUS, 0) != 0) { std::printf("00782a90_TEST FAIL\n"); return 1; }
    if (Run(HERO_WITHIN_ANTICIPATION, &alive) != 0 || Run(HERO_OUTSIDE, &alive) != 0) { std::printf("00782a90_TEST FAIL\n"); return 1; }
    std::printf("00782a90_TEST PASS\n");
    return 0;
}