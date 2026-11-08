// Behaviour model of CWorldMap::GetMapNavigationAreaInit (0x0050A650): a null box means the whole map's area; high-detail buildings /
// objects with a physics TC become detailed areas (bounding circle squared, clipped to the box); villages, AI creatures, creature
// generators, navigation seeds, region entrances (in the float box) and region exits each add a navigation seed at their position.
#include <cstdio>
#include <vector>
struct BoxI { long TLX, TLY, BRX, BRY; };
struct BoxF { float TLX, TLY, BRX, BRY; };
struct Seed { float X, Y; long Layer; };
enum Kind { BUILDING, OBJECT, VILLAGE, AICREATURE, GENERATOR, NAVSEED, ENTRANCE, EXIT, OTHER };
struct Thing { Kind K; float X, Y, Radius; bool HighDetail, HasPhysics; };
struct AreaInit { std::vector<BoxF> DetailedAreas; std::vector<Seed> NavRegionSeeds; BoxI WorldArea; };
static bool Overlaps(const Thing& t, const BoxI& b) { return t.X + t.Radius >= b.TLX && t.X - t.Radius <= b.BRX && t.Y + t.Radius >= b.TLY && t.Y - t.Radius <= b.BRY; }
static void GetMapNavigationAreaInit(const BoxI& map_area, const BoxI* pworld_box, const std::vector<Thing>& things, AreaInit& out)
{
    BoxI box;
    if (!pworld_box) { box = map_area; pworld_box = &box; }
    for (size_t i = 0; i < things.size(); ++i) {
        const Thing& t = things[i];
        if ((t.K == BUILDING || t.K == OBJECT) && Overlaps(t, *pworld_box) && t.HighDetail && t.HasPhysics) {
            BoxF f = { t.X - t.Radius, t.Y - t.Radius, t.X + t.Radius, t.Y + t.Radius };
            if (f.TLX < pworld_box->TLX) f.TLX = (float)pworld_box->TLX;
            if (f.TLY < pworld_box->TLY) f.TLY = (float)pworld_box->TLY;
            if (f.BRX > pworld_box->BRX) f.BRX = (float)pworld_box->BRX;
            if (f.BRY > pworld_box->BRY) f.BRY = (float)pworld_box->BRY;
            out.DetailedAreas.push_back(f);
        }
    }
    out.WorldArea = *pworld_box;
    const Kind order[6] = { VILLAGE, AICREATURE, GENERATOR, NAVSEED, ENTRANCE, EXIT };
    for (int k = 0; k < 6; ++k)
        for (size_t i = 0; i < things.size(); ++i)
            if (things[i].K == order[k] && Overlaps(things[i], *pworld_box)) {
                Seed s = { things[i].X, things[i].Y, 0 };
                out.NavRegionSeeds.push_back(s);
            }
}
int main()
{
    BoxI map = { 0, 0, 100, 100 };
    std::vector<Thing> t;
    Thing a = { BUILDING, 5, 50, 10, true, true }; t.push_back(a);
    Thing b = { OBJECT, 50, 50, 5, false, true }; t.push_back(b);
    Thing c = { EXIT, 10, 10, 0, false, false }; t.push_back(c);
    Thing d = { VILLAGE, 20, 20, 0, false, false }; t.push_back(d);
    Thing e = { AICREATURE, 300, 300, 0, false, false }; t.push_back(e);
    AreaInit out;
    GetMapNavigationAreaInit(map, 0, t, out);
    if (out.DetailedAreas.size() != 1 || out.DetailedAreas[0].TLX != 0.0f || out.DetailedAreas[0].BRX != 15.0f || out.DetailedAreas[0].TLY != 40.0f) { std::printf("FAIL areas\n"); return 1; }
    if (out.NavRegionSeeds.size() != 2 || out.NavRegionSeeds[0].X != 20.0f || out.NavRegionSeeds[1].X != 10.0f || out.WorldArea.BRX != 100) { std::printf("FAIL seeds\n"); return 1; }
    BoxI small = { 0, 0, 15, 15 };
    AreaInit out2;
    GetMapNavigationAreaInit(map, &small, t, out2);
    if (out2.DetailedAreas.size() != 0 || out2.NavRegionSeeds.size() != 1 || out2.WorldArea.BRX != 15) { std::printf("FAIL small\n"); return 1; }
    std::printf("0050a650_TEST PASS\n");
    return 0;
}