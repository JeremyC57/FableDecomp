// Behaviour test for 0x00678760 CPersistTraits<std::list<CReactionMatch*> >::TransferIn: the ".Add(type, Shape(...))"
// grammar -- shape name selects the reaction-match class and its float count; an unknown name is an error.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// returns floats expected after the shape name (opinion args counted separately), -1 for unknown
static int ShapeFloats(const char* name, int& opinions)
{
    opinions = 0;
    if (strcmp(name, "Sphere") == 0) { opinions = 0; return 6; }              // COpinionSet (5 floats) + radius
    if (strcmp(name, "MoralityRenownBlob") == 0) return 7;
    if (strcmp(name, "AsymmetricPyramid") == 0) { opinions = 2; return 6; }
    if (strcmp(name, "SecondaryAxisMRShifted") == 0) { opinions = 1; return 6; }
    if (strcmp(name, "SecondaryAxisCentredMRShifted") == 0) { opinions = 1; return 6; }
    return -1;
}
static bool Parse(const char* text, char* shape, int& count)
{
    if (strncmp(text, ".Add(", 5) != 0) return false;
    const char* p = strchr(text, ','); if (!p) return false; ++p; while (*p == ' ') ++p;
    const char* q = strchr(p, '('); if (!q) return false;
    memcpy(shape, p, q - p); shape[q - p] = 0;
    count = 0; for (const char* r = q + 1; *r && *r != ')'; ++r) if (*r == ',') ++count;
    ++count; return true;
}
int main()
{
    int ok = 1, ops, n; char shape[64];
    ok &= Parse(".Add(3, MoralityRenownBlob(1,2,3,4,5,6,7))", shape, n) && strcmp(shape, "MoralityRenownBlob") == 0 && n == 7;
    ok &= ShapeFloats(shape, ops) == 7 && ops == 0;
    ok &= Parse(".Add(1, AsymmetricPyramid(A,B,1,2,3,4,5,6))", shape, n) && n == 8 && ShapeFloats(shape, ops) + ops == 8;
    ok &= ShapeFloats("SecondaryAxisCentredMRShifted", ops) == 6 && ops == 1;
    ok &= ShapeFloats("Cube", ops) == -1;
    ok &= !Parse("Add(1, Sphere(1))", shape, n);
    if (ok) printf("00678760_TEST PASS\n"); else printf("00678760_TEST FAIL\n");
    return ok ? 0 : 1;
}
