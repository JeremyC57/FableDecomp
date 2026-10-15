// Behaviour test for 0x006A7960 CThingFilter_InRegion::operator()
#include <cstdio>
struct CThing { short CurrentMapNumber; };
struct CWorldMap {
    long GetRegionNumberMapIsIn(long map) { return map / 10; }
};
struct CThingFilter_InRegion {
    CWorldMap* WorldMap;
    long       RegionID;
    bool operator()(CThing* thing) const
    {
        return WorldMap->GetRegionNumberMapIsIn(thing->CurrentMapNumber) == RegionID;
    }
};
int main()
{
    CWorldMap w; CThing t; t.CurrentMapNumber = 34;
    CThingFilter_InRegion f; f.WorldMap = &w; f.RegionID = 3;
    bool ok = f(&t);
    t.CurrentMapNumber = 41; if (f(&t)) ok = false;
    t.CurrentMapNumber = -1; f.RegionID = 0; if (!f(&t)) ok = false;
    if (!ok) { std::printf("006a7960_TEST FAIL\n"); return 1; }
    std::printf("006a7960_TEST PASS\n");
    return 0;
}
