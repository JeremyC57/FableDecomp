// Behaviour test for row 0x00A6C640 CPhysicsMesh::CalculateBoundingInfo (links the row's object). The test models only the bytes the
// row touches: TotalPoints (CArray<C3DVector>: begin / end at +0x3C / +0x40), the bounding sphere (+0x74 centre, +0x80 radius), the
// bounding box (+0x84 min, +0x90 max) and the bounding circle (+0x9C centre, +0xA4 radius). Each axis' minimum and maximum come from a
// different point, the first point is extreme on no axis (so dropping any of the six compares changes a bound) and the last point
// carries two extremes (so a loop that stops one early fails). A second mesh with one point checks the degenerate box.
#include <stdio.h>
#include <math.h>

class C3DVector { public: float X, Y, Z; };

class CPhysicsMesh {
public:
    char Pad0[0x3C];
    C3DVector* PointsBegin;          // +0x3C
    C3DVector* PointsEnd;            // +0x40
    C3DVector* PointsCapacity;       // +0x44
    char Pad1[0x74 - 0x48];
    float SphereCentre[3];           // +0x74
    float SphereRadius;              // +0x80
    float BoxMin[3];                 // +0x84
    float BoxMax[3];                 // +0x90
    float CircleCentre[2];           // +0x9C
    float CircleRadius;              // +0xA4
    char Pad2[0x10];
private:
    void CalculateBoundingInfo();
    friend int main();
};

static int g_fail = 0;
static void check(const char* what, float got, float want)
{
    if (fabs(got - want) > 1e-4f * (1.0f + fabs(want))) { printf("FAIL %s: got %f want %f\n", what, got, want); g_fail = 1; }
}

int main()
{
    C3DVector points[4] = { { 1.0f, 2.0f, 3.0f }, { -4.0f, 5.0f, 0.5f }, { 6.0f, -1.0f, 7.0f }, { 2.0f, 9.0f, -2.0f } };
    CPhysicsMesh mesh;
    for (unsigned i = 0; i < sizeof(mesh); ++i) ((char*)&mesh)[i] = 0;
    mesh.PointsBegin = points; mesh.PointsEnd = points + 4; mesh.PointsCapacity = points + 4;
    mesh.CalculateBoundingInfo();
    // min (-4, -1, -2), max (6, 9, 7): centre (1, 4, 2.5), half diagonal (5, 5, 4.5)
    check("box min x", mesh.BoxMin[0], -4.0f); check("box min y", mesh.BoxMin[1], -1.0f); check("box min z", mesh.BoxMin[2], -2.0f);
    check("box max x", mesh.BoxMax[0], 6.0f); check("box max y", mesh.BoxMax[1], 9.0f); check("box max z", mesh.BoxMax[2], 7.0f);
    check("sphere centre x", mesh.SphereCentre[0], 1.0f); check("sphere centre y", mesh.SphereCentre[1], 4.0f);
    check("sphere centre z", mesh.SphereCentre[2], 2.5f);
    check("sphere radius", mesh.SphereRadius, (float)sqrt(5.0 * 5.0 + 5.0 * 5.0 + 4.5 * 4.5));
    check("circle centre x", mesh.CircleCentre[0], 1.0f); check("circle centre y", mesh.CircleCentre[1], 4.0f);
    check("circle radius", mesh.CircleRadius, (float)sqrt(5.0 * 5.0 + 5.0 * 5.0));
    if (mesh.Pad1[0] != 0 || mesh.Pad2[0] != 0) { printf("FAIL stray store\n"); g_fail = 1; }

    C3DVector single[1] = { { 3.0f, -2.0f, 8.0f } };
    CPhysicsMesh one;
    for (unsigned j = 0; j < sizeof(one); ++j) ((char*)&one)[j] = 0;
    one.PointsBegin = single; one.PointsEnd = single + 1; one.PointsCapacity = single + 1;
    one.CalculateBoundingInfo();
    check("one min x", one.BoxMin[0], 3.0f); check("one max z", one.BoxMax[2], 8.0f);
    check("one centre y", one.SphereCentre[1], -2.0f); check("one radius", one.SphereRadius, 0.0f);
    check("one circle radius", one.CircleRadius, 0.0f);

    if (!g_fail) printf("00a6c640_TEST PASS\n");
    return g_fail;
}