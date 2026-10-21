// Behaviour test for 00d9d4f0 (CQ_HobbeToothContestScript::CQ_HobbeToothWhisper::Init): the parent gets a spawned
// "ParentClass.CheckToSeeIfWhisperHit" thread bound to this Whisper (empty region), then the Whisper joins the hero's
// faction, is made unkillable, gets boss phase 2 and no information marker.
#include <cstdio>
#include <cstring>

struct Thing { int id; };
struct Parent { char spawned[64]; char region[8]; int spawnedOn; int count;
    void Add(const char* name, const char* rgn, const Thing& par) { std::strcpy(spawned, name); std::strcpy(region, rgn); spawnedOn = par.id; ++count; } };
struct Game { char faction[32]; int killable, killB, phase, infoA, infoB, infoC; };
struct Whisper {
    Game* game; Thing Me; Parent* ParentClass;
    void Init()
    {
        char name[64]; std::strcpy(name, "ParentClass."); std::strcat(name, "CheckToSeeIfWhisperHit");
        ParentClass->Add(name, "", Me);
        std::strcpy(game->faction, "FACTION_HERO");
        game->killable = 0; game->killB = 1;
        game->phase = 2;
        game->infoA = 0; game->infoB = 1; game->infoC = 0;
    }
};
int main()
{
    Parent p; std::memset(&p, 0, sizeof p); Game g; std::memset(&g, 0, sizeof g); g.killable = 1;
    Whisper w; w.game = &g; w.Me.id = 42; w.ParentClass = &p;
    w.Init();
    int ok = p.count == 1 && std::strcmp(p.spawned, "ParentClass.CheckToSeeIfWhisperHit") == 0 && p.region[0] == 0 &&
             p.spawnedOn == 42 && std::strcmp(g.faction, "FACTION_HERO") == 0 && g.killable == 0 && g.killB == 1 &&
             g.phase == 2 && g.infoA == 0 && g.infoB == 1 && g.infoC == 0;
    if (!ok) { std::printf("00d9d4f0_TEST FAIL\n"); return 1; }
    std::printf("00d9d4f0_TEST PASS\n");
    return 0;
}
