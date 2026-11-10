// CWorldMap::GetThemeFaceNormalAt(const C3DVector&) const -- the face normal of the theme height surface at a position: the cell's four corner heights, the triangle picked by the cell's parity and the position within it, and the normalised cross product of the two edge slopes
// Leaf: ?GetThemeFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063AB90, 582 bytes, thiscall, `ret 8` (C3DVector by hidden pointer), frame 0x28. The catalog's
//   608-byte extent runs on into the next function (0x0063ADE0 `fld [0122DEDCh] ; ret 4`): SPLITS.tsv.
// Shape: the first lookup's C2DCoordI is a scoped local filled member by member (X before the second conversion); the
//   other three are temporaries. Frame and x87 order: C2DVector copy of the position, per-branch fractional expressions.
// Identity: Ego_r.pdb / FableWin.pdb ?GetThemeFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z.
// Callees: _ftol 0x00BFEA70 (x2), CWorldMap::GetThemeSizeZAtBlock(const C2DCoordI&) const 0x0063A7E0 (x4), CWorldMap virtual slot 13
//   GetDimensions (+0x34) for the clamps; the expanded C3DVector cross product and Normalise (GFFastOneOverSqrt table 0x013CE558).
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_THEMEFACENORMAL
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CWorldMap.h"

C3DVector CWorldMap::GetThemeFaceNormalAt(const C3DVector& pos) const
{
    // The cell's own corner: its X is stored as soon as the first conversion is done (0x0063ABBD `mov [esp+1Ch],ebx`, before the
    // second _ftol), and the scoped coordinate's slot is reused by the next lookup's temporary (frame 0x28).
    C2DVector p(pos.X, pos.Y);
    long ix = (long)p.X;
    long iy;
    float h00;
    {
        C2DCoordI cell;
        cell.X = ix;
        iy = (long)p.Y;
        cell.Y = iy;
        h00 = GetThemeSizeZAtBlock(cell);
    }
    float h10 = GetThemeSizeZAtBlock(C2DCoordI(ix + 1 < GetDimensions().BRX - 1 ? ix + 1 : GetDimensions().BRX - 1, iy));
    float h01 = GetThemeSizeZAtBlock(C2DCoordI(ix, iy + 1 < GetDimensions().BRY - 1 ? iy + 1 : GetDimensions().BRY - 1));
    float h11 = GetThemeSizeZAtBlock(C2DCoordI(ix + 1 < GetDimensions().BRX - 1 ? ix + 1 : GetDimensions().BRX - 1,
                                               iy + 1 < GetDimensions().BRY - 1 ? iy + 1 : GetDimensions().BRY - 1));
    float dx, dy;
    if ((ix + iy) % 2 == 1)
    {
        if ((p.X - ix) + (p.Y - iy) > 1.0f)
        {
            dx = h11 - h01;
            dy = h11 - h10;
        }
        else
        {
            dx = h10 - h00;
            dy = h01 - h00;
        }
    }
    else if ((p.Y - iy) > (p.X - ix))
    {
        dx = h11 - h01;
        dy = h01 - h00;
    }
    else
    {
        dx = h10 - h00;
        dy = h11 - h10;
    }
    C3DVector normal = C3DVector(1.0f, 0.0f, dx) ^ C3DVector(0.0f, 1.0f, dy);
    normal.Normalise();
    return normal;
}