// Behaviour test for 004fdcb0: CWorldMap::IsSectionToHaveNavigationMap -- true for the "NULL" or empty section, else
// true when a game object, then a building, of that quest section stands in the map (InMap && IsInQuest filters).
#include <cstdio>
#include <cstring>

struct Thing { int kind; long map; const char* section; };   // kind 0 = object, 1 = building
static Thing g_things[8];
static int g_count = 0;
static int g_searches = 0;

static const Thing* first_thing(int kind, long map, const char* section)
{
    ++g_searches;
    for (int i = 0; i < g_count; ++i)
        if (g_things[i].kind == kind && g_things[i].map == map && std::strcmp(g_things[i].section, section) == 0)
            return &g_things[i];
    return 0;
}
static bool is_section_to_have_navigation_map(const char* section, long mapNo)
{
    if (std::strcmp(section, "NULL") == 0 || std::strcmp(section, "") == 0)
        return true;
    {
        const Thing* object = first_thing(0, mapNo, section);
        if (object != 0)
            return true;
    }
    {
        const Thing* building = first_thing(1, mapNo, section);
        if (building != 0)
            return true;
    }
    return false;
}

int main()
{
    Thing t0 = { 0, 3, "QuestA" }, t1 = { 1, 4, "QuestB" };
    g_things[0] = t0; g_things[1] = t1; g_count = 2;
    bool ok = true;
    g_searches = 0;
    ok = ok && is_section_to_have_navigation_map("NULL", 9) && is_section_to_have_navigation_map("", 9) && g_searches == 0;
    ok = ok && is_section_to_have_navigation_map("QuestA", 3) && g_searches == 1;      // the object search hits
    g_searches = 0;
    ok = ok && is_section_to_have_navigation_map("QuestB", 4) && g_searches == 2;      // falls through to buildings
    g_searches = 0;
    ok = ok && !is_section_to_have_navigation_map("QuestA", 4) && g_searches == 2;
    ok = ok && !is_section_to_have_navigation_map("null", 3);                          // the "NULL" test is exact
    if (!ok) { std::printf("004fdcb0_TEST FAIL\n"); return 1; }
    std::printf("004fdcb0_TEST PASS\n");
    return 0;
}