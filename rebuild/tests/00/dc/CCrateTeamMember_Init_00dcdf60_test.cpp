#include <cstdio>
#include <cstring>
// Behaviour test for CCrateTeamMember::Init @ 0x00DCDF60: the team comes from the thing's script name, the faction /
// marker / ally / turncoat / emote calls depend on whether that team is the hero's, the data string names the team,
// and the member joins its team manager in state 0 with both AI states 2 and an empty patrol target.
struct Thing { const char* name; char data[32]; char faction[32]; char marker[32]; int ally; int turncoatOff; int emoteOff; };
struct Script {
    int heroCalls;
    void EntitySetInFaction(Thing& t, const char* f) { std::strcpy(t.faction, f); }
    void MiniMapAddMarker(Thing& t, const char* m) { std::strcpy(t.marker, m); }
    void SetIsThingTurncoatable(Thing& t, bool on) { if (!on) ++t.turncoatOff; }
    void EntitySetAsDisplayingEmoteIcon(Thing& t, bool on) { if (!on) ++t.emoteOff; }
    int GetHero() { ++heroCalls; return 7; }
    void EntitySetThingAsAllyOfThing(Thing& t, int ally) { t.ally = ally; }
};
struct Team { long StateCounter[6]; long MemberCount; };
struct Raid { long HeroTeam; Team Teams[2]; };
struct Member {
    Script* m_pScript;
    Thing m_thing;
    Raid* ParentClass;
    long CurrentAIState, PreviousAIState, TeamID, MemberState;
    Team* MyTeam;
    int patrolTarget;
    void Init()
    {
        if (m_thing.name && std::strcmp(m_thing.name, "BanditTeamMember") == 0)
            TeamID = 1;
        else
            TeamID = 0;
        if (TeamID == 1) {
            if (TeamID == ParentClass->HeroTeam) {
                m_pScript->EntitySetInFaction(m_thing, "FACTION_BANDITS_FRIENDLY");
                m_pScript->EntitySetThingAsAllyOfThing(m_thing, m_pScript->GetHero());
            } else {
                m_pScript->EntitySetInFaction(m_thing, "FACTION_BANDITS");
                m_pScript->MiniMapAddMarker(m_thing, "HUD_ORB_RED_SMALL");
                m_pScript->SetIsThingTurncoatable(m_thing, false);
                m_pScript->EntitySetAsDisplayingEmoteIcon(m_thing, false);
            }
            std::strcpy(m_thing.data, "BANDIT");
        } else {
            if (TeamID == ParentClass->HeroTeam) {
                m_pScript->EntitySetInFaction(m_thing, "FACTION_VILLAGERS");
                m_pScript->EntitySetThingAsAllyOfThing(m_thing, m_pScript->GetHero());
            } else {
                m_pScript->EntitySetInFaction(m_thing, "FACTION_GUARDS_ENEMY");
                m_pScript->MiniMapAddMarker(m_thing, "HUD_ORB_RED_SMALL");
                m_pScript->EntitySetAsDisplayingEmoteIcon(m_thing, false);
            }
            std::strcpy(m_thing.data, "GUARD");
        }
        MyTeam = &ParentClass->Teams[TeamID];
        ++MyTeam->MemberCount;
        CurrentAIState = 2;
        PreviousAIState = 2;
        MemberState = 0;
        ++MyTeam->StateCounter[MemberState];
        patrolTarget = 0;
    }
};

static int Fail(int n) { std::printf("00dcdf60_TEST FAIL %d\n", n); return 1; }

static void Reset(Member& m, Script& s, Raid& r, const char* name, long heroTeam)
{
    std::memset(&s, 0, sizeof(s));
    std::memset(&r, 0, sizeof(r));
    r.HeroTeam = heroTeam;
    std::memset(&m.m_thing, 0, sizeof(m.m_thing));
    m.m_thing.name = name;
    m.m_pScript = &s; m.ParentClass = &r;
    m.CurrentAIState = m.PreviousAIState = 9; m.TeamID = 5; m.MemberState = 3; m.MyTeam = 0; m.patrolTarget = 42;
}

int main()
{
    Script s; Raid r; Member m;
    // bandit on the hero's team: friendly faction, ally of the hero
    Reset(m, s, r, "BanditTeamMember", 1);
    m.Init();
    if (m.TeamID != 1 || m.MyTeam != &r.Teams[1]) return Fail(1);
    if (std::strcmp(m.m_thing.faction, "FACTION_BANDITS_FRIENDLY") || m.m_thing.ally != 7 || s.heroCalls != 1) return Fail(2);
    if (m.m_thing.marker[0] || m.m_thing.turncoatOff || m.m_thing.emoteOff || std::strcmp(m.m_thing.data, "BANDIT")) return Fail(3);
    if (r.Teams[1].MemberCount != 1 || r.Teams[1].StateCounter[0] != 1 || r.Teams[0].MemberCount != 0) return Fail(4);
    if (m.CurrentAIState != 2 || m.PreviousAIState != 2 || m.MemberState != 0 || m.patrolTarget != 0) return Fail(5);
    // bandit against the hero: hostile, red orb, not turncoatable, no emote icon
    Reset(m, s, r, "BanditTeamMember", 0);
    m.Init();
    if (m.TeamID != 1 || std::strcmp(m.m_thing.faction, "FACTION_BANDITS") || std::strcmp(m.m_thing.marker, "HUD_ORB_RED_SMALL")) return Fail(6);
    if (m.m_thing.turncoatOff != 1 || m.m_thing.emoteOff != 1 || m.m_thing.ally != 0 || s.heroCalls != 0) return Fail(7);
    // guard on the hero's team: villager faction, ally
    Reset(m, s, r, "GuardTeamMember", 0);
    m.Init();
    if (m.TeamID != 0 || m.MyTeam != &r.Teams[0] || std::strcmp(m.m_thing.faction, "FACTION_VILLAGERS") || m.m_thing.ally != 7) return Fail(8);
    if (std::strcmp(m.m_thing.data, "GUARD") || r.Teams[0].MemberCount != 1 || r.Teams[0].StateCounter[0] != 1) return Fail(9);
    // guard against the hero (and an unnamed thing counts as a guard): hostile guards, orb, no emote, turncoat untouched
    Reset(m, s, r, 0, 1);
    m.Init();
    if (m.TeamID != 0 || std::strcmp(m.m_thing.faction, "FACTION_GUARDS_ENEMY") || std::strcmp(m.m_thing.marker, "HUD_ORB_RED_SMALL")) return Fail(10);
    if (m.m_thing.emoteOff != 1 || m.m_thing.turncoatOff != 0 || s.heroCalls != 0) return Fail(11);
    std::printf("00dcdf60_TEST PASS\n");
    return 0;
}