// Behaviour model of NScript::CQ_WhiteBalverineWWScript::SpawnBalverines (0x00E19560): heal 250, quarter = 25% of health, threshold =
// health - quarter; three rounds: each time health drops below the threshold spawn `round` soldier balverines (at marker B when the hero
// is within 10 of the first marker), play the howl, run the first-spawn cutscene once (B variant if marker B was used), threshold -= quarter.
#include <cstdio>
#include <string>
#include <vector>
struct Sim {
    float Health; float HeroDist; int Frame; int Spawned; std::vector<std::string> Cutscenes; std::string LastMarker; int Howls;
    float GetHealth() { return Health; }
};
static void SpawnBalverines(Sim& g, const std::vector<float>& health_by_frame)
{
    bool use_marker_b = false, first_spawn_done = false;
    g.Health += 250.0f;
    long quarter = (long)(g.GetHealth() * 0.25f);
    long threshold = (long)(g.GetHealth() - quarter);
    for (long round = 1; round <= 3; ) {
        if (g.Frame >= (int)health_by_frame.size()) return;   // thread terminating
        g.Health = health_by_frame[g.Frame++];
        if (g.GetHealth() < threshold) {
            std::string marker = "MK_WBW_FIRSTSPAWN";
            if (g.HeroDist < 10.0f) { marker = "MK_WBW_FIRSTSPAWNB"; use_marker_b = true; }
            g.LastMarker = marker;
            for (long i = 0; i < round; ++i) ++g.Spawned;
            ++g.Howls;
            if (!first_spawn_done) {
                g.Cutscenes.push_back(use_marker_b ? "CS_WBW_FIRSTSPAWNB" : "CS_WBW_FIRSTSPAWN");
                first_spawn_done = true;
            }
            ++round;
            threshold -= quarter;
        }
    }
}
int main()
{
    Sim g = { 150.0f, 50.0f, 0, 0, std::vector<std::string>(), "", 0 };   // healed to 400: quarter 100, thresholds 300 / 200 / 100
    std::vector<float> h;
    h.push_back(400); h.push_back(350); h.push_back(299); h.push_back(250); h.push_back(199); h.push_back(150); h.push_back(99); h.push_back(50);
    SpawnBalverines(g, h);
    if (g.Spawned != 6 || g.Howls != 3 || g.Cutscenes.size() != 1 || g.Cutscenes[0] != "CS_WBW_FIRSTSPAWN" || g.Frame != 7) { std::printf("FAIL a\n"); return 1; }
    Sim g2 = { 150.0f, 5.0f, 0, 0, std::vector<std::string>(), "", 0 };
    std::vector<float> h2(3, 10.0f);
    SpawnBalverines(g2, h2);
    if (g2.Spawned != 6 || g2.Cutscenes[0] != "CS_WBW_FIRSTSPAWNB" || g2.LastMarker != "MK_WBW_FIRSTSPAWNB") { std::printf("FAIL b\n"); return 1; }
    Sim g3 = { 150.0f, 50.0f, 0, 0, std::vector<std::string>(), "", 0 };
    std::vector<float> h3(5, 350.0f);
    SpawnBalverines(g3, h3);
    if (g3.Spawned != 0 || !g3.Cutscenes.empty()) { std::printf("FAIL c\n"); return 1; }
    std::printf("00e19560_TEST PASS\n");
    return 0;
}
