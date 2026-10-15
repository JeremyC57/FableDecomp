// Behaviour test for CRegion::GetNameGraphicOffset @ 0x006bbf70: the def's offset, or (0, 0) without a def.
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
    C2DVector GetNameGraphicOffset() const
    {
        if (PDef)
            return PDef->NameGraphicOffset;
        return C2DVector(0.0f, 0.0f);
    }
};

int main()
{
    CRegionDef def;
    CRegion r; r.PDef = &def;
    C2DVector v = r.GetNameGraphicOffset();
    if (v.X != def.NameGraphicOffset.X || v.Y != def.NameGraphicOffset.Y) { std::printf("006bbf70_TEST FAIL\n"); return 1; }
    r.PDef = 0;
    C2DVector z = r.GetNameGraphicOffset();
    if (z.X != 0.0f || z.Y != 0.0f) { std::printf("006bbf70_TEST FAIL\n"); return 1; }
    std::printf("006bbf70_TEST PASS\n");
    return 0;
}