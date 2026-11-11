// CHeightMap::Resize(long sizeX, long sizeY, long offsetX, long offsetY) -- rebuild the cell grid at a new size: a cleared grid of (sizeX+1)*(sizeY+1) cells, the overlapping part of the old grid copied across shifted by the offsets, then the new grid and sizes taken over and the pending update areas dropped
// Grade: DIFFER(518v530) -- 3 grades. Loop structure matches with the two-variable for loops (newX/oldX, newY/oldY) and std::max(0L, -off) / std::min(Size, size + off); residue: frame 0x24 v 0x28 -- retail homes sizeX + 1 in a frame word ([esp+1Ch], `imul eax,[esp+1Ch]`, step from `or eax,-1 ; sub eax,sizeX`) and keeps sizeY in ebp / the zero in ebx (ours ebx / ebp), and adds the offset INTO sizeX's register (`add esi,ecx`). Next: drop the two named newSize locals (use sizeX + 1 / sizeY + 1 in place, end stores in the order XInc, SizeX, SizeY, YInc), or declare them after the vector.
// Leaf: ?Resize@CHeightMap@@AAEXJJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F61A0, 530 bytes, thiscall, `ret 10h`; Ego_r.pdb ?Resize@CHeightMap@@AAEXJJJJ@Z.
// Callees: _allmul 0x00BFEB50, vector<CHeightMapCell>::vector(size_type) 0x008F69B0, std::for_each<CHeightMapCell*,
//   mem_fun_ref_t<void, CHeightMapCell> > 0x008F6740 with CHeightMapCell::Clear 0x008F5EB0, vector<CHeightMapCell>::operator=
//   0x008F6A90, free 0x00BFEA14 (the temporary grid's storage).
// Header overlay: engine/CHeightMap.h is new (hdr/README.md); grade with FABLE_HDR_OVERLAY.
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CHeightMap.h"

// The landed constructor's count guard (0x008F63C0): the 64-bit cell count must fit a long.
static inline bool IsSafeCount(__int64 count)
{
    return count >= 0 && count <= 0x7FFFFFFF;
}

void CHeightMap::Resize(long sizeX, long sizeY, long offsetX, long offsetY)
{
    if (IsSafeCount(((__int64)sizeX + 1) * ((__int64)sizeY + 1))) {
        long newSizeXIncludingExtraCells = sizeX + 1;
        long newSizeYIncludingExtraCells = sizeY + 1;
        std::vector<CHeightMapCell> newCells(newSizeYIncludingExtraCells * newSizeXIncludingExtraCells);
        std::for_each(newCells.begin(), newCells.end(), std::mem_fun_ref(&CHeightMapCell::Clear));

        // The part of the old grid that is still inside the new one moves across; rows are stored top down.
        for (long newX = std::max(0L, -offsetX), oldX = std::max(0L, offsetX); oldX < std::min(SizeX, sizeX + offsetX); ++oldX, ++newX) {
            for (long newY = std::max(0L, -offsetY), oldY = std::max(0L, offsetY); oldY < std::min(SizeY, sizeY + offsetY); ++oldY, ++newY) {
                newCells[(sizeY - newY) * newSizeXIncludingExtraCells + newX] = Cells[(SizeY - oldY) * (SizeX + 1) + oldX];
            }
        }

        Cells = newCells;
        SizeXIncludingExtraCells = newSizeXIncludingExtraCells;
        SizeX = sizeX;
        SizeY = sizeY;
        SizeYIncludingExtraCells = newSizeYIncludingExtraCells;
        NoMapUpdateAreas = 0;
    }
}
