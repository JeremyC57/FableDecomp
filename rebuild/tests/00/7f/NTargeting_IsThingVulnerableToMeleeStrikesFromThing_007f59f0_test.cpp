#include <cstdio>
#include <cmath>
// Behaviour test: NTargeting::IsThingVulnerableToMeleeStrikesFromThing -- a player in kill-everything mode hits any thing flagged 8; with g_bMeleeHitsBestAttackTarget the attacker's best attack target is hit; enemies are always hit.
#define CHECK(c) if (!(c)) { std::printf("007f59f0_TEST FAIL\n"); return 1; }
struct CPlayer { bool KillEverythingMode; };
struct CThing;
struct CTCTargetingAI { CThing* Best; CThing* GetPBestEnemyCreatureForAttack() const { return Best; } };
struct CThing {
    unsigned long ThingTypeFlags; CPlayer* Player; const CTCTargetingAI* AI; int Team;
    CPlayer* GetPlayer() const { return Player; }
    bool PeekTC(const CTCTargetingAI** out) const { if (AI) { *out = AI; return true; } return false; }
};
static bool g_bMeleeHitsBestAttackTarget = false;
static bool AreThingsEnemies(const CThing& a, const CThing& b) { return a.Team != b.Team; }
static bool IsVulnerable(const CThing& thing, const CThing& fromThing)
{
    if ((thing.ThingTypeFlags & 8) && fromThing.GetPlayer()->KillEverythingMode) return true;
    if (g_bMeleeHitsBestAttackTarget) {
        const CTCTargetingAI* ai;
        if (fromThing.PeekTC(&ai) && ai->GetPBestEnemyCreatureForAttack() == &thing) return true;
    }
    if (AreThingsEnemies(fromThing, thing)) return true;
    return false;
}
int main()
{
    CPlayer pl; pl.KillEverythingMode = false;
    CTCTargetingAI ai; ai.Best = 0;
    CThing hero; hero.ThingTypeFlags = 0; hero.Player = &pl; hero.AI = &ai; hero.Team = 1;
    CThing villager; villager.ThingTypeFlags = 8; villager.Player = &pl; villager.AI = 0; villager.Team = 1;
    CThing bandit; bandit.ThingTypeFlags = 8; bandit.Player = &pl; bandit.AI = 0; bandit.Team = 2;
    CHECK(!IsVulnerable(villager, hero) && IsVulnerable(bandit, hero))
    pl.KillEverythingMode = true; CHECK(IsVulnerable(villager, hero))
    pl.KillEverythingMode = false; ai.Best = &villager; CHECK(!IsVulnerable(villager, hero))
    g_bMeleeHitsBestAttackTarget = true; CHECK(IsVulnerable(villager, hero))
    hero.AI = 0; CHECK(!IsVulnerable(villager, hero))
    std::printf("007f59f0_TEST PASS\n");
    return 0;
}