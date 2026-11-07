// Behaviour test for 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData: the per-frame keyframes give the position and
// scaling factors (largest absolute component / 32767); each track is quantised (rotations kept as quaternions, positions and
// scalings as three shorts) with a palette of first-occurrence indices. One unique value gives mode 0 (identity / zero / unit, track
// dropped) or mode 1 (the single value); otherwise the sequence is animated and uses mode 3 (unique values + byte palette) when that is
// smaller and the unique count fits a byte, else mode 2 (every frame, palette dropped). The identity flag is set when all three modes
// are 0.
#include <cstdio>
#include <cmath>
#include <vector>

struct Q { float x, y, z, w; bool operator==(const Q& o) const { return x == o.x && y == o.y && z == o.z && w == o.w; } };
struct W { short x, y, z; bool operator==(const W& o) const { return x == o.x && y == o.y && z == o.z; } };
struct Out { int rmode, pmode, smode; bool animated, identity; size_t rtrack, ptrack, rpal, ppal; float pfactor; };

template <class T> static void Palette(const std::vector<T>& in, std::vector<T>& unique, std::vector<unsigned char>& pal)
{
    for (size_t i = 0; i < in.size(); ++i) {
        size_t j = 0;
        while (j < unique.size() && !(unique[j] == in[i])) ++j;
        if (j == unique.size()) unique.push_back(in[i]);
        pal.push_back((unsigned char)j);
    }
}

static Out Build(const std::vector<Q>& rot, const std::vector<float>& px)
{
    Out o = {};
    size_t n = rot.size();
    float pf = 0.0f;
    for (size_t i = 0; i < n; ++i) pf = std::fabs(px[i]) > pf ? std::fabs(px[i]) : pf;
    pf *= 1.0f / 32767.0f;
    o.pfactor = pf;
    float ps = pf > 0.0f ? 1.0f / pf : 0.0f;
    std::vector<Q> ur; std::vector<unsigned char> rp; Palette(rot, ur, rp);
    std::vector<Q> rt = rot;
    if (ur.size() == 1) {
        if (ur[0].x == 0 && ur[0].y == 0 && ur[0].z == 0 && ur[0].w == 1) { o.rmode = 0; rt.clear(); } else { o.rmode = 1; rt = ur; }
        rp.clear();
    } else {
        o.animated = true;
        if (ur.size() * 16 + rp.size() < rt.size() * 16 && ur.size() <= 255) { o.rmode = 3; rt = ur; } else { o.rmode = 2; rp.clear(); }
    }
    std::vector<W> pw; for (size_t i = 0; i < n; ++i) { W w = { (short)(px[i] * ps), 0, 0 }; pw.push_back(w); }
    std::vector<W> up; std::vector<unsigned char> pp; Palette(pw, up, pp);
    if (up.size() == 1) {
        if (px[0] * px[0] <= 1e-4f * 1e-4f) { o.pmode = 0; pw.clear(); } else { o.pmode = 1; pw = up; }
        pp.clear();
    } else {
        o.animated = true;
        if (up.size() * 6 + pp.size() < pw.size() * 6 && up.size() <= 255) { o.pmode = 3; pw = up; } else { o.pmode = 2; pp.clear(); }
    }
    o.smode = 0;
    o.rtrack = rt.size(); o.ptrack = pw.size(); o.rpal = rp.size(); o.ppal = pp.size();
    o.identity = o.rmode == 0 && o.pmode == 0 && o.smode == 0;
    return o;
}

int main()
{
    Q id = { 0, 0, 0, 1 }, q2 = { 0.5f, 0.5f, 0.5f, 0.5f };
    std::vector<Q> r1(4, id); std::vector<float> p0(4, 0.0f);
    Out a = Build(r1, p0);
    bool ok = a.rmode == 0 && a.pmode == 0 && a.identity && !a.animated && a.rtrack == 0 && a.ptrack == 0;
    std::vector<Q> r2(4, q2); std::vector<float> p1(4, 2.0f);
    Out b = Build(r2, p1);
    ok = ok && b.rmode == 1 && b.rtrack == 1 && b.pmode == 1 && b.ptrack == 1 && !b.identity && std::fabs(b.pfactor - 2.0f / 32767.0f) < 1e-9f;
    std::vector<Q> r3; std::vector<float> p3;
    for (int i = 0; i < 40; ++i) { r3.push_back((i & 1) ? id : q2); p3.push_back((float)(i % 3)); }
    Out c = Build(r3, p3);
    ok = ok && c.animated && c.rmode == 3 && c.rtrack == 2 && c.rpal == 40 && c.pmode == 3 && c.ptrack == 3 && c.ppal == 40;
    std::vector<Q> r4; std::vector<float> p4;
    for (int i = 0; i < 3; ++i) { Q q = { (float)i, 0, 0, 1 }; r4.push_back(q); p4.push_back((float)i); }
    Out d = Build(r4, p4);
    ok = ok && d.rmode == 2 && d.rpal == 0 && d.rtrack == 3 && d.pmode == 2 && d.ppal == 0;
    std::printf(ok ? "00a4efc0_TEST PASS\n" : "00a4efc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
