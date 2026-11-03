// Behaviour test for CTCInventoryMap::CreateMiniMapMarkers 0x00603000 (model copy): the prison / Headsman's Hill flag logic,
// the script-marker visibility tests, the cached-marker erase for the hero's region and the persistent marker record.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
struct Pos { float X, Y, Z; };
struct Flags { bool Court1, Court2, North, South; };
struct Marker { long Region; std::string UIDefName; float X, Y; long DefNameLength, GraphicIndex; float Angle; };
static bool IsThingAsPosBeingDrawn(const Flags& f, const Pos& p)
{
    if (f.Court1 && p.Z > 51.0f) return false;
    if (f.Court2 && p.Z <= 51.0f) return false;
    if (f.North && p.Y <= 4740.0f) return false;
    if (f.South && p.Y > 4740.0f) return false;
    return true;
}
static void FlipHeadsmansHill(Flags& f, const Pos& hero)
{
    if (f.North && hero.Y <= 4740.0f) { f.North = false; f.South = true; }
    else if (f.South && hero.Y > 4740.0f) { f.North = true; f.South = false; }
}
static void SetPrisonFlags(Flags& f, long graphic, long court1, long court2, long tree)
{
    f.Court1 = f.Court2 = f.North = f.South = false;
    if (graphic == court1) f.Court1 = true;
    else if (graphic == court2) { f.Court1 = false; f.Court2 = true; }
    else if (graphic == tree) f.North = true;
}
static void EraseRegion(std::vector<Marker>& markers, long region)
{
    for (std::vector<Marker>::iterator it = markers.begin(); it != markers.end(); ) {
        if (it->Region == region) it = markers.erase(it); else ++it;
    }
}
static Marker Persistent(long region, const char* name, float x, float y)
{
    Marker m; m.Region = 0; m.UIDefName = ""; m.X = 0; m.Y = 0; m.DefNameLength = 0; m.GraphicIndex = 0; m.Angle = 0;
    m.X = x; m.Y = y; m.Region = region; m.UIDefName = name; m.DefNameLength = (long)m.UIDefName.size(); m.GraphicIndex = 0;
    return m;
}
int main()
{
    Flags f;
    SetPrisonFlags(f, 7, 5, 7, 9);
    if (f.Court1 || !f.Court2 || f.North) { std::printf("00603000_TEST FAIL\n"); return 1; }
    Pos low = {0, 0, 10}, high = {0, 0, 80};
    if (IsThingAsPosBeingDrawn(f, low) || !IsThingAsPosBeingDrawn(f, high)) { std::printf("00603000_TEST FAIL\n"); return 1; }
    SetPrisonFlags(f, 9, 5, 7, 9);
    Pos south = {0, 4000, 0}, north = {0, 5000, 0};
    FlipHeadsmansHill(f, south);
    if (f.North || !f.South) { std::printf("00603000_TEST FAIL\n"); return 1; }
    if (IsThingAsPosBeingDrawn(f, north) || !IsThingAsPosBeingDrawn(f, south)) { std::printf("00603000_TEST FAIL\n"); return 1; }
    FlipHeadsmansHill(f, north);
    if (!f.North || f.South) { std::printf("00603000_TEST FAIL\n"); return 1; }
    std::vector<Marker> cache;
    cache.push_back(Persistent(3, "UI_TELEPORT_MARKER", 1, 2));
    cache.push_back(Persistent(4, "UI_WIFE_MARKER", 3, 4));
    cache.push_back(Persistent(3, "UI_TRADER_MARKER", 5, 6));
    EraseRegion(cache, 3);
    if (cache.size() != 1 || cache[0].Region != 4 || cache[0].DefNameLength != 14 || cache[0].GraphicIndex != 0) {
        std::printf("00603000_TEST FAIL\n"); return 1;
    }
    std::printf("00603000_TEST PASS\n");
    return 0;
}
