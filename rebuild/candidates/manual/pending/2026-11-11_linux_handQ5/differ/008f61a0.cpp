// CHeightMap::Resize(long sizeX, long sizeY, long offsetX, long offsetY) -- rebuild the cell grid at a new size: a cleared grid of (sizeX+1)*(sizeY+1) cells, the overlapping part of the old grid copied across shifted by the offsets, then the new grid and sizes taken over and the pending update areas dropped
// Grade: DIFFER(518v530) -- Q5: 11 grades logged (9 hypotheses + baseline and final regrades; 19 in all), unchanged; this Q4 source is still the best (ratio 0.627). Residue: retail homes sizeX + 1 at [esp+1Ch] (index base `imul eax,[esp+1Ch]`, end store `mov eax,[esp+1Ch]`), takes the inner-loop step from the sizeX parameter in memory (`or eax,-1; sub eax,[esp+3Ch]`), keeps sizeX in esi only up to `add esi,ecx` (sizeX + offsetX), and gives ebx to the zero and then oldX; ours keeps sizeX in a callee-saved register to the end and rematerialises sizeX + 1 with `lea`. A literal (sizeX + 1) in the new-cells index gives retail's step form but 509v530 (the base is rematerialised too). Probed or graded inert: FableWin/Ego_r ?GetExtraCells@CHeightMap@@CIJXZ for every +1 (hdr opt-in FABLE_RETAIL_CHEIGHTMAP_GETEXTRACELLS: 518 / 509), GetIndexIntoCells as the top-down flip `(sizeY - y) * (sizeX + GetExtraCells()) + x` (opt-in FABLE_RETAIL_CHEIGHTMAP_INDEXFLIP, consistent with AlterHeightBy 0x008F60F5: 509; new cells only 518; through a cell reference 507), early-return guard 509, the named sizes after the vector 518 / 509. Next: something in the source that ends sizeX's register piece at the sum (the step and end store read the parameter slot) -- likely a different source shape around the loops, not a renaming.
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
