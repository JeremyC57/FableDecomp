// Behaviour test for 00436500: src/00436500.cpp copied verbatim; the mapwho level table defined here so the box
// conversion (min edges shifted down, max edges `((v - 1) >> shift) + 1`) is checked per level.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingSearchTools::GetMapwhoArea(CThingMap*, long, const C2DBoxI&) -- converts a world box into the mapwho cells of `level` (min edges shifted down, max edges rounded up)
// Retry 10b (B-12 sweep): the level entry pointer is bound to a const REFERENCE (`const SMapwhoLevel* const& info = &g_mapwhoLevels[level];`, a reference to a pointer temporary): retail materialises the entry address (`shl eax,4 ; add eax,137DAD8h ; mov ecx,[eax+4]`, ebx pushed between); a plain pointer local folds it into `mov ecx,[eax+137DADCh]`.
// Retail: Fable.exe 0x00436500, 66 bytes (`ret 10h`: hidden result pointer + three arguments; `this` and
// the CThingMap argument are unused).
// Data: the mapwho level table at 0x0137DAD8 (16-byte entries, the scale shift at +4).
// The generated CThingSearchTools.h is included for `this`; C2DBoxI (four longs) and the level entry are
// local models (the PDB CMapwhoLevelInfo is 20 bytes, the retail table stride is 16).
// Shape: the box built field by field in a named local (fields computed in x0,y0,x1,y1 order) and returned
// through a memberwise copy constructor; the shift loaded into ecx once (`sar r,cl` four times), the max
// edges as `((v - 1) >> shift) + 1`.
#include "engine/CThingSearchTools.h"

struct CThingMap;

class C2DBoxI { public:
    long x0, y0, x1, y1;
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& o) : x0(o.x0), y0(o.y0), x1(o.x1), y1(o.y1) {}
};

struct SMapwhoLevel {
    long m_size;                                         // +0x00
    long m_scaleShift;                                   // +0x04
    long m_extents[2];                                   // +0x08
};

extern SMapwhoLevel g_mapwhoLevels[];                    // 0x0137DAD8

struct CThingSearchToolsMethods : CThingSearchTools {
    C2DBoxI GetMapwhoArea(CThingMap* map, long level, const C2DBoxI& area);
};

C2DBoxI CThingSearchToolsMethods::GetMapwhoArea(CThingMap* map, long level, const C2DBoxI& area)
{
    const SMapwhoLevel* const& info = &g_mapwhoLevels[level];
    long shift = info->m_scaleShift;
    C2DBoxI result;
    result.x0 = area.x0 >> shift;
    result.y0 = area.y0 >> shift;
    result.x1 = ((area.x1 - 1) >> shift) + 1;
    result.y1 = ((area.y1 - 1) >> shift) + 1;
    return result;
}
// ---- end source ----
SMapwhoLevel g_mapwhoLevels[3] = { { 0, 0, { 0, 0 } }, { 0, 3, { 0, 0 } }, { 0, 5, { 0, 0 } } };
int main()
{
    static unsigned char raw[sizeof(CThingSearchToolsMethods)];
    CThingSearchToolsMethods* tools = (CThingSearchToolsMethods*)raw;
    C2DBoxI box; box.x0 = 17; box.y0 = -9; box.x1 = 33; box.y1 = 40;
    C2DBoxI r = tools->GetMapwhoArea(0, 1, box);                 // shift 3
    CHECK(r.x0 == (17 >> 3) && r.y0 == (-9 >> 3) && r.x1 == ((33 - 1) >> 3) + 1 && r.y1 == ((40 - 1) >> 3) + 1);
    CHECK(r.x0 == 2 && r.y0 == -2 && r.x1 == 5 && r.y1 == 5);
    r = tools->GetMapwhoArea(0, 2, box);                         // shift 5
    CHECK(r.x0 == 0 && r.y0 == -1 && r.x1 == 2 && r.y1 == 2);
    r = tools->GetMapwhoArea(0, 0, box);                         // shift 0: max edges unchanged
    CHECK(r.x0 == 17 && r.y0 == -9 && r.x1 == 33 && r.y1 == 40);
    if (fails == 0) printf("00436500_TEST PASS\n"); else printf("00436500_TEST FAIL\n");
    return fails;
}