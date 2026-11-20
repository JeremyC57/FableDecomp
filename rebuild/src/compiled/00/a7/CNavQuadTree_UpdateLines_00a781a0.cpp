// CNavQuadTree::UpdateLines(line_lists, lines, update_area) const -- rebuild the per-cell line lists of an area: clear every 8 x 8 cell's list in the area while noting the cell's world box, then file every line into each cell of its bounding range whose box holds one of its end points or is crossed by it
// Leaf: ?UpdateLines@CNavQuadTree@@ABEXAAV?$vector@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@V?$allocator@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@2@@std@@ABV?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@3@ABVC2DBoxI@@@Z
// Flags: none
// Retail-asm: 0x00A78359 `fld dword ptr [ebp+64h]` / 0x00A7835C `fadd dword ptr [ebp+24h]` / 0x00A7835F `fistp dword ptr [ebp+30h]` ..
//   0x00A78373 `dec dword ptr [ebp+30h]` (and the three sites after it) -- the engine's inline-assembly ceiling and floor, through the
//   shared engine/GFFloatToLongCeil.h / GFFloatToLongFloor.h helpers. No asm in this source.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A781A0, 1244 bytes, `ret 0Ch` (ebp-biased frame, `lea ebp,[esp-6Ch]`).
// Fidelity: the TU also emits the std::vector<C2DBoxF>::vector(size_type) COMDAT the row calls for `node_areas`; on the shared
//   headers' default path (the sized constructor a plain inline, which VC7.1 leaves out of line here by itself) with C2DBoxF's
//   user copy constructor (FABLE_RETAIL_C2DBOXF_COPY) its bytes equal retail 0x00A80360 (122 / 122 B, 0 differences).
//   Audit 2026-11-20: the delivered text forced this with three stlport opt-ins (a noinline sized constructor and two FORCEINLINE
//   fill helpers, one of them new); none is needed -- the row and the COMDAT are the same bytes without them -- so they are gone
//   and this row changes no stlport header. Declaration-only (FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE) the row is 134 instructions
//   off (the author's measurement): the body must be visible.
// Audit 2026-11-20: the four limits are GFLimitUpper / GFLimitLower calls: Ego_d's lines 1900 / 1902 / 1904 / 1906 are 14 / 14 /
//   11 / 14 debug bytes, exactly this text's /Od sizes (a fastcall call with two references; the TLX one has no member offset to
//   add); `if (v > bound) v = bound;` statements are 20 / 20 / 18 / 20. Same retail bytes either way.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_INITIALISE
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_CONTAINSPOINT_INLINE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_UPDATELINES
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DLineF.h"
#include "engine/CNavQuadTree.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFFloatToLongFloor.h"

// Ego_d.pdb / FableWin.pdb ??$GFLimitUpper@JJ@@YIXAAJABJ@Z / ??$GFLimitLower@JJ@@YIXAAJABJ@Z (a maths header, file id 480: lines 81..83 and
// 87..89, parameters x, min / x, max; 35 debug bytes each); expanded in retail. Row-local as the landed users define them.
template <class T, class U> inline void FABLE_FASTCALL GFLimitLower(T& x, const U& min) { if (x < min) x = min; }
template <class T, class U> inline void FABLE_FASTCALL GFLimitUpper(T& x, const U& max) { if (x > max) x = max; }

void CNavQuadTree::UpdateLines(std::vector<std::list<C2DLineF> >& line_lists, const std::vector<C2DLineF>& lines, const C2DBoxI& update_area) const
{
    long full_boxes_wide = (long)(MapWidth * 0.125f);
    std::vector<C2DBoxF> node_areas(update_area.GetWidth() * update_area.GetHeight());

    for (unsigned long x = update_area.TLX; x < (unsigned long)update_area.BRX; x++)
    {
        for (unsigned long y = update_area.TLY; y < (unsigned long)update_area.BRY; y++)
        {
            // Bracket: `(y * 8.0f)` below is load-bearing (measured: without it c2 schedules `sub eax, esi` at 0x00A7824F before
            //   `fld st(1)`, one slot off retail); `(x * 8.0f)` is byte-neutral and bracketed to keep the two lines alike.
            float tlx = (x * 8.0f) + MapPos.X;
            float tly = (y * 8.0f) + MapPos.Y;
            float brx = tlx + 8.0f;
            float bry = tly + 8.0f;
            node_areas[(y - update_area.TLY) * update_area.GetWidth() + (x - update_area.TLX)].Initialise(tlx, tly, brx, bry);
            line_lists[y * full_boxes_wide + x].clear();
        }
    }

    for (std::vector<C2DLineF>::const_iterator ln_it = lines.begin(); ln_it != lines.end(); ++ln_it)
    {
        const C2DLineF& line = *ln_it;
        long high_x = GFFloatToLongCeil((line.GetHighestX() - MapPos.X) * 0.125f);
        GFLimitUpper(high_x, update_area.BRX);
        long high_y = GFFloatToLongCeil((line.GetHighestY() - MapPos.Y) * 0.125f);
        GFLimitUpper(high_y, update_area.BRY);
        long low_x = GFFloatToLongFloor((line.GetLowestX() - MapPos.X) * 0.125f);
        GFLimitLower(low_x, update_area.TLX);
        long low_y = GFFloatToLongFloor((line.GetLowestY() - MapPos.Y) * 0.125f);
        GFLimitLower(low_y, update_area.TLY);

        for (long x = low_x; x < high_x; x++)
        {
            for (long y = low_y; y < high_y; y++)
            {
                __int64 overflow_check = ((__int64)y - (__int64)update_area.TLY) * (__int64)update_area.GetWidth() + ((__int64)x - (__int64)update_area.TLX);
                if (overflow_check < 0 || overflow_check > 0x7fffffff)
                {
                    return;
                }
                // Inferred: `index` is declared after the overflow return, as the Ego_d line table has it (line 1911 the 64-bit sum, 1912 the
                //   range test, 1915 the return, 1918 the 32-bit index); declared before `overflow_check` the body is 1250 bytes against 1244.
                long index = (y - update_area.TLY) * update_area.GetWidth() + (x - update_area.TLX);
                if ((unsigned long)index > node_areas.size() || index < 0)
                {
                    continue;
                }
                if (node_areas[index].ContainsPoint(line.Point1) || node_areas[index].ContainsPoint(line.Point2) || line.IntersectsWith(node_areas[index]))
                {
                    line_lists[y * full_boxes_wide + x].push_back(line);
                }
            }
        }
    }
}