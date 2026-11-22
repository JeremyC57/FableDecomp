// CHeightMap::Resize(long, long, long, long) -- clears a resized grid and copies its shifted overlap.
// Leaf: ?Resize@CHeightMap@@AAEXJJJJ@Z
// Flags: /Gr
// Inline-budget: the original vector size constructor and assignment stay visible to VC7.1. The size constructor remains an out-of-line call.
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_CHEIGHTMAP_CELLINDEX
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_STLP_VECTOR_SIZECTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_CHEIGHTMAPCELL_EMPTY_CTOR_PASS27
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_AND_COPY_ORIGINAL_SCOPE_PASS22
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C2DCoordI.h"
#include "engine/CHeightMap.h"
#include "fable_assert.h"

void CHeightMap::Resize(long width, long height, long x_offset, long y_offset)
{
    __int64 overflow_check = ((__int64)width + GetExtraCells()) * ((__int64)height + GetExtraCells());
    if (overflow_check < 0 || overflow_check > 0x7fffffffL) {
        FABLE_RETAIL_ASSERT(false);
        return;
    }

    std::vector<CHeightMapCell> new_cells((width + GetExtraCells()) * (height + GetExtraCells()));
    std::for_each(new_cells.begin(), new_cells.end(), std::mem_fun_ref(&CHeightMapCell::Clear));

    long to_x = std::max(0L, -x_offset);
    for (long from_x = std::max(0L, x_offset); from_x < std::min(SizeX, width + x_offset); ++from_x, ++to_x) {
        long to_y = std::max(0L, -y_offset);
        for (long from_y = std::max(0L, y_offset); from_y < std::min(SizeY, height + y_offset); ++from_y, ++to_y) {
            long to_index = GetIndexIntoCells(to_x, to_y, width, height);
            FABLE_RETAIL_ASSERT(to_index >= 0 && (unsigned long)to_index < new_cells.size());
            new_cells[to_index] = GetCell(C2DCoordI(from_x, from_y));
        }
    }
    Cells = new_cells;
    SizeX = width;
    SizeY = height;
    SizeXIncludingExtraCells = width + GetExtraCells();
    SizeYIncludingExtraCells = height + GetExtraCells();
    NoMapUpdateAreas = 0;
}