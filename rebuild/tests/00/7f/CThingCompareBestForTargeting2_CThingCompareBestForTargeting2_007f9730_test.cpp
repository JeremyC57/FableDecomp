#include <cstdio>
// behaviour test for the CThingCompare_BestForTargeting2 copy constructor (0x007F9730)
struct CThing { int dummy; };
struct CVectorCompare_BestForTargeting {
    long AnchorPos[3];
    long PreferredLine[3];
    long AngleCompare;
    long SqrDistCompare;
    long OverrideShortDistanceFactor;
    long OverrideShortDistanceFalloff;
    bool CompareInXYPlane;
};
struct CThingCompare_BestForTargeting2 {
    CThing* Hero;
    CVectorCompare_BestForTargeting VectorCompare;
    long EnemyPreferenceFactor;
    CThingCompare_BestForTargeting2() { }
    CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other);
};
CThingCompare_BestForTargeting2::CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other)
    : Hero(other.Hero)
{
    VectorCompare.AnchorPos[0] = other.VectorCompare.AnchorPos[0];
    VectorCompare.AnchorPos[1] = other.VectorCompare.AnchorPos[1];
    VectorCompare.AnchorPos[2] = other.VectorCompare.AnchorPos[2];
    VectorCompare.PreferredLine[0] = other.VectorCompare.PreferredLine[0];
    VectorCompare.PreferredLine[1] = other.VectorCompare.PreferredLine[1];
    VectorCompare.PreferredLine[2] = other.VectorCompare.PreferredLine[2];
    VectorCompare.AngleCompare = other.VectorCompare.AngleCompare;
    VectorCompare.SqrDistCompare = other.VectorCompare.SqrDistCompare;
    VectorCompare.OverrideShortDistanceFactor = other.VectorCompare.OverrideShortDistanceFactor;
    VectorCompare.OverrideShortDistanceFalloff = other.VectorCompare.OverrideShortDistanceFalloff;
    VectorCompare.CompareInXYPlane = other.VectorCompare.CompareInXYPlane;
    EnemyPreferenceFactor = other.EnemyPreferenceFactor;
}
int main()
{
    CThing hero;
    CThingCompare_BestForTargeting2 a;
    a.Hero = &hero;
    for (int i = 0; i < 3; ++i) { a.VectorCompare.AnchorPos[i] = 10 + i; a.VectorCompare.PreferredLine[i] = 20 + i; }
    a.VectorCompare.AngleCompare = 30; a.VectorCompare.SqrDistCompare = 31;
    a.VectorCompare.OverrideShortDistanceFactor = 32; a.VectorCompare.OverrideShortDistanceFalloff = 33;
    a.VectorCompare.CompareInXYPlane = true; a.EnemyPreferenceFactor = 34;
    CThingCompare_BestForTargeting2 b(a);
    if (b.Hero != &hero || b.VectorCompare.AnchorPos[2] != 12 || b.VectorCompare.PreferredLine[0] != 20 || b.VectorCompare.AngleCompare != 30
        || b.VectorCompare.SqrDistCompare != 31 || b.VectorCompare.OverrideShortDistanceFactor != 32 || b.VectorCompare.OverrideShortDistanceFalloff != 33
        || !b.VectorCompare.CompareInXYPlane || b.EnemyPreferenceFactor != 34) { std::printf("007f9730_TEST FAIL\n"); return 1; }
    std::printf("007f9730_TEST PASS\n");
    return 0;
}