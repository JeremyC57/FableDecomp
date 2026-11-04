// Behaviour test for NParticleEngine::CPSCRenderMesh::PreparePrimitives (0x00B03CA0): a standalone model of three pieces -- the colour
// table lookup (round(life * 511): entry index >> 8, blend weight & 0xFF between entries i and i+1), the cosine smooth fade
// (0.5 - 0.5 cos(pi x)) applied to the alpha before fade-in end / after fade-out begin, and the per-particle primitive id cache
// (GetPrimID: a valid id reuses SetChildPrimitive; otherwise AddChildPrimitive's id is stored by SetPrimitiveInfo, growing the lists).
#include <cstdio>
#include <cmath>
#include <vector>
static unsigned long Lerp(unsigned long a, unsigned long b, unsigned long w) {
    unsigned long r = 0;
    for (int s = 0; s < 32; s += 8) { unsigned long ca = (a >> s) & 255, cb = (b >> s) & 255; r |= ((ca * (256 - w) + cb * w) >> 8) << s; }
    return r;
}
static unsigned long Colour(const unsigned long* c, float life) {
    unsigned long pos = (unsigned long)std::floor(life * 511.0f + 0.5f);
    return Lerp(c[pos >> 8], c[(pos >> 8) + 1], pos & 0xff);
}
static float SmoothFade(float x) { return 0.5f - 0.5f * std::cos(3.14159265f * x); }
struct Particle {
    std::vector<std::vector<long> > ids;
    bool GetPrimID(long i, long j, long& id) const {
        if (ids.size() > (unsigned long)i && ids[i].size() > (unsigned long)j) { id = ids[i][j]; return id >= 0; }
        return false;
    }
    void SetPrimitiveInfo(long i, long j, long id) {
        if (ids.size() <= (unsigned long)i) ids.resize(i + 1);
        if (ids[i].size() <= (unsigned long)j) ids[i].resize(j + 1, 0);
        ids[i][j] = id;
    }
};
int main()
{
    unsigned long colours[3] = { 0x00000000, 0xFFFFFFFF, 0x00000000 };
    bool ok = Colour(colours, 0.0f) == 0 && Colour(colours, 255.0f / 511.0f) == 0xFEFEFEFE && Colour(colours, 1.0f) == 0x00000000;
    ok = ok && std::fabs(SmoothFade(0.0f)) < 1e-5f && std::fabs(SmoothFade(1.0f) - 1.0f) < 1e-5f && std::fabs(SmoothFade(0.5f) - 0.5f) < 1e-5f;
    Particle p; long id = -1; int adds = 0, sets = 0;
    for (int frame = 0; frame < 3; ++frame) {
        if (p.GetPrimID(1, 0, id)) ++sets;
        else { ++adds; p.SetPrimitiveInfo(1, 0, 42); }
    }
    ok = ok && adds == 1 && sets == 2 && p.ids.size() == 2 && p.ids[0].empty() && p.ids[1][0] == 42;
    printf(ok ? "00b03ca0_TEST PASS\n" : "00b03ca0_TEST FAIL\n");
    return ok ? 0 : 1;
}
