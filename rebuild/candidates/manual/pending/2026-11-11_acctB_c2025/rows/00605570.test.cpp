// Behaviour test for 0x00605570 CTCInventoryMap::ConstructRegionDescription: find the region's cached CUIRegionInfo (or append a zeroed
// one tagged with the region number), mark the counts for recomputation when the region is the hero's current one, then build the
// panel in retail's order: a text container under the parent holding the description line (the region def's RegionDescription text id
// with an empty extra string when the def has one, else the display name) and the hero's crime report; the region's quests; a second
// text container whose shop / buyable-building lines are added only for a visited region (with the info record and the recompute
// flag); the "TEXT_GUI_MENU_MAP_KEYS" line followed by a newline (unsigned short text); the map keys. PC or UI templates by `pc`.
#include <cstdio>
#include <cstring>

struct RegionInfo { long Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns; };
struct Region { long number; bool has_def; unsigned long description; const char* display_name; };

enum { EV_CONTAINER, EV_TEXT_ID, EV_TEXT_NAME, EV_TEXT_CRIME, EV_QUESTS, EV_SHOPS, EV_BUYABLE, EV_TEXT_MAPKEYS, EV_MAPKEYS };
struct Event { int kind; int parent; long value; bool pc; const RegionInfo* info; bool recompute; };

struct Map {
    RegionInfo infos[8]; int info_count;
    long visited[8]; int visited_count;
    long hero_region;
    Event events[32]; int event_count; int next_component;
    int Add(int kind, int parent, long value, bool pc, const RegionInfo* info = 0, bool recompute = false)
    {
        Event& e = events[event_count++];
        e.kind = kind; e.parent = parent; e.value = value; e.pc = pc; e.info = info; e.recompute = recompute;
        return ++next_component;
    }
    void ConstructRegionDescription(int parent, const Region& region, bool pc)
    {
        bool recompute = false;
        RegionInfo* info = 0;
        if (region.number == hero_region) recompute = true;
        for (int i = 0; i < info_count; ++i)
            if (infos[i].Region == region.number) { info = &infos[i]; break; }
        if (!info) {
            std::memset(&infos[info_count], 0, sizeof infos[info_count]);
            info = &infos[info_count++];
            info->Region = region.number;
        }
        int container = Add(EV_CONTAINER, parent, 0, pc);
        if (region.has_def && region.description) Add(EV_TEXT_ID, container, (long)region.description, pc);
        else Add(EV_TEXT_NAME, container, 0, pc);
        Add(EV_TEXT_CRIME, container, 0, pc);
        Add(EV_QUESTS, parent, region.number, pc);
        bool is_visited = false;
        for (int v = 0; v < visited_count; ++v) if (visited[v] == region.number) is_visited = true;
        container = Add(EV_CONTAINER, parent, 0, pc);
        if (is_visited) {
            Add(EV_SHOPS, container, region.number, pc, info, recompute);
            Add(EV_BUYABLE, container, region.number, pc, info, recompute);
        }
        Add(EV_TEXT_MAPKEYS, container, 0, pc);
        Add(EV_MAPKEYS, parent, region.number, pc);
    }
};

static int Fail(int n) { std::printf("00605570_TEST FAIL\n"); return n ? 1 : 1; }

int main()
{
    Map map; std::memset(&map, 0, sizeof map);
    map.hero_region = 5;
    map.infos[0].Region = 3; map.infos[0].Shops = 2; map.info_count = 1;
    map.visited[0] = 3; map.visited[1] = 5; map.visited_count = 2;

    // Cached, visited, not the hero's region: the existing record, no recompute; description by text id.
    Region r3 = { 3, true, 0x1234, "R3" };
    map.ConstructRegionDescription(100, r3, true);
    if (map.event_count != 9 || map.info_count != 1) return Fail(1);
    if (map.events[0].kind != EV_CONTAINER || map.events[0].parent != 100 || !map.events[0].pc) return Fail(2);
    if (map.events[1].kind != EV_TEXT_ID || map.events[1].value != 0x1234 || map.events[1].parent != 1) return Fail(3);
    if (map.events[2].kind != EV_TEXT_CRIME || map.events[3].kind != EV_QUESTS || map.events[3].parent != 100) return Fail(4);
    if (map.events[5].kind != EV_SHOPS || map.events[5].info != &map.infos[0] || map.events[5].recompute) return Fail(5);
    if (map.events[6].kind != EV_BUYABLE || map.events[6].parent != map.events[5].parent) return Fail(6);
    if (map.events[7].kind != EV_TEXT_MAPKEYS || map.events[8].kind != EV_MAPKEYS || map.events[8].parent != 100) return Fail(7);

    // New record for the hero's (visited) region: appended with its number, recompute set; no def -> display name.
    map.event_count = 0;
    Region r5 = { 5, false, 0, "R5" };
    map.ConstructRegionDescription(200, r5, false);
    if (map.info_count != 2 || map.infos[1].Region != 5 || map.infos[1].Shops != 0) return Fail(8);
    if (map.events[1].kind != EV_TEXT_NAME || map.events[0].pc) return Fail(9);
    if (map.events[5].kind != EV_SHOPS || map.events[5].info != &map.infos[1] || !map.events[5].recompute) return Fail(10);

    // Not visited: no shop / building lines; a def without a description id falls back to the display name.
    map.event_count = 0;
    Region r9 = { 9, true, 0, "R9" };
    map.ConstructRegionDescription(300, r9, false);
    if (map.event_count != 7 || map.info_count != 3 || map.events[1].kind != EV_TEXT_NAME) return Fail(11);
    if (map.events[5].kind != EV_TEXT_MAPKEYS || map.events[6].kind != EV_MAPKEYS) return Fail(12);
    std::printf("00605570_TEST PASS\n");
    return 0;
}
