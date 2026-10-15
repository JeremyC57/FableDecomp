// CRegionMinimap::InitialiseRegionBox() -- derive the square region-map box: the region map box's top-left corner with both sides set to the larger of its width and height
// Grade: DIFFER(ours v retail) -- 90v94: retail copies the top-left words before subtracting (`mov edx,esi ; sub eax,edx`, `mov ebx,edi ; sub edx,ebx`) and builds the larger-width arm's right edge in ebx (`lea ebx,[eax+esi] ; ... mov eax,ebx`); the lane subtracts esi/edi directly and allocates the arms' sums differently
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C7B0, length 94 bytes. No callees.
// Layout: engine/CRegionMinimap.h (PDB names; the leading members agree with retail): RegionMapBox (C2DBoxI) +0x04..+0x10,
// RegionMapSquare (C2DBoxI) +0x14..+0x20.
// Shape: three arms (`cmp ; jne` equal first, then `jle`) that each end in the same four stores of a common join
// (tail-duplicated: TLY, TLX, BRX, BRY with the right/bottom edges in eax/edx). The register copies of TLX/TLY suggest the
// top-left and the extents come from the inlined GetRegionMapExtents (0x0066C6A0, same object) into locals.
// Tried: flat if/else clamp of one side, three arms each storing a C2DBoxI temporary (struct copy through `add ecx,14h`),
// per-arm field stores, a brx/bry join with the header fields, and the join over the inlined GetRegionMapExtents (this file).
// Leaf: InitialiseRegionBox
#include "engine/CRegionMinimap.h"
struct C2DVectorI {
    long X, Y;
    C2DVectorI() {}
    C2DVectorI(long x, long y) : X(x), Y(y) {}
};
struct C2DExtentsI {
    long Width, Height;
    C2DExtentsI() {}
    C2DExtentsI(long width, long height) : Width(width), Height(height) {}
};
struct CRegionMinimap_InitialiseRegionBox : CRegionMinimap {
    void GetRegionMapExtents(C2DVectorI& topLeft, C2DExtentsI& extents) const
    {
        topLeft = C2DVectorI(RegionMapBox_TLX, RegionMapBox_TLY);
        extents = C2DExtentsI(RegionMapBox_BRX - RegionMapBox_TLX, RegionMapBox_BRY - RegionMapBox_TLY);
    }
    void InitialiseRegionBox();
};
void CRegionMinimap_InitialiseRegionBox::InitialiseRegionBox()
{
    C2DVectorI topLeft;
    C2DExtentsI extents;
    GetRegionMapExtents(topLeft, extents);
    long brx, bry;
    if (extents.Width == extents.Height) { brx = topLeft.X + extents.Width; bry = topLeft.Y + extents.Height; }
    else if (extents.Width > extents.Height) { brx = topLeft.X + extents.Width; bry = topLeft.Y + extents.Width; }
    else { brx = topLeft.X + extents.Height; bry = topLeft.Y + extents.Height; }
    RegionMapSquare_TLX = topLeft.X;
    RegionMapSquare_TLY = topLeft.Y;
    RegionMapSquare_BRX = brx;
    RegionMapSquare_BRY = bry;
}
