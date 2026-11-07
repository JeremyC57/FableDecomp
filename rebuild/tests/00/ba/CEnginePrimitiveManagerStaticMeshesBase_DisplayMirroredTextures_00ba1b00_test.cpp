// Behaviour test for 0x00BA1B00 CEnginePrimitiveManagerStaticMeshesBase::DisplayMirroredTextures: nothing is drawn for an empty
// mirrored list, a disabled highlight switch or render pass 1; otherwise the 16-bit index buffer holds the low words of the list,
// two draws follow (texture factor red, then green, alpha = sub-primitive alpha / 255; pixel-shader constant 2 when pixel shaders
// are on, else the packed TEXTUREFACTOR colour), each with index_count / 2 vertices and index_count / 6 primitives, then a flush.
#include <cstdio>
#include <cmath>
#include <vector>

struct Draw { float r, g, b, a; unsigned long tfactor; bool ps; unsigned long verts, prims; };
struct Renderer {
    bool highlight, pixel_shaders;
    std::vector<unsigned short> ib;
    std::vector<Draw> draws;
    int flushes;
    float c[4]; unsigned long tf;
    void SetTextureFactor(float r, float g, float b, float a) {
        if (pixel_shaders) { c[0] = r; c[1] = g; c[2] = b; c[3] = a; }
        else tf = ((unsigned long)(unsigned char)(a * 255.0f) << 24) | ((unsigned long)(unsigned char)(r * 255.0f) << 16)
                | ((unsigned long)(unsigned char)(g * 255.0f) << 8) | (unsigned long)(unsigned char)(b * 255.0f);
    }
    void DisplayMirroredTextures(const std::vector<unsigned long>& list, int pass, unsigned char alpha) {
        unsigned long n = (unsigned long)list.size();
        if (n == 0 || !highlight || pass == 1) return;
        ib.resize(n);
        for (unsigned long i = 0; i < n; ++i) ib[i] = (unsigned short)list[i];
        const float k = 1.0f / 255.0f;
        for (int p = 0; p < 2; ++p) {
            SetTextureFactor(p == 0 ? 1.0f : 0.0f, p == 0 ? 0.0f : 1.0f, 0.0f, alpha * k);
            Draw d = { c[0], c[1], c[2], c[3], tf, pixel_shaders, n / 2, n / 6 };
            draws.push_back(d);
        }
        ++flushes;
    }
};

int main()
{
    std::vector<unsigned long> list;
    for (unsigned long i = 0; i < 12; ++i) list.push_back(0x10000u + i * 3);
    Renderer r = { true, false };
    r.flushes = 0; r.tf = 0;
    r.DisplayMirroredTextures(std::vector<unsigned long>(), 0, 255);
    r.DisplayMirroredTextures(list, 1, 255);
    bool ok = r.draws.empty() && r.flushes == 0;
    r.DisplayMirroredTextures(list, 0, 255);
    ok = ok && r.draws.size() == 2 && r.flushes == 1 && r.ib.size() == 12 && r.ib[5] == 15 && r.draws[0].verts == 6 && r.draws[0].prims == 2
        && r.draws[0].tfactor == 0xFFFF0000ul && r.draws[1].tfactor == 0xFF00FF00ul;
    Renderer s = { true, true };
    s.flushes = 0; s.tf = 0;
    s.DisplayMirroredTextures(list, 3, 51);
    ok = ok && s.draws.size() == 2 && s.draws[0].r == 1.0f && s.draws[1].g == 1.0f && std::fabs(s.draws[1].a - 0.2f) < 1e-4f;
    Renderer t = { false, true };
    t.flushes = 0;
    t.DisplayMirroredTextures(list, 0, 255);
    ok = ok && t.draws.empty();
    std::printf(ok ? "00ba1b00_TEST PASS\n" : "00ba1b00_TEST FAIL\n");
    return ok ? 0 : 1;
}