#include <cstdio>
#include <cstring>
// Behaviour model of CTCQuestCard::GetMarkerRegionName: the teleporter region name, blanked when the region is known and
// routable from the current region.
struct Map {
    long current; long routeResult;
    long GetRegionNumberFromName(const char* n) const { return std::strcmp(n, "Oakvale") == 0 ? 7 : (std::strcmp(n, "Nowhere") == 0 ? 0 : 3); }
    long GetCurrentRegion() const { return current; }
    long GetNextRegionOnRouteTo(long, long) const { return routeResult; }
};
struct Card {
    const char* TeleporterRegionName; Map* map;
    const char* GetMarkerRegionName() const
    {
        const char* name = TeleporterRegionName;
        if (name[0] != 0) {
            long region = map->GetRegionNumberFromName(name);
            if (region > 0)
                if (map->GetNextRegionOnRouteTo(map->GetCurrentRegion(), region) != -1)
                    name = "";
        }
        return name;
    }
};
int main()
{
    Map m; m.current = 1; m.routeResult = 4;
    Card c; c.map = &m; c.TeleporterRegionName = "Oakvale";
    if (c.GetMarkerRegionName()[0] != 0) { std::printf("007021f0_TEST FAIL\n"); return 1; }
    m.routeResult = -1;
    if (std::strcmp(c.GetMarkerRegionName(), "Oakvale")) { std::printf("007021f0_TEST FAIL\n"); return 1; }
    m.routeResult = 4; c.TeleporterRegionName = "Nowhere";
    if (std::strcmp(c.GetMarkerRegionName(), "Nowhere")) { std::printf("007021f0_TEST FAIL\n"); return 1; }
    std::printf("007021f0_TEST PASS\n");
    return 0;
}