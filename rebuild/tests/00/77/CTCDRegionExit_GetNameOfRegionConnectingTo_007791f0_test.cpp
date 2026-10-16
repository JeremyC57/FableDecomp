// behaviour test for 0x007791f0 CTCDRegionExit region-name lookup through the connected entrance
#include <cstdio>
struct Str { const char* s; };
static Str g_Empty = { "" };
struct Pos { float x; };
struct Thing { Pos pos; const Pos& GetPosition() const { return pos; } };
struct Region { Str name; Str display; const Str& GetDisplayName() const { return display.s[0] ? display : name; } };
struct WorldMap {
    Region regions[3];
    long GetMapNumberAtPosition(const Pos& p) const { return (long)p.x; }
    long GetRegionNumberMapIsIn(long map) const { return map / 10; }
    const Region& GetRegion(long r) const { return regions[r]; }
};
struct Exit {
    Thing* entrance; WorldMap* map;
    const WorldMap& PeekWorldMap() const { return *map; }
    const Str& Get() const
    {
        if (entrance)
        {
            const WorldMap& worldMap = PeekWorldMap();
            long mapNumber = worldMap.GetMapNumberAtPosition(entrance->GetPosition());
            const Region& r = PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(mapNumber));
            return 0 ? r.GetDisplayName() : r.name;
        }
        return g_Empty;
    }
};
int main()
{
    WorldMap m;
    m.regions[0].name.s = "A"; m.regions[0].display.s = "";
    m.regions[1].name.s = "B"; m.regions[1].display.s = "Bee";
    m.regions[2].name.s = "C"; m.regions[2].display.s = "";
    Thing t; t.pos.x = 15.0f;
    Exit e; e.entrance = 0; e.map = &m;
    bool ok = &e.Get() == &g_Empty;
    e.entrance = &t;
    const char* want = 0 ? "Bee" : "B";
    ok = ok && e.Get().s[0] == want[0] && e.Get().s[1] == want[1];
    t.pos.x = 23.0f;
    ok = ok && e.Get().s[0] == 'C';
    if (!ok) { std::printf("007791f0_TEST FAIL\n"); return 1; }
    std::printf("007791f0_TEST PASS\n");
    return 0;
}