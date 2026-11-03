// CNavQuadTree::IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lineLists) const -- true when any navigation line of any list touches the area: a fuzzy bounding-range reject on X and Y, then either end point inside the box or the segment crossing it
// Leaf: ?IsAreaBlockedByLines@CNavQuadTree@@ABE_NABVC2DBoxF@@ABV?$vector@PBV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@V?$allocator@PBV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@2@@std@@@Z
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe @ 0x00A76F30, 371 bytes, `ret 8`, 8-aligned frame for the double temporaries.
// Body: for each list, for each line: skip when GetHighestX() - area.TLX <= -eps, GetLowestX() - area.BRX >= eps,
// GetHighestY() - area.TLY <= -eps or GetLowestY() - area.BRY >= eps (eps = the float at 0x0129BA3C); else
// area.ContainsPoint(Point1) / ContainsPoint(Point2) (0x00535CD0) or IntersectsWith(area) (0x00A57BD0) -> true.
// Layout: engine/CNavQuadTree.h FABLE_RETAIL_CNAVQUADTREE (the Ego_r private const member; landing 2026-11-03g moved the row
// off the `CNavQuadTree_Methods` wrapper, chunk 736 NAME_FIXES), engine/C2DBoxF.h {TLX, TLY, BRX, BRY} + ContainsPoint,
// engine/C2DLineF.h {Point1, Point2} + the extent getters; STLport list / vector from stlport/fable_containers.h.
// Shape: GetHighestX/GetLowestX/... are the landed C2DLineF bodies (0x00A56B80..0x00A56C10: (float)Max<double> /
// Min<double> over const-reference templates on double temporaries), inlined here; a `const C2DLineF& line = *it`
// reference local (retail forms &Point2 from &line, `lea eax,[esi+8]`).
// Opt: /O2 (16-aligned entry).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVQUADTREE
#include "engine/CNavQuadTree.h"

extern const float DELTA_NOTIONAL_ZERO; // 0x0129BA3C

bool CNavQuadTree::IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lineLists) const
{
    for (std::vector<const std::list<C2DLineF>*>::const_iterator it = lineLists.begin(); it != lineLists.end(); ++it) {
        const std::list<C2DLineF>* lines = *it;
        for (std::list<C2DLineF>::const_iterator it2 = lines->begin(); it2 != lines->end(); ++it2) {
            const C2DLineF& line = *it2;
            if (line.GetHighestX() - area.TLX <= -DELTA_NOTIONAL_ZERO)
                continue;
            if (line.GetLowestX() - area.BRX >= DELTA_NOTIONAL_ZERO)
                continue;
            if (line.GetHighestY() - area.TLY <= -DELTA_NOTIONAL_ZERO)
                continue;
            if (line.GetLowestY() - area.BRY >= DELTA_NOTIONAL_ZERO)
                continue;
            if (area.ContainsPoint(line.Point1) || area.ContainsPoint(line.Point2) || line.IntersectsWith(area))
                return true;
        }
    }
    return false;
}
