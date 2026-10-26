#include <cstdio>
#include <cstring>
#include <vector>
// behaviour test: CBriarRose::UpdateTarget keeps a live non-turncoated target, otherwise picks the nearest live
// non-turncoated minion guard (falling back to the nearest live one) and hands it to GiveThingBestEnemyTarget
static int Fail(const char* what) { std::printf("00da2a20_TEST FAIL %s\n", what); return 1; }
#define CHECK(cond) do { if (!(cond)) return Fail(#cond); } while (0)
static bool g_term = false;
static int g_gives = 0;
class CScriptThing { public:
    int Id; float X; bool Alive; bool Turncoat;
    CScriptThing(int id = 0, float x = 0.0f, bool alive = true, bool turn = false) : Id(id), X(x), Alive(alive), Turncoat(turn) { }
    bool IsAlive() const { return Alive; }
    bool IsNull() const { return Id == 0; }
};
static float GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b) { float d = a.X - b.X; return d < 0 ? -d : d; }
class CGame { public:
    std::vector<CScriptThing> Guards; CScriptThing Hero; int GivenTo;
    long GetAllThingsWithScriptName(const char* name, std::vector<CScriptThing>& out) { if (std::strcmp(name, "MC_MinionGuard") == 0) out = Guards; return (long)out.size(); }
    CScriptThing& GetHero() { return Hero; }
    bool IsThingTurncoated(const CScriptThing& t) const { return t.Turncoat; }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& target) { GivenTo = target.Id; ++g_gives; }
};
struct CBriarRose {
    CGame* Game; CScriptThing Me; CScriptThing Target;
    bool IsActiveThreadTerminating() const { return g_term; }
    bool UpdateTarget();
};
bool CBriarRose::UpdateTarget()
{
    std::vector<CScriptThing> things;
    Game->GetAllThingsWithScriptName("MC_MinionGuard", things);
    unsigned int count = (unsigned int)things.size();
    if (count == 0)
        return false;
    if (!Target.IsNull() && Target.IsAlive() && !Game->IsThingTurncoated(Target))
        return true;
    if (IsActiveThreadTerminating())
        return false;
    CScriptThing* nearest = 0;
    CScriptThing* nearestLoyal = 0;
    float nearestDistance = -1.0f;
    float nearestLoyalDistance = -1.0f;
    for (unsigned int i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return false;
        if (things[i].IsAlive()) {
            float distance = GetDistanceBetweenThings(Game->GetHero(), things[i]);
            if (distance < nearestDistance || nearestDistance == -1.0f) {
                nearestDistance = distance;
                nearest = &things[i];
            }
            if (!Game->IsThingTurncoated(things[i])) {
                if (distance < nearestLoyalDistance || nearestLoyalDistance == -1.0f) {
                    nearestLoyalDistance = distance;
                    nearestLoyal = &things[i];
                }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return false;
    CScriptThing* pick = nearestLoyal ? nearestLoyal : nearest;
    if (!pick)
        return false;
    Target = *pick;
    Game->GiveThingBestEnemyTarget(Me, Target);
    return true;
}
int main()
{
    CGame game; game.Hero = CScriptThing(99, 0.0f); game.GivenTo = 0;
    CBriarRose b; b.Game = &game; b.Me = CScriptThing(1, 50.0f);
    CHECK(!b.UpdateTarget() && g_gives == 0);                       // no guards at all
    game.Guards.push_back(CScriptThing(10, 5.0f, true, true));      // nearest but turncoated
    game.Guards.push_back(CScriptThing(11, 8.0f, true, false));     // nearest loyal
    game.Guards.push_back(CScriptThing(12, 2.0f, false, false));    // dead
    game.Guards.push_back(CScriptThing(13, -20.0f, true, false));
    CHECK(b.UpdateTarget() && b.Target.Id == 11 && game.GivenTo == 11 && g_gives == 1);
    CHECK(b.UpdateTarget() && g_gives == 1);                        // current target still valid: kept
    b.Target.Alive = false;
    game.Guards[1].Turncoat = true; game.Guards[3].Turncoat = true;  // no loyal guard left: nearest live one
    CHECK(b.UpdateTarget() && b.Target.Id == 10 && g_gives == 2);
    b.Target = CScriptThing();
    g_term = true;
    CHECK(!b.UpdateTarget() && g_gives == 2);
    std::printf("00da2a20_TEST PASS\n");
    return 0;
}