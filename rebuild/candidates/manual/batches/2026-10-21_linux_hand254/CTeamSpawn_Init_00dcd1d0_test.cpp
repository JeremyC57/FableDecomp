// Behaviour test for CTeamSpawn::Init (0x00DCD1D0): the other spawn point is taken from a random EitherTeamSpawn
// marker (ref counts balance, the old handle is released), Me named BanditTeamSpawn makes team 1 with bandit members,
// anything else team 0 with guards; the hero's team gets limit 2 and the your-team respawn time, otherwise limit 3 and
// the enemy-team time; BanditsLeftID is cleared.
#include <cstdio>
#include <cstring>
#include <string>

static int g_live = 0;
struct CCharString {
    std::string s;
    CCharString(const char* t = "", long = -1) : s(t) {}
    CCharString& operator=(const char* t) { s = t; return *this; }
    bool operator==(const char* t) const { return s == t; }
};
struct CScriptThing {
    int id; CCharString name;
    CScriptThing(int i = 0, const char* n = "") : id(i), name(n) { ++g_live; }
    CScriptThing(const CScriptThing& o) : id(o.id), name(o.name) { ++g_live; }
    CScriptThing& operator=(const CScriptThing& o) { id = o.id; name = o.name; return *this; }
    ~CScriptThing() { --g_live; }
    const CCharString& GetName() const { return name; }
};
struct CGameScriptInterface {
    CScriptThing GetRandomThingWithScriptName(const CCharString& n) const
    { return n == "EitherTeamSpawn" ? CScriptThing(77, "EitherTeamSpawn") : CScriptThing(); }
};
struct CScriptDef { long GuardReinforcementsYourTeam, GuardReinforcementsEnemyTeam; };
static CScriptDef g_def = { 11, 22 };
static CScriptDef* g_pScriptDef = &g_def;
struct CQ_OrchardFarmRaidScript { long HeroTeam; };

class CTeamSpawn {
public:
    CGameScriptInterface*     Game;
    CScriptThing              Me;
    CQ_OrchardFarmRaidScript* ParentClass;
    CScriptThing              OtherSpawnPoint;
    long                      TeamID, TeamMemberLimit, TeamRespawnTime;
    CCharString               TeamMemberName, TeamMemberDefName;
    long                      BanditsLeftID;
    void Init();
};

void CTeamSpawn::Init()
{
    OtherSpawnPoint = Game->GetRandomThingWithScriptName("EitherTeamSpawn");
    if (Me.GetName() == "BanditTeamSpawn") {
        TeamID = 1;
        TeamMemberName = "BanditTeamMember";
        TeamMemberDefName = "CREATURE_BANDIT_GRUNT";
    } else {
        TeamID = 0;
        TeamMemberName = "GuardTeamMember";
        TeamMemberDefName = "CREATURE_ORCHARD_FARM_GUARD";
    }
    if (TeamID == ParentClass->HeroTeam) {
        TeamMemberLimit = 2;
        TeamRespawnTime = g_pScriptDef->GuardReinforcementsYourTeam;
    } else {
        TeamMemberLimit = 3;
        TeamRespawnTime = g_pScriptDef->GuardReinforcementsEnemyTeam;
    }
    BanditsLeftID = 0;
}

static bool Run(const char* name, long heroTeam, long id, const char* member, const char* def, long limit, long time)
{
    CGameScriptInterface game; CQ_OrchardFarmRaidScript parent = { heroTeam };
    CTeamSpawn t; t.Game = &game; t.ParentClass = &parent; t.Me.name = name; t.BanditsLeftID = 9;
    int before = g_live;
    t.Init();
    return g_live == before && t.OtherSpawnPoint.id == 77 && t.TeamID == id && t.TeamMemberName == member
        && t.TeamMemberDefName == def && t.TeamMemberLimit == limit && t.TeamRespawnTime == time && t.BanditsLeftID == 0;
}

int main()
{
    int ok = 1;
    ok &= Run("BanditTeamSpawn", 1, 1, "BanditTeamMember", "CREATURE_BANDIT_GRUNT", 2, 11);
    ok &= Run("BanditTeamSpawn", 0, 1, "BanditTeamMember", "CREATURE_BANDIT_GRUNT", 3, 22);
    ok &= Run("GuardTeamSpawn", 0, 0, "GuardTeamMember", "CREATURE_ORCHARD_FARM_GUARD", 2, 11);
    ok &= Run("", 1, 0, "GuardTeamMember", "CREATURE_ORCHARD_FARM_GUARD", 3, 22);
    ok &= g_live == 0;
    std::printf(ok ? "00dcd1d0_TEST PASS\n" : "00dcd1d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
