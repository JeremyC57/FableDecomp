#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_added = 0; static void* g_addedThing = 0; static long g_addedIndex = -1;
class CThing { public: unsigned char pad[0x91]; unsigned char dead : 1; unsigned char rest : 7; };
static CThing g_hero; static CThing* g_heroPtr = &g_hero;
class CPlayer { public: CThing* GetHeroThing() { return g_heroPtr; } };
class CPlayerManager { public: CPlayer p; CPlayer* GetPlayer() { return &p; } };
class CGame { public: unsigned char pad[0x1c]; CPlayerManager* pm; };
static CGame* g_pGame;
struct CArray { long* b; long* e; long& front() { return *b; } };
struct CGuiDef { unsigned char pad[0x958]; CArray graphics; };
static CGuiDef g_def; static CGuiDef* PeekGuiDef() { return &g_def; }
struct CMiniMap { virtual ~CMiniMap() { } unsigned char pad[0xb4]; void* region;
    void AddMarker(const CThing* t, long g) { ++g_added; g_addedThing = (void*)t; g_addedIndex = g; }
    void UpdateMarkersInternal() { if (region) { CThing* hero = g_pGame->pm->GetPlayer()->GetHeroThing(); if (hero && !hero->dead) AddMarker(hero, PeekGuiDef()->graphics.front()); } } };
int main()
{
    CPlayerManager pm; CGame game; game.pm = &pm; g_pGame = &game; long gr[2] = { 42, 43 }; g_def.graphics.b = gr; g_def.graphics.e = gr + 2; g_hero.dead = 0;
    CMiniMap mm; int r; mm.region = &r;
    mm.UpdateMarkersInternal(); if (g_added != 1 || g_addedThing != &g_hero || g_addedIndex != 42) { std::printf("0082a960_TEST FAIL\n"); return 1; }
    g_hero.dead = 1; mm.UpdateMarkersInternal(); if (g_added != 1) { std::printf("0082a960_TEST FAIL\n"); return 1; }
    g_hero.dead = 0; g_heroPtr = 0; mm.UpdateMarkersInternal(); if (g_added != 1) { std::printf("0082a960_TEST FAIL\n"); return 1; }
    g_heroPtr = &g_hero; mm.region = 0; mm.UpdateMarkersInternal(); if (g_added != 1) { std::printf("0082a960_TEST FAIL\n"); return 1; }
    mm.region = &r; mm.UpdateMarkersInternal(); if (g_added != 2) { std::printf("0082a960_TEST FAIL\n"); return 1; }
    std::printf("0082a960_TEST PASS\n");
    return 0;
}