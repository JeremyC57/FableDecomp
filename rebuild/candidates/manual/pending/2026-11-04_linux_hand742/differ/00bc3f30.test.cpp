// Behaviour test for CEnginePrimitiveManagerParticleLineGroup::RenderSubPrimitive (0x00BC3F30): a model of the line batching --
// three constants per line (start + width, end + 1, colour / 255 with doubled alpha and RGB premultiplied by it when asked), a
// draw of 40 vertices / 20 triangles at every 30 constants, and a tail draw of count/3 lines (4 vertices, 2 triangles each).
#include <cstdio>
#include <cmath>
#include <vector>
struct Line { float s[3], e[3], width; unsigned char b, g, r, a; };
struct Draw { unsigned long constants, vertices, tris; };
static std::vector<Draw> Batch(const std::vector<Line>& lines, bool premultiply, float out[30][4])
{
    std::vector<Draw> draws; unsigned long n = 0;
    for (size_t i = 0; i < lines.size(); ++i) {
        const Line& l = lines[i];
        float R = l.r / 255.0f, G = l.g / 255.0f, B = l.b / 255.0f, A = l.a / 255.0f; A += A;
        if (premultiply) { R *= A; G *= A; B *= A; }
        float* c = out[n]; c[0] = l.s[0]; c[1] = l.s[1]; c[2] = l.s[2]; c[3] = l.width; ++n;
        c = out[n]; c[0] = l.e[0]; c[1] = l.e[1]; c[2] = l.e[2]; c[3] = 1.0f; ++n;
        c = out[n]; c[0] = R; c[1] = G; c[2] = B; c[3] = A; ++n;
        if (n == 30) { Draw d = { n, 40, 20 }; draws.push_back(d); n = 0; }
    }
    if (n) { unsigned long k = n / 3; Draw d = { k * 3, k * 4, k * 2 }; draws.push_back(d); }
    return draws;
}
int main()
{
    float cb[30][4]; std::vector<Line> lines;
    for (int i = 0; i < 12; ++i) { Line l = { { (float)i, 0, 0 }, { 0, (float)i, 0 }, 0.5f, 0, 0, 255, 255 }; lines.push_back(l); }
    std::vector<Draw> d = Batch(lines, true, cb);
    bool ok = d.size() == 2 && d[0].constants == 30 && d[0].vertices == 40 && d[0].tris == 20
        && d[1].constants == 6 && d[1].vertices == 8 && d[1].tris == 4
        && std::fabs(cb[0][0] - 10.0f) < 1e-6f && std::fabs(cb[0][3] - 0.5f) < 1e-6f && cb[1][3] == 1.0f
        && std::fabs(cb[2][0] - 2.0f) < 1e-6f && std::fabs(cb[2][3] - 2.0f) < 1e-6f && cb[2][1] == 0.0f;
    std::vector<Line> one(1, lines[0]); d = Batch(one, false, cb);
    ok = ok && d.size() == 1 && d[0].constants == 3 && std::fabs(cb[2][0] - 1.0f) < 1e-6f;
    printf(ok ? "00bc3f30_TEST PASS\n" : "00bc3f30_TEST FAIL\n");
    return ok ? 0 : 1;
}
