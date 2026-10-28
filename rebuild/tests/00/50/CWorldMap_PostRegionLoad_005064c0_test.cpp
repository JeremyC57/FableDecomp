// Behaviour test for CWorldMap::PostRegionLoad (0x005064C0): each village is populated once (flag set), otherwise it
// gets its missing dummy villagers; every village fills its crate stacks with 2 and adds crate tasks from {2} to {0,3};
// sound themes are re-enabled at the end.
#include <cstdio>

struct CVillage {
    bool PopulatedOnce; int Populated, Dummies, Filled; long From[4]; int FromCount; long To[4]; int ToCount;
    CVillage(bool populated) : PopulatedOnce(populated), Populated(0), Dummies(0), Filled(0), FromCount(0), ToCount(0) { }
};
class CWorldMap { public:
    bool SoundThemesDisabled; int Progress;
    CWorldMap() : SoundThemesDisabled(true), Progress(0) { }
    void PopulateRegionWithVillagers(CVillage& v) { ++v.Populated; }
    void PostRegionLoad(CVillage** villages, int count)
    {
        for (int i = 0; i < count; ++i) {
            ++Progress;
            CVillage* village = villages[i];
            if (!village->PopulatedOnce) {
                PopulateRegionWithVillagers(*village);
                village->PopulatedOnce = true;
            } else
                ++village->Dummies;
            village->Filled = 2;
            village->From[village->FromCount++] = 2;
            village->To[village->ToCount++] = 0;
            village->To[village->ToCount++] = 3;
            ++Progress;
        }
        SoundThemesDisabled = false;
    }
};

int main()
{
    CVillage a(false), b(true); CVillage* list[2] = { &a, &b };
    CWorldMap map; map.PostRegionLoad(list, 2);
    bool ok = a.Populated == 1 && a.PopulatedOnce && a.Dummies == 0 && b.Populated == 0 && b.Dummies == 1
        && a.Filled == 2 && b.Filled == 2 && a.FromCount == 1 && a.From[0] == 2 && a.ToCount == 2 && a.To[0] == 0
        && a.To[1] == 3 && map.Progress == 4 && !map.SoundThemesDisabled;
    map.PostRegionLoad(list, 2);
    ok = ok && a.Populated == 1 && a.Dummies == 1 && b.Dummies == 2;
    CWorldMap empty; empty.PostRegionLoad(list, 0);
    ok = ok && !empty.SoundThemesDisabled && empty.Progress == 0;
    if (!ok) { std::printf("005064c0_TEST FAIL\n"); return 1; }
    std::printf("005064c0_TEST PASS\n");
    return 0;
}