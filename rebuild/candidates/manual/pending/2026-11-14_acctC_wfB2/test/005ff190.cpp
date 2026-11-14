// Behaviour test for 0x005FF190 CTCInventoryMap::CreateMapMarkers: the hero marker (only for a living hero; at the world-map offset
// of the region the parent thing's map is in; angle 1 - facing when the hero has physics), one teleporter-region quest marker per
// (region, active card whose teleporter region is that region) by kind core / optional / vignette, the per-region quest count,
// one quest marker per active card with a region (kind order core, vignette, optional, else core) at the region's offset and
// spread when the region holds several (++index: 1 +X, 2 -Y, 3 +Y by 15; a single quest is not moved), the region-name sprites
// (regions on the world map; offset minus the name-graphic offset) each followed by a teleport marker when the region has a
// teleporter, and the path sprites (key min * 1000 + max in the def's MapPaths; at the midpoint of the two offsets).
// Model: the row's statements in source order over plain structs; every marker goes to the parent in the order retail adds it.
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
#include <list>
#include <string>
struct V2 { float X, Y; V2() : X(0), Y(0) { } V2(float x, float y) : X(x), Y(y) { } };
struct Child { std::string Def; V2 Position; long Graphic; float Angle; };
struct Region { std::string Name; long Number; V2 Offset, NameOffset; bool OnWorldMap; long NameGraphic; bool Teleporter; };
struct Card { std::string TeleporterRegion, RegionName; bool Core, Optional, Vignette; };
struct Hero { bool Dead; bool HasPhysics; float Facing; long Map; };
struct World {
    std::vector<Region> All; std::map<long, long> MapRegion; std::map<unsigned long, unsigned long> MapPaths;
    long NumberFromName(const std::string& n) const { for (size_t i = 0; i < All.size(); ++i) if (All[i].Name == n) return All[i].Number; return 0; }
    const Region& Get(long number) const { for (size_t i = 0; i < All.size(); ++i) if (All[i].Number == number) return All[i]; return All[0]; }
};
static long Min(long a, long b) { return b < a ? b : a; }
static long Max(long a, long b) { return a < b ? b : a; }
static std::vector<Child> g_parent;
static void Add(const char* def, const V2& p, long graphic = 0, float angle = 0.0f) { Child c; c.Def = def; c.Position = p; c.Graphic = graphic; c.Angle = angle; g_parent.push_back(c); }
static void CreateMapMarkers(const World& w, const Hero* hero, std::vector<Region>& regions, const std::list<Card>& active,
                             const std::list<std::pair<const Region*, const Region*> >& correspondances)
{
    if (hero && !hero->Dead) {
        V2 heroPosition = w.Get(w.MapRegion.find(hero->Map)->second).Offset;
        float angle = 0.0f;
        if (hero->HasPhysics)
            angle = 1.0f - hero->Facing;
        Add("UI_HERO_MARKER_MAIN", heroPosition, 0, angle);
    }
    for (unsigned long i = 0; i < regions.size(); ++i) {
        std::list<Card> cards = active;
        for (std::list<Card>::iterator it = cards.begin(); it != cards.end(); ++it) {
            if (it->TeleporterRegion == regions[i].Name) {
                if (it->Core)
                    Add("UI_CORE_MARKER_MAIN", regions[i].Offset);
                else if (it->Optional)
                    Add("UI_OPTIONAL_MARKER_MAIN", regions[i].Offset);
                else if (it->Vignette)
                    Add("UI_VIGNETTE_MARKER_MAIN", regions[i].Offset);
            }
        }
    }
    std::map<long, std::pair<long, long> > regionQuests;
    std::list<Card> cards = active;
    for (std::list<Card>::iterator it = cards.begin(); it != cards.end(); ++it) {
        long region = w.NumberFromName(it->RegionName);
        if (region) {
            std::map<long, std::pair<long, long> >::iterator found = regionQuests.find(region);
            if (found == regionQuests.end())
                regionQuests[region] = std::pair<long, long>(1, 0);
            else
                ++found->second.first;
        }
    }
    for (std::list<Card>::iterator it2 = cards.begin(); it2 != cards.end(); ++it2) {
        if (w.NumberFromName(it2->RegionName)) {
            const char* def;
            if (it2->Core) def = "UI_CORE_MARKER_MAIN";
            else if (it2->Vignette) def = "UI_VIGNETTE_MARKER_MAIN";
            else if (it2->Optional) def = "UI_OPTIONAL_MARKER_MAIN";
            else def = "UI_CORE_MARKER_MAIN";
            std::map<long, std::pair<long, long> >::iterator found = regionQuests.find(w.NumberFromName(it2->RegionName));
            V2 position = w.Get(w.NumberFromName(it2->RegionName)).Offset;
            if (found->second.first == 1 && found->second.second == 0) {
                Add(def, position);
            } else {
                switch (++found->second.second) {
                case 0: position.X -= 15.0f; break;
                case 1: position.X += 15.0f; break;
                case 2: position.Y -= 15.0f; break;
                case 3: position.Y += 15.0f; break;
                }
                Add(def, position);
            }
        }
    }
    for (unsigned long j = 0; j < regions.size(); ++j) {
        if (regions[j].OnWorldMap) {
            Add("UI_REGION_NAME_MARKER", V2(regions[j].Offset.X - regions[j].NameOffset.X, regions[j].Offset.Y - regions[j].NameOffset.Y), regions[j].NameGraphic);
            if (regions[j].Teleporter && regions[j].OnWorldMap) {
                V2 position = regions[j].Offset;
                Add("UI_TELEPORT_MARKER_MAIN", position);
            }
        }
    }
    for (std::list<std::pair<const Region*, const Region*> >::const_iterator path = correspondances.begin(); path != correspondances.end(); ++path) {
        long from = w.NumberFromName(path->first->Name);
        long to = w.NumberFromName(path->second->Name);
        std::map<unsigned long, unsigned long>::const_iterator graphic = w.MapPaths.find(Min(from, to) * 1000 + Max(from, to));
        if (graphic != w.MapPaths.end())
            Add("UI_REGION_PATH_MARKER", V2(path->first->Offset.X + (path->second->Offset.X - path->first->Offset.X) / 2.0f,
                                            path->first->Offset.Y + (path->second->Offset.Y - path->first->Offset.Y) / 2.0f), (long)graphic->second);
    }
}
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
static bool Is(size_t i, const char* def, float x, float y) { return i < g_parent.size() && g_parent[i].Def == def && Near(g_parent[i].Position.X, x) && Near(g_parent[i].Position.Y, y); }
static Region MakeRegion(const char* name, long number, float x, float y, bool on, bool teleporter)
{ Region r; r.Name = name; r.Number = number; r.Offset = V2(x, y); r.NameOffset = V2(1.0f, 2.0f); r.OnWorldMap = on; r.NameGraphic = number * 10; r.Teleporter = teleporter; return r; }
static Card MakeCard(const char* teleporter, const char* region, bool core, bool optional, bool vignette)
{ Card c; c.TeleporterRegion = teleporter; c.RegionName = region; c.Core = core; c.Optional = optional; c.Vignette = vignette; return c; }
int main()
{
    World w;
    w.All.push_back(MakeRegion("GUILD", 1, 100.0f, 100.0f, true, true));
    w.All.push_back(MakeRegion("OAKVALE", 2, 200.0f, 50.0f, true, false));
    w.All.push_back(MakeRegion("HIDDEN", 3, 300.0f, 300.0f, false, true));
    w.MapRegion[7] = 2;
    w.MapPaths[1002] = 55;
    std::vector<Region> regions = w.All;
    std::list<Card> active;
    active.push_back(MakeCard("GUILD", "OAKVALE", true, false, false));       // core, region 2
    active.push_back(MakeCard("OAKVALE", "OAKVALE", false, true, true));      // optional + vignette, region 2
    active.push_back(MakeCard("NOWHERE", "OAKVALE", false, false, false));    // no kind, region 2
    active.push_back(MakeCard("GUILD", "GUILD", false, false, true));         // vignette, region 1 (single)
    active.push_back(MakeCard("HIDDEN", "UNKNOWN", false, true, false));      // optional, no region
    std::list<std::pair<const Region*, const Region*> > paths;
    paths.push_back(std::make_pair((const Region*)&regions[1], (const Region*)&regions[0]));    // 2 -> 1: key 1002
    paths.push_back(std::make_pair((const Region*)&regions[0], (const Region*)&regions[2]));    // 1 -> 3: key 1003, no graphic
    Hero hero; hero.Dead = false; hero.HasPhysics = true; hero.Facing = 0.25f; hero.Map = 7;
    CreateMapMarkers(w, &hero, regions, active, paths);
    size_t n = 0;
    bool ok = Is(n, "UI_HERO_MARKER_MAIN", 200.0f, 50.0f) && Near(g_parent[n].Angle, 0.75f); ++n;
    // teleporter-region markers: region GUILD has cards 0 (core) and 3 (vignette); OAKVALE card 1 (optional before vignette); HIDDEN card 4
    ok = ok && Is(n++, "UI_CORE_MARKER_MAIN", 100.0f, 100.0f) && Is(n++, "UI_VIGNETTE_MARKER_MAIN", 100.0f, 100.0f)
            && Is(n++, "UI_OPTIONAL_MARKER_MAIN", 200.0f, 50.0f) && Is(n++, "UI_OPTIONAL_MARKER_MAIN", 300.0f, 300.0f);
    // quest markers: OAKVALE holds three (+X, -Y, +Y), GUILD one (not moved); the card without a region gives none
    ok = ok && Is(n++, "UI_CORE_MARKER_MAIN", 215.0f, 50.0f) && Is(n++, "UI_VIGNETTE_MARKER_MAIN", 200.0f, 35.0f)
            && Is(n++, "UI_CORE_MARKER_MAIN", 200.0f, 65.0f) && Is(n++, "UI_VIGNETTE_MARKER_MAIN", 100.0f, 100.0f);
    // region names (HIDDEN is not on the world map) with the teleport marker of GUILD
    ok = ok && Is(n, "UI_REGION_NAME_MARKER", 99.0f, 98.0f) && g_parent[n].Graphic == 10; ++n;
    ok = ok && Is(n++, "UI_TELEPORT_MARKER_MAIN", 100.0f, 100.0f);
    ok = ok && Is(n, "UI_REGION_NAME_MARKER", 199.0f, 48.0f) && g_parent[n].Graphic == 20; ++n;
    // one path sprite (key 1002) at the midpoint
    ok = ok && Is(n, "UI_REGION_PATH_MARKER", 150.0f, 75.0f) && g_parent[n].Graphic == 55; ++n;
    ok = ok && n == g_parent.size();
    // a dead hero gets no marker
    g_parent.clear(); hero.Dead = true;
    std::list<Card> none; std::list<std::pair<const Region*, const Region*> > nopaths;
    CreateMapMarkers(w, &hero, regions, none, nopaths);
    ok = ok && g_parent.size() == 3 && g_parent[0].Def == "UI_REGION_NAME_MARKER";
    std::printf(ok ? "005ff190_TEST PASS\n" : "005ff190_TEST FAIL\n");
    return ok ? 0 : 1;
}
