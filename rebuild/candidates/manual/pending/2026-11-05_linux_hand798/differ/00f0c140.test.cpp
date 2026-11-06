// Behaviour test for CQ_AmbushTradersScript::PhaseTwo (0x00F0C140), region loop: Bowerstone Jail activates phase 3 only once;
// Gibbet Woods recreates every recorded convoy member (health set by the difference to the current health) and empties the list.
#include <cstdio>
#include <cstring>

struct Member { const char* def; float health; };
struct Phase {
    bool ActivatedPhase3; int activations; Member info[4]; int count; int created; float healths[4];
    void Region(const char* r)
    {
        if (std::strcmp(r, "BowerstoneJail") == 0) { if (!ActivatedPhase3) { ++activations; ActivatedPhase3 = true; } }
        else if (std::strcmp(r, "DesertedFarm") == 0) { }
        else if (std::strcmp(r, "GibbetWoods") == 0) {
            for (int t = 0; t < count; ++t) { float current = 100.0f; healths[created++] = current + (info[t].health - current); }
            count = 0;
        }
    }
};

int main()
{
    Phase p; std::memset(&p, 0, sizeof p);
    p.info[0].def = "CREATURE_TRADER"; p.info[0].health = 40.0f; p.info[1].def = "CREATURE_GUARD"; p.info[1].health = 75.0f; p.count = 2;
    p.Region("BowerstoneJail"); p.Region("BowerstoneJail"); p.Region("DesertedFarm"); p.Region("GibbetWoods"); p.Region("GibbetWoods");
    bool ok = p.activations == 1 && p.created == 2 && p.count == 0 && p.healths[0] == 40.0f && p.healths[1] == 75.0f;
    if (!ok) { std::printf("00f0c140_TEST FAIL\n"); return 1; }
    std::printf("00f0c140_TEST PASS\n");
    return 0;
}
