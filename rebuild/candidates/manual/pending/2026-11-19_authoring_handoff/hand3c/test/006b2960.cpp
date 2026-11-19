// Behaviour test for CGameNavigatorManagerExternalInterface::GetNavigationInformation (0x006B2960), a portable model of the row. The
// float box becomes a cell box: its top-left is floored (GFFloatToLongFloor: fistp(v - 0.5) under round-to-nearest, plus one when
// the result + 1 equals v exactly) and its bottom-right is rounded to nearest-even (GFFloatToLongNear). The map number is looked up at
// the cell box's top-left, and the world map then fills `update` for (map, cell box, "NULL").
#include <stdio.h>
#include <string.h>
#include <math.h>
static long Near(double v)                                              // default RC: round to nearest, ties to even
{
    double f = floor(v); double d = v - f; long r = (long)f;
    if (d > 0.5 || (d == 0.5 && (r & 1))) ++r;
    return r;
}
static long Floor(float v)
{
    long r = Near((double)v - 0.5);
    float next = (float)((double)r + 1.0);
    if (memcmp(&v, &next, 4) == 0) ++r;
    return r;
}
struct Box { float TLX, TLY, BRX, BRY; };
struct BoxI { long TLX, TLY, BRX, BRY; };
struct Update { long map; BoxI box; char name[8]; };
static long MapNumberAt(long x, long y) { return (x >= 100 ? 2 : 1) + (y >= 100 ? 10 : 0); }
static void GetNavigationInformation(const Box& box, Update& update)
{
    BoxI cells = { Floor(box.TLX), Floor(box.TLY), Near(box.BRX), Near(box.BRY) };
    long map_number = MapNumberAt(cells.TLX, cells.TLY);
    update.map = map_number; update.box = cells; strcpy(update.name, "NULL");
}
int main()
{
    int bad = 0; Update u;
    Box a = { 3.7f, -1.25f, 9.5f, 10.5f };
    GetNavigationInformation(a, u);
    if (u.box.TLX != 3 || u.box.TLY != -2 || u.box.BRX != 10 || u.box.BRY != 10 || u.map != 1 || strcmp(u.name, "NULL")) ++bad;   // ties to even
    Box b = { 100.0f, 250.0f, 120.4f, 260.6f };                           // integral top-left floors to itself
    GetNavigationInformation(b, u);
    if (u.box.TLX != 100 || u.box.TLY != 250 || u.box.BRX != 120 || u.box.BRY != 261 || u.map != 12) ++bad;
    printf(bad ? "006b2960_TEST FAIL\n" : "006b2960_TEST PASS\n");
    return bad ? 1 : 0;
}
