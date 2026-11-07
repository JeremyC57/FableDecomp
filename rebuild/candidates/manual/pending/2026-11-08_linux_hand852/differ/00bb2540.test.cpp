// Behaviour test for CEngineClothRenderer::RenderPolygons (0x00BB2540): the index list holds every triangle and two triangles per
// quad, offset by the vertex count for the back copy of a double-sided cloth, with the first two indices swapped on that copy
// (reversed winding); the vertex normals of the back copy are negated.
#include <cstdio>

int main()
{
    unsigned short tri[3] = { 0, 1, 2 };
    unsigned short quad[4] = { 1, 2, 3, 4 };
    unsigned short out[18];
    unsigned short* p = out;
    unsigned short base = 0;
    unsigned char first = 0, second = 1;
    const unsigned long vertex_count = 5;
    for (int side = 0; side < 2; ++side) {
        p[first] = tri[0] + base; p[second] = tri[1] + base; p[2] = tri[2] + base; p += 3;
        p[first] = quad[0] + base; p[second] = quad[1] + base; p[2] = quad[2] + base; p += 3;
        p[first] = quad[1] + base; p[second] = quad[3] + base; p[2] = quad[2] + base; p += 3;
        base = (unsigned short)(base + vertex_count);
        unsigned char swap = first; first = second; second = swap;
    }
    bool ok = out[0] == 0 && out[1] == 1 && out[2] == 2 && out[6] == 2 && out[7] == 4 && out[8] == 3;
    ok = ok && out[9] == 6 && out[10] == 5 && out[11] == 7 && out[15] == 9 && out[16] == 7 && out[17] == 8;
    float nz = 0.5f; float back = -nz;
    ok = ok && back == -0.5f;
    if (!ok) { std::printf("00bb2540_TEST FAIL\n"); return 1; }
    std::printf("00bb2540_TEST PASS\n");
    return 0;
}
