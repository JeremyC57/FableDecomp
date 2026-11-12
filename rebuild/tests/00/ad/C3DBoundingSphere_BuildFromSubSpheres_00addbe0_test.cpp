// Behaviour test for C3DBoundingSphere::BuildFromSubSpheres (retail 0x00ADDBE0): the same logic through a copy of the model.
#include <cstdio>
#include <math.h>

struct TVector { float X, Y, Z; };
struct TSphere {
    TVector Center;   // +0x00
    float   Radius;   // +0x0c
    float GetRadius() const { return Radius; }
    void BuildFromSubSpheres(const TSphere* spheres, long count);
};

void TSphere::BuildFromSubSpheres(const TSphere* spheres, long count)
{
    float minX = 1e30f, minY = 1e30f, minZ = 1e30f;
    float maxX = -1e30f, maxY = -1e30f, maxZ = -1e30f;
    long i;
    for (i = 0; i < count; ++i) {
        if (minX > spheres[i].Center.X - spheres[i].GetRadius()) minX = spheres[i].Center.X - spheres[i].GetRadius();
        if (minY > spheres[i].Center.Y - spheres[i].GetRadius()) minY = spheres[i].Center.Y - spheres[i].GetRadius();
        if (minZ > spheres[i].Center.Z - spheres[i].GetRadius()) minZ = spheres[i].Center.Z - spheres[i].GetRadius();
        if (maxX < spheres[i].Center.X + spheres[i].GetRadius()) maxX = spheres[i].Center.X + spheres[i].GetRadius();
        if (maxY < spheres[i].Center.Y + spheres[i].GetRadius()) maxY = spheres[i].Center.Y + spheres[i].GetRadius();
        if (maxZ < spheres[i].Center.Z + spheres[i].GetRadius()) maxZ = spheres[i].Center.Z + spheres[i].GetRadius();
    }
    Radius = 0.0f;
    float cx = (maxX + minX) * 0.5f, cy = (maxY + minY) * 0.5f, cz = (maxZ + minZ) * 0.5f;
    Center.X = cx;
    Center.Y = cy;
    Center.Z = cz;
    for (i = 0; i < count; ++i) {
        float dx = spheres[i].Center.X - cx;
        float dy = spheres[i].Center.Y - cy;
        float dz = spheres[i].Center.Z - cz;
        float reach = (float)sqrt(dy * dy + (dx * dx + dz * dz)) + spheres[i].Radius;
        if (reach > Radius) Radius = reach;
    }
}

static bool Near(float a, float b) { float d = a - b; if (d < 0.0f) d = -d; return d < 1e-4f; }

int main()
{
    bool ok = sizeof(TSphere) == 16;
    TSphere out;

    // Two unit spheres on the X axis at -2 and +2: box X [-3,3], Y/Z [-1,1] -> centre origin, radius 2 + 1.
    TSphere pair[2] = { { { -2.0f, 0.0f, 0.0f }, 1.0f }, { { 2.0f, 0.0f, 0.0f }, 1.0f } };
    out.Center.X = out.Center.Y = out.Center.Z = 9.0f; out.Radius = 9.0f;
    out.BuildFromSubSpheres(pair, 2);
    ok = ok && Near(out.Center.X, 0.0f) && Near(out.Center.Y, 0.0f) && Near(out.Center.Z, 0.0f) && Near(out.Radius, 3.0f);

    // One sphere: the result is that sphere.
    TSphere one[1] = { { { 4.0f, -5.0f, 6.0f }, 2.5f } };
    out.BuildFromSubSpheres(one, 1);
    ok = ok && Near(out.Center.X, 4.0f) && Near(out.Center.Y, -5.0f) && Near(out.Center.Z, 6.0f) && Near(out.Radius, 2.5f);

    // Unequal radii, all three axes: box X [-1,7], Y [-3,4], Z [-1,12] -> centre (3, 0.5, 5.5);
    // reach = max(|(0,0,0)-c| + 1, |(5,2,10)-c| + 2, |(1,-2,3)-c| + 1).
    TSphere mix[3] = { { { 0.0f, 0.0f, 0.0f }, 1.0f }, { { 5.0f, 2.0f, 10.0f }, 2.0f }, { { 1.0f, -2.0f, 3.0f }, 1.0f } };
    out.BuildFromSubSpheres(mix, 3);
    float r0 = (float)sqrt(3.0f * 3.0f + 0.5f * 0.5f + 5.5f * 5.5f) + 1.0f;
    float r1 = (float)sqrt(2.0f * 2.0f + 1.5f * 1.5f + 4.5f * 4.5f) + 2.0f;
    float r2 = (float)sqrt(2.0f * 2.0f + 2.5f * 2.5f + 2.5f * 2.5f) + 1.0f;
    float want = r0; if (r1 > want) want = r1; if (r2 > want) want = r2;
    ok = ok && Near(out.Center.X, 3.0f) && Near(out.Center.Y, 0.5f) && Near(out.Center.Z, 5.5f) && Near(out.Radius, want);
    // every sub-sphere is inside the result
    for (int k = 0; k < 3; ++k) {
        float dx = mix[k].Center.X - out.Center.X, dy = mix[k].Center.Y - out.Center.Y, dz = mix[k].Center.Z - out.Center.Z;
        ok = ok && (float)sqrt(dx * dx + dy * dy + dz * dz) + mix[k].Radius <= out.Radius + 1e-4f;
    }

    // No spheres: the loops are skipped, the bounds stay at +/-1e30 and cancel -> centre 0, radius 0.
    out.Center.X = out.Center.Y = out.Center.Z = 9.0f; out.Radius = 9.0f;
    out.BuildFromSubSpheres(pair, 0);
    ok = ok && out.Center.X == 0.0f && out.Center.Y == 0.0f && out.Center.Z == 0.0f && out.Radius == 0.0f;

    if (!ok) { std::printf("00addbe0_TEST FAIL\n"); return 1; }
    std::printf("00addbe0_TEST PASS\n");
    return 0;
}