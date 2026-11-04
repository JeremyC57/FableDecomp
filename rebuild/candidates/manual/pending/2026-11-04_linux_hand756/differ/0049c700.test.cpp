// Behaviour test for CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea (0x0049C700): a model of the row -- the area is
// clipped to the map's area (each edge clamped into [TL, BR]) and shifted so the map's top-left corner is the origin.
#include <stdio.h>
struct Box { long TLX, TLY, BRX, BRY; };
static long Clamp(long v, long lo, long hi) { return v < lo ? lo : (v > hi ? hi : v); }
static Box ClipAndConvert(const Box& mapArea, const Box& area)
{
    Box b = area;
    b.TLX = Clamp(b.TLX, mapArea.TLX, mapArea.BRX); b.TLY = Clamp(b.TLY, mapArea.TLY, mapArea.BRY);
    b.BRX = Clamp(b.BRX, mapArea.TLX, mapArea.BRX); b.BRY = Clamp(b.BRY, mapArea.TLY, mapArea.BRY);
    b.TLX -= mapArea.TLX; b.BRX -= mapArea.TLX; b.TLY -= mapArea.TLY; b.BRY -= mapArea.TLY;
    return b;
}
int main()
{
    Box map = { 100, 200, 164, 264 }, a = { 90, 210, 120, 300 };
    Box r = ClipAndConvert(map, a);
    int ok = r.TLX == 0 && r.TLY == 10 && r.BRX == 20 && r.BRY == 64;
    Box inside = { 110, 220, 130, 240 }; r = ClipAndConvert(map, inside);
    ok = ok && r.TLX == 10 && r.TLY == 20 && r.BRX == 30 && r.BRY == 40;
    printf(ok ? "0049c700_TEST PASS\n" : "0049c700_TEST FAIL\n");
    return ok ? 0 : 1;
}
