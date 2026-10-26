#include <cstdio>
// Behaviour test for CThingObject::UseHighDetailQuadTree (0x00703010): bit 3 of the def flag byte.
class CThingObjectDef { public:
    unsigned char IsFragile : 1; unsigned char IsClothing : 1; unsigned char IsSelfIlluminating : 1;
    unsigned char UseHighDetailQuadTree : 1; unsigned char Electrocutable : 1;
};
class CThingObject { public:
    const CThingObjectDef* PDef;
    unsigned char UseHighDetailQuadTree() const { return PDef->UseHighDetailQuadTree; }
};
int main()
{
    CThingObjectDef d; d.IsFragile = 1; d.IsClothing = 1; d.IsSelfIlluminating = 1; d.UseHighDetailQuadTree = 0; d.Electrocutable = 1;
    CThingObject o; o.PDef = &d;
    int fails = 0;
    if (o.UseHighDetailQuadTree() != 0) ++fails;
    d.UseHighDetailQuadTree = 1; d.IsFragile = 0; d.Electrocutable = 0;
    if (o.UseHighDetailQuadTree() != 1) ++fails;
    if (fails) { std::printf("00703010_TEST FAIL\n"); return 1; }
    std::printf("00703010_TEST PASS\n");
    return 0;
}