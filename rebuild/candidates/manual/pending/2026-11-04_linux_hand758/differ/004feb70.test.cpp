// Behaviour test for CWorldMap::HeightAreaChanged (0x004FEB70), g++ model: null area = whole world; notify engine, each overlapping map (local area), edges, then each navigation map.
#include <cstdio>
#include <vector>
struct Box { long x0, y0, x1, y1; };
struct MapInfo { Box world; bool hasMap; std::vector<Box> changed; };
struct Calls { std::vector<Box> engine; int edges; std::vector<long> nav; };
static bool Overlap(const Box& a, const Box& b) { return a.x0 < b.x1 && b.x0 < a.x1 && a.y0 < b.y1 && b.y0 < a.y1; }
static void HeightAreaChanged(const Box* area, const Box& dims, std::vector<MapInfo>& maps, Calls& c)
{
    if (!area) area = &dims;
    c.engine.push_back(*area);
    std::vector<long> hit;
    for (long i = 0; i < (long)maps.size(); ++i) if (Overlap(*area, maps[i].world)) hit.push_back(i);
    for (size_t k = 0; k < hit.size(); ++k) {
        MapInfo& m = maps[hit[k]];
        Box local = { area->x0 - m.world.x0, area->y0 - m.world.y0, area->x1 - m.world.x0, area->y1 - m.world.y0 };
        if (m.hasMap) m.changed.push_back(local);
    }
    ++c.edges;
    for (size_t k = 0; k < hit.size(); ++k) c.nav.push_back(hit[k]);
}
int main()
{
    Box dims = { 0, 0, 100, 100 };
    std::vector<MapInfo> maps(3);
    Box w0 = { 0, 0, 50, 50 }, w1 = { 50, 0, 100, 50 }, w2 = { 0, 50, 50, 100 };
    maps[0].world = w0; maps[0].hasMap = true; maps[1].world = w1; maps[1].hasMap = false; maps[2].world = w2; maps[2].hasMap = true;
    Calls c = { std::vector<Box>(), 0, std::vector<long>() };
    Box a = { 40, 10, 60, 20 };
    HeightAreaChanged(&a, dims, maps, c);
    int ok = c.engine.size() == 1 && c.engine[0].x0 == 40 && maps[0].changed.size() == 1 && maps[0].changed[0].x0 == 40
          && maps[1].changed.empty() && maps[2].changed.empty() && c.edges == 1 && c.nav.size() == 2 && c.nav[1] == 1;
    HeightAreaChanged(0, dims, maps, c);
    ok = ok && c.engine[1].x1 == 100 && c.nav.size() == 5 && maps[2].changed.size() == 1 && c.edges == 2;
    std::printf(ok ? "004feb70_TEST PASS\n" : "004feb70_TEST FAIL\n");
    return ok ? 0 : 1;
}
