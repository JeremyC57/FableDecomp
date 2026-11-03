// Behaviour model of CWorldMap::GetMapNavigationAreaUpdateInfo (retail 0x00509A20): gathers navigation lines (plain and
// switchable), ACTION_POINT dummies, TRANSITION_POINT helpers and the topology weights of one map area.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
struct V2 { float X, Y; };
struct Line { V2 P1, P2; };
struct Box { long TLX, TLY, BRX, BRY;
    bool Contains(long x, long y) const { return x >= TLX && x < BRX && y >= TLY && y < BRY; } };
struct Blockage { unsigned long long UID; std::list<Line> Lines; };
struct Layer { std::vector<Line> Lines; std::vector<Blockage> Blockages; };
struct TP { V2 Pos; long L1, L2; };
struct NP { V2 Pos; long L; };
struct Update { std::vector<Layer> Layers; std::vector<TP> Transitions; std::vector<NP> Actions; std::vector<unsigned char> Weights; };
struct Named { std::string Name; V2 Pos; };
struct Thing {
    unsigned long long UID; long BaseLayer; bool Switchable; bool HasMeshInfo;
    std::vector<std::vector<Line> > LineLists; std::vector<Named> Dummies; std::vector<Named> Helpers;
};
static long LayerAt(const V2& p) { return p.X > 50.0f ? 1 : 0; }
static bool Passable(long x, long y) { return (x + y) % 3 != 0; }
static bool PrefNav(long x, long y) { return x % 2 == 0; }
static long ParseAfter(const std::string& s, size_t& at) {
    at = s.find('_', at); ++at; long v = std::atol(s.c_str() + at); while (at < s.size() && s[at] != '_') ++at; return v; }
static void Gather(const std::vector<Thing>& things, const Box& area, Update& u) {
    u.Layers.clear();
    for (size_t t = 0; t < things.size(); ++t) {
        const Thing& th = things[t];
        long layer = th.BaseLayer, count = (long)th.LineLists.size();
        if ((unsigned long)(layer + count) > u.Layers.size()) u.Layers.resize(layer + count);
        for (long i = 0; i < count; ++i) {
            if (th.Switchable) {
                Blockage b; b.UID = th.UID & 0xFFFFFFFFFFull;
                for (size_t k = 0; k < th.LineLists[i].size(); ++k) b.Lines.push_back(th.LineLists[i][k]);
                u.Layers[layer + i].Blockages.push_back(b);
            } else
                for (size_t k = 0; k < th.LineLists[i].size(); ++k) u.Layers[layer + i].Lines.push_back(th.LineLists[i][k]);
        }
        long dummyIndex = 0;
        for (size_t d = 0; d < th.Dummies.size(); ++d)
            if (th.Dummies[d].Name.size() >= 12 && th.Dummies[d].Name.compare(0, 12, "ACTION_POINT") == 0) {
                const V2& p = th.Dummies[d].Pos;
                if (area.Contains((long)p.X, (long)p.Y)) { NP n = { p, LayerAt(p) }; u.Actions.push_back(n); }
                ++dummyIndex;
            }
        if (th.HasMeshInfo && !th.Switchable)
            for (size_t h = 0; h < th.Helpers.size(); ++h) {
                const std::string& n = th.Helpers[h].Name; const V2& p = th.Helpers[h].Pos;
                if (n.compare(0, 16, "TRANSITION_POINT") != 0 || !area.Contains((long)p.X, (long)p.Y)) continue;
                TP tp = { p, 0, 1 };
                if (n != "TRANSITION_POINT") { size_t at = 16; tp.L1 = ParseAfter(n, at) - 1; tp.L2 = ParseAfter(n, at) - 1; }
                u.Transitions.push_back(tp);
            }
    }
    long w = area.BRX - area.TLX;
    u.Weights.resize(w * (area.BRY - area.TLY));
    for (long y = area.TLY, j = 0; y < area.BRY; ++y, ++j)
        for (long x = area.TLX, i = 0; x < area.BRX; ++x, ++i)
            u.Weights[w * j + i] = Passable(x, y) ? (PrefNav(x, y) ? 0 : 0x80) : 0xFF;
}
int main() {
    Box area = { 0, 0, 100, 4 };
    Line a = { { 1, 1 }, { 2, 2 } }, b = { { 3, 3 }, { 4, 4 } };
    std::vector<Thing> things(2);
    things[0].UID = 0xAB0000000123ull; things[0].BaseLayer = 1; things[0].Switchable = true; things[0].HasMeshInfo = true;
    things[0].LineLists.push_back(std::vector<Line>(1, a)); things[0].LineLists.push_back(std::vector<Line>(2, b));
    Named act = { "ACTION_POINT", { 60, 2 } }, act2 = { "ACTION_POINT_02", { 500, 2 } };
    things[0].Dummies.push_back(act); things[0].Dummies.push_back(act2);
    things[1].UID = 7; things[1].BaseLayer = 0; things[1].Switchable = false; things[1].HasMeshInfo = true;
    things[1].LineLists.push_back(std::vector<Line>(1, a));
    Named t1 = { "TRANSITION_POINT", { 10, 1 } }, t2 = { "TRANSITION_POINT_2_3", { 20, 1 } }, t3 = { "OTHER", { 5, 1 } };
    things[1].Helpers.push_back(t1); things[1].Helpers.push_back(t2); things[1].Helpers.push_back(t3);
    Update u; u.Layers.resize(5);
    Gather(things, area, u);
    bool ok = u.Layers.size() == 3 && u.Layers[1].Blockages.size() == 1 && u.Layers[2].Blockages.size() == 1
        && u.Layers[1].Blockages[0].UID == 0x0000000123ull && u.Layers[2].Blockages[0].Lines.size() == 2
        && u.Layers[0].Lines.size() == 1 && u.Actions.size() == 1 && u.Actions[0].L == 1
        && u.Transitions.size() == 2 && u.Transitions[0].L1 == 0 && u.Transitions[0].L2 == 1
        && u.Transitions[1].L1 == 1 && u.Transitions[1].L2 == 2 && u.Weights.size() == 400
        && u.Weights[0] == 0xFF && u.Weights[1] == 0x80 && u.Weights[100 * 1 + 0] == 0 && u.Weights[100 * 3 + 2] == 0;
    if (!ok) { std::printf("00509a20_TEST FAIL\n"); return 1; }
    std::printf("00509a20_TEST PASS\n");
    return 0;
}
