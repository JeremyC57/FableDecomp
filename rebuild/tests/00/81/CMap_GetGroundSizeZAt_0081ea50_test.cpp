#include <assert.h>
#include <stdio.h>

// Run the selected member body on a fixture that only supplies its height map.
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_CHEIGHTMAP_CELLINDEX
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_ENGINE_CMAP_H
#define CMap GroundHeightFixture
#include "engine/CHeightMap.h"
#include "engine/C3DVector.h"

class GroundHeightFixture
{
public:
    explicit GroundHeightFixture(CHeightMap& map) : HeightMap(&map)
    {
        map.SizeX = map.SizeY = 3;
        map.SizeXIncludingExtraCells = map.SizeYIncludingExtraCells = 4;
        CHeightMapCell empty;
        empty.Clear();
        std::vector<CHeightMapCell> cells(16, empty);
        for (long y = 0; y <= 3; ++y)
            for (long x = 0; x <= 3; ++x)
                cells[(3 - y) * 4 + x].Height = (float)(2 * x + 4 * y + 5 * x * y);
        map.Cells.swap(cells);
    }

    float GetGroundSizeZAt(const C3DVector& pos) const;

private:
    CHeightMap* HeightMap;
};

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

#undef CMap

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CHeightMap::~CHeightMap() {}

int main()
{
    CHeightMap height_map;
    GroundHeightFixture map(height_map);
    assert(map.GetGroundSizeZAt(C3DVector(0.75f, 0.25f, 9.0f)) == 3.75f);
    assert(map.GetGroundSizeZAt(C3DVector(0.25f, 0.75f, 0.0f)) == 4.75f);
    assert(map.GetGroundSizeZAt(C3DVector(0.5f, 0.5f, 0.0f)) == 5.5f);

    assert(map.GetGroundSizeZAt(C3DVector(1.25f, 0.25f, 0.0f)) == 4.75f);
    assert(map.GetGroundSizeZAt(C3DVector(1.75f, 0.75f, 0.0f)) == 12.75f);
    assert(map.GetGroundSizeZAt(C3DVector(1.25f, 0.75f, 0.0f)) == 9.25f);

    assert(map.GetGroundSizeZAt(C3DVector(-2.0f, 0.25f, 0.0f)) == -13.0f);
    assert(map.GetGroundSizeZAt(C3DVector(0.25f, -2.0f, 0.0f)) == -17.5f);
    assert(map.GetGroundSizeZAt(C3DVector(4.0f, 0.25f, 0.0f)) == 12.75f);
    assert(map.GetGroundSizeZAt(C3DVector(0.25f, 4.0f, 0.0f)) == 20.25f);
    assert(map.GetGroundSizeZAt(C3DVector(0.0f, 0.0f, 0.0f)) == 0.0f);
    assert(map.GetGroundSizeZAt(C3DVector(2.0f, 0.0f, 0.0f)) == 4.0f);
    puts("0081ea50_TEST PASS");
}