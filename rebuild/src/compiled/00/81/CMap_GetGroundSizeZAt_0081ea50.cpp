// Leaf: ?GetGroundSizeZAt@CMap@@IBEMABVC3DVector@@@Z
// Flags:
// Retail-asm-helper: GFFastFloatToLong
// Retail-asm: 0x0081EA76 `fistp dword ptr [edx]`
// Retail-asm: 0x0081EA9E `fistp dword ptr [edx]`
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_CHEIGHTMAP_CELLINDEX
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#include "engine/C3DVector.h"
#include "engine/CMap.h"
#include "engine/CHeightMap.h"
#include "engine/GFFastFloatToLong.h"

#include "engine/GFLimit.h"

float CMap::GetGroundSizeZAt(const C3DVector& pos) const
{
    long base_x = GFFastFloatToLong(pos.X + 0.5f) - 1;
    long base_y = GFFastFloatToLong(pos.Y + 0.5f) - 1;

    GFLimit(base_x, 0, HeightMap->GetSizeXIncludingExtraCells() - 2);
    GFLimit(base_y, 0, HeightMap->GetSizeYIncludingExtraCells() - 2);
    float frac_x = pos.X - base_x;
    float frac_y = pos.Y - base_y;

    const CHeightMapCell* cell = &HeightMap->GetCellIncludingExtraCellsAt(C2DCoordI(base_x, base_y));

    float cell_bl_height = cell->Height;
    ++cell;
    float cell_br_height = cell->Height;
    cell -= HeightMap->GetSizeXIncludingExtraCells();
    float cell_tr_height = cell->Height;
    --cell;
    float cell_tl_height = cell->Height;

    float height;
    if ((base_x + base_y) & 1)
    {
        if (frac_x + frac_y > 1.0f)
            height = (cell_br_height - cell_tr_height) * (1.0f - frac_y) + (cell_tl_height - cell_tr_height) * (1.0f - frac_x) + cell_tr_height;
        else
            height = (cell_tl_height - cell_bl_height) * frac_y + (cell_br_height - cell_bl_height) * frac_x + cell_bl_height;
    }
    else
    {
        if (frac_x > frac_y)
            height = (cell_tr_height - cell_br_height) * frac_y + (cell_br_height - cell_bl_height) * frac_x + cell_bl_height;
        else
            height = (cell_tr_height - cell_tl_height) * frac_x + (cell_tl_height - cell_bl_height) * frac_y + cell_bl_height;
    }
    return height;
}