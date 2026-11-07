// Behaviour test for 0x005FE840 CTCInventoryMap::ConstructRegionExitMarkers: core / optional route regions come from the active,
// living quest cards (next region on the route from the current region; core cards go to the core set, others only when their
// route appears on the minimap); every drawn, active, not-hidden region exit and every drawn, not-hidden scripted hook whose
// entrance is alive gets a marker created from UI_EXIT_CORE_MARKER / UI_EXIT_OPTIONAL_MARKER / UI_EXIT_MARKER (route markers
// only for exits not script-named "NoQuestRoute"), angle 1 - facing (+0.5 when reversed), recorded with the plain marker name.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <set>
#include <string>
#include <vector>

struct Card { bool alive; int region; bool core; bool onMinimap; };
struct Exit { bool drawn, active, hidden, reversed, hasPhysics; int connects; float facing; std::string scriptName; };
struct Hook { bool drawn, hidden, reversed, entranceAlive, hasPhysics; int entranceRegion; float facing; };
struct Marker { std::string created, recorded; float angle; };

static int NextOnRoute(int from, int to) { return to == 99 ? 0 : (from * 10 + to); }   // 0 = no route

static void Construct(int current, const std::vector<Card>& cards, const std::vector<Exit>& exits, const std::vector<Hook>& hooks,
                      std::vector<Marker>& out)
{
    std::set<long> core, optional;
    for (size_t i = 0; i < cards.size(); ++i) {
        const Card& c = cards[i];
        if (!c.alive || c.region <= 0) continue;
        int next = NextOnRoute(current, c.region);
        if (next <= 0) continue;
        if (c.core) core.insert(next);
        else if (c.onMinimap) optional.insert(next);
    }
    for (size_t i = 0; i < exits.size(); ++i) {
        const Exit& e = exits[i];
        if (!e.drawn || !e.active || e.hidden) continue;
        Marker m; m.created = "UI_EXIT_MARKER"; m.angle = 0.0f;
        if (e.scriptName != "NoQuestRoute") {
            if (core.find(e.connects) != core.end()) m.created = "UI_EXIT_CORE_MARKER";
            else if (optional.find(e.connects) != optional.end()) m.created = "UI_EXIT_OPTIONAL_MARKER";
        }
        m.recorded = "UI_EXIT_MARKER";
        if (e.hasPhysics) { m.angle = 1.0f - e.facing; if (e.reversed) m.angle += 0.5f; }
        out.push_back(m);
    }
    for (size_t i = 0; i < hooks.size(); ++i) {
        const Hook& h = hooks[i];
        if (!h.drawn || h.hidden || !h.entranceAlive) continue;
        Marker m; m.created = "UI_EXIT_MARKER"; m.angle = 0.0f;
        if (core.find(h.entranceRegion) != core.end()) m.created = "UI_EXIT_CORE_MARKER";
        else if (optional.find(h.entranceRegion) != optional.end()) m.created = "UI_EXIT_OPTIONAL_MARKER";
        m.recorded = "UI_EXIT_MARKER";
        if (h.hasPhysics) { m.angle = 1.0f - h.facing; if (h.reversed) m.angle += 0.5f; }
        out.push_back(m);
    }
}

static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    std::vector<Card> cards;
    Card c1 = { true, 2, true, false };  cards.push_back(c1);    // core -> 12
    Card c2 = { true, 3, false, true };  cards.push_back(c2);    // optional -> 13
    Card c3 = { true, 4, false, false }; cards.push_back(c3);    // not on the minimap
    Card c4 = { false, 5, true, false }; cards.push_back(c4);    // dead
    Card c5 = { true, 99, true, false }; cards.push_back(c5);    // no route
    std::vector<Exit> exits;
    Exit e1 = { true, true, false, false, true, 12, 0.25f, "" };             exits.push_back(e1);
    Exit e2 = { true, true, false, true, true, 13, 0.25f, "" };              exits.push_back(e2);
    Exit e3 = { true, true, false, false, false, 12, 0.0f, "NoQuestRoute" }; exits.push_back(e3);
    Exit e4 = { true, false, false, false, true, 12, 0.0f, "" };             exits.push_back(e4);   // inactive
    Exit e5 = { false, true, false, false, true, 12, 0.0f, "" };             exits.push_back(e5);   // not drawn
    Exit e6 = { true, true, false, false, true, 14, 0.0f, "" };              exits.push_back(e6);   // card 3's region: plain
    std::vector<Hook> hooks;
    Hook h1 = { true, false, false, true, true, 13, 0.5f };  hooks.push_back(h1);
    Hook h2 = { true, false, false, false, true, 12, 0.5f }; hooks.push_back(h2);   // entrance dead
    std::vector<Marker> out;
    Construct(1, cards, exits, hooks, out);
    bool ok = out.size() == 5
        && out[0].created == "UI_EXIT_CORE_MARKER" && near(out[0].angle, 0.75f)
        && out[1].created == "UI_EXIT_OPTIONAL_MARKER" && near(out[1].angle, 1.25f)
        && out[2].created == "UI_EXIT_MARKER" && near(out[2].angle, 0.0f)
        && out[3].created == "UI_EXIT_MARKER"
        && out[4].created == "UI_EXIT_OPTIONAL_MARKER" && near(out[4].angle, 0.5f);
    for (size_t i = 0; i < out.size(); ++i) ok = ok && out[i].recorded == "UI_EXIT_MARKER";
    std::printf(ok ? "005fe840_TEST PASS\n" : "005fe840_TEST FAIL\n");
    return ok ? 0 : 1;
}
