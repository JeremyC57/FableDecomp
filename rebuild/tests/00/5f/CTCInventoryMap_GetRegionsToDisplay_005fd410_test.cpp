// Test for CTCInventoryMap::GetRegionsToDisplay (0x005FD410): the region set receives the region the parent thing's
// current map lies in, every region in VisitedRegions, and -- for each active quest card (copied list) whose
// CTCQuestCard region name is not "" -- the world map's region number for that name; the set keeps each region once.
#include <cstdio>
#include <cstring>
struct RegionSet {
    long v[32]; int n;
    RegionSet() : n(0) { }
    void insert(long x) { int i = 0; while (i < n && v[i] < x) ++i; if (i < n && v[i] == x) return; for (int j = n; j > i; --j) v[j] = v[j - 1]; v[i] = x; ++n; }
};
struct WorldMap {
    long GetRegionNumberMapIsIn(long map) const { return map / 10; }
    long GetRegionNumberFromName(const char* name) const
    {
        static const char* const names[4] = { "Oakvale", "Bowerstone", "Knothole", "Darkwood" };
        for (int i = 0; i < 4; ++i) if (std::strcmp(names[i], name) == 0) return 100 + i;
        return -1;
    }
};
struct QuestCard { const char* RegionName; const char* GetRegionName() const { return RegionName; } };
struct Thing { long CurrentMapNumber; };
struct InventoryMap {
    Thing* ParentThing; long VisitedRegions[8]; int VisitedCount;
    const QuestCard* const* ActiveQuestCards; int CardCount;
    void GetRegionsToDisplay(RegionSet& regions, const WorldMap& map) const
    {
        Thing* thing = ParentThing;
        regions.insert(map.GetRegionNumberMapIsIn(thing->CurrentMapNumber));
        for (int i = 0; i < VisitedCount; ++i) regions.insert(VisitedRegions[i]);
        const QuestCard* cards[8]; int n = CardCount;
        for (int i = 0; i < n; ++i) cards[i] = ActiveQuestCards[i];          // the list is copied before the walk
        for (int i = 0; i < n; ++i) {
            const QuestCard* questCard = cards[i];
            if (std::strcmp(questCard->GetRegionName(), "") != 0)
                regions.insert(map.GetRegionNumberFromName(questCard->GetRegionName()));
        }
    }
};
int main()
{
    WorldMap world; Thing hero = { 57 };
    QuestCard a = { "Bowerstone" }, b = { "" }, c = { "Darkwood" }, d = { "Bowerstone" };
    const QuestCard* cards[4] = { &a, &b, &c, &d };
    InventoryMap inv; inv.ParentThing = &hero; inv.VisitedCount = 3; inv.VisitedRegions[0] = 2; inv.VisitedRegions[1] = 5; inv.VisitedRegions[2] = 2;
    inv.ActiveQuestCards = cards; inv.CardCount = 4;
    RegionSet r; inv.GetRegionsToDisplay(r, world);
    static const long expect[4] = { 2, 5, 101, 103 };
    if (r.n != 4) { std::printf("005fd410_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) if (r.v[i] != expect[i]) { std::printf("005fd410_TEST FAIL\n"); return 1; }
    inv.VisitedCount = 0; inv.CardCount = 0; hero.CurrentMapNumber = 31; RegionSet r2; inv.GetRegionsToDisplay(r2, world);
    if (r2.n != 1 || r2.v[0] != 3) { std::printf("005fd410_TEST FAIL\n"); return 1; }
    std::printf("005fd410_TEST PASS\n");
    return 0;
}