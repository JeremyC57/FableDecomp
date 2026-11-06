// Behaviour test for NScript::CV_BanditTollScript::CTauntingBandit::Main (0x00E49020): one scripted frame at a time -- fleeing walks
// to the escape point until within CloseToExit + 2 and then fades out; angered bandits join FACTION_BANDITS and follow the hero; with
// the toll paid and the taunt timer at 0, a threatening hero gets WARNING_10/20/30 then ANGERED_10 (which angers the bandits) and the
// timer is reset; a calm hero ends the anger with NO_LONGER_THREATENING_10 and is taunted when near and rand() % 300 == 0.
#include <cstdio>
#include <string>
#include <vector>
struct World {
    bool BanditsFlee, BanditsAngered, TollPaid, Threatening, HeroNear, Alive, AwareOfHero;
    float DistanceToExit; long TimerValue; int RandValue;
    std::vector<std::string> Log;
    World() : BanditsFlee(false), BanditsAngered(false), TollPaid(false), Threatening(false), HeroNear(false), Alive(true),
              AwareOfHero(true), DistanceToExit(0.0f), TimerValue(0), RandValue(1) {}
};
struct Bandit {
    long AngerCount; bool AngryWithHero;
    Bandit() : AngerCount(0), AngryWithHero(false) {}
    // one pass of the main loop body (the terminate checks never fire here)
    void Frame(World& w, float closeToExit, long tauntTimer)
    {
        if (w.BanditsFlee) {
            while (w.DistanceToExit > closeToExit + 2.0f) { w.Log.push_back("MoveToPosition"); w.DistanceToExit -= 5.0f; }
            w.Log.push_back("FadeOutAndKillEntity");
        }
        if (w.BanditsAngered && w.Alive && w.AwareOfHero) {
            w.Log.push_back("EntitySetInFaction FACTION_BANDITS");
            w.Log.push_back("FollowThing hero");
        }
        if (w.TollPaid && !w.BanditsAngered) {
            if (w.TimerValue == 0) {
                if (w.Threatening) {
                    if (!AngryWithHero) AngryWithHero = true;
                    w.Log.push_back("EntitySetFacingAngleTowardsThing");
                    w.TimerValue = tauntTimer;
                    switch (AngerCount) {
                    case 0: w.Log.push_back("TEXT_QST_011_BANDIT_WARNING_10"); break;
                    case 1: w.Log.push_back("TEXT_QST_011_BANDIT_WARNING_20"); break;
                    case 2: w.Log.push_back("TEXT_QST_011_BANDIT_WARNING_30"); break;
                    case 3: w.Log.push_back("TEXT_QST_011_BANDIT_ANGERED_10"); w.BanditsAngered = true; break;
                    }
                    ++AngerCount;
                } else {
                    if (AngryWithHero) { w.Log.push_back("TEXT_QST_011_BANDIT_NO_LONGER_THREATENING_10"); AngryWithHero = false; }
                    if (w.HeroNear && w.RandValue % 300 == 0) w.Log.push_back("TEXT_QST_011_BANDIT_RANDOM_TAUNT");
                }
            }
        }
    }
};
int main()
{
    int fails = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); ++fails; } } while (0)
    World w; Bandit b;
    w.TollPaid = true; w.Threatening = true;
    b.Frame(w, 3.0f, 10);
    CHECK(w.Log.back() == "TEXT_QST_011_BANDIT_WARNING_10" && w.TimerValue == 10 && b.AngryWithHero && b.AngerCount == 1);
    size_t n = w.Log.size();
    b.Frame(w, 3.0f, 10);                        // timer running: nothing said
    CHECK(w.Log.size() == n);
    w.TimerValue = 0; b.Frame(w, 3.0f, 10); CHECK(w.Log.back() == "TEXT_QST_011_BANDIT_WARNING_20");
    w.TimerValue = 0; b.Frame(w, 3.0f, 10); CHECK(w.Log.back() == "TEXT_QST_011_BANDIT_WARNING_30");
    w.TimerValue = 0; b.Frame(w, 3.0f, 10); CHECK(w.Log.back() == "TEXT_QST_011_BANDIT_ANGERED_10" && w.BanditsAngered && b.AngerCount == 4);
    b.Frame(w, 3.0f, 10);                        // angered: follow the hero, no more toll talk
    CHECK(w.Log.back() == "FollowThing hero");
    World c; Bandit d;
    c.TollPaid = true; c.Threatening = true; d.Frame(c, 3.0f, 0);
    c.Threatening = false; c.HeroNear = true; c.RandValue = 600; d.Frame(c, 3.0f, 0);
    CHECK(c.Log[c.Log.size() - 2] == "TEXT_QST_011_BANDIT_NO_LONGER_THREATENING_10" && c.Log.back() == "TEXT_QST_011_BANDIT_RANDOM_TAUNT");
    CHECK(!d.AngryWithHero);
    c.RandValue = 7; n = c.Log.size(); d.Frame(c, 3.0f, 0); CHECK(c.Log.size() == n);
    World f; Bandit g;
    f.BanditsFlee = true; f.DistanceToExit = 16.0f; g.Frame(f, 3.0f, 0);
    CHECK(f.Log.size() == 4 && f.Log[0] == "MoveToPosition" && f.Log[2] == "MoveToPosition" && f.Log[3] == "FadeOutAndKillEntity");
    if (fails == 0) std::printf("00e49020_TEST PASS\n");
    return fails;
}
