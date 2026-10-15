// Behaviour test for CRegion::GetWorldMapOffset @ 0x006bbf40: the def's offset, or (0, 0) without a def.
#include <cstdio>

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

struct CRegionDef {
    C2DVector WorldMapOffset;
    C2DVector NameGraphicOffset;
    CRegionDef() : WorldMapOffset(1.0f, 2.0f), NameGraphicOffset(-3.0f, 4.5f) {}
};

struct CRegion {
    const CRegionDef* PDef;
    C2DVector GetWorldMapOffset() const
    {
        if (PDef)
            return PDef->WorldMapOffset;
        return C2DVector(0.0f, 0.0f);
    }
};

int main()
{
    CRegionDef def;
    CRegion r; r.PDef = &def;
    C2DVector v = r.GetWorldMapOffset();
    if (v.X != def.WorldMapOffset.X || v.Y != def.WorldMapOffset.Y) { std::printf("006bbf40_TEST FAIL\n"); return 1; }
    r.PDef = 0;
    C2DVector z = r.GetWorldMapOffset();
    if (z.X != 0.0f || z.Y != 0.0f) { std::printf("006bbf40_TEST FAIL\n"); return 1; }
    std::printf("006bbf40_TEST PASS\n");
    return 0;
}