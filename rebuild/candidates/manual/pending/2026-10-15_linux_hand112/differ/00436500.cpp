// CThingSearchTools::GetMapwhoArea(CThingMap*, long, const C2DBoxI&) -- converts a world box into the mapwho cells of `level` (min edges shifted down, max edges rounded up)
// Grade: DIFFER(64v66) -- retail materialises the level entry address (`shl eax,4 ; add eax,137DAD8h ; mov ecx,[eax+4]`, ebx pushed first) where the lane folds it into `mov ecx,[eax+137DADCh]`; everything else matches
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

struct C2DBoxI {
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
    const SMapwhoLevel* info = &g_mapwhoLevels[level];
    long shift = info->m_scaleShift;
    C2DBoxI result;
    result.x0 = area.x0 >> shift;
    result.y0 = area.y0 >> shift;
    result.x1 = ((area.x1 - 1) >> shift) + 1;
    result.y1 = ((area.y1 - 1) >> shift) + 1;
    return result;
}
