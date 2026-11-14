// Behaviour test for 0x00AA0FA0 C3DAnimationManager::CalcBoneTransformation: the bone's chain is the bone then each parent up to the
// root; only chain bones are animated; each combination blend uses its own time and weight, a weight of exactly 1 replaces (partial) or adds (relative) fully, otherwise blends by the weight; partial animations skip masked-off bones; with a
// skeletal morph each chain position is scaled by its parent's bone scaling and the bone's own scaling is returned (else 1,1,1); the chain's
// keyframe matrices are concatenated bone first: M = K[0] * K[1] * ... * K[n-1] (row-vector convention, translation in row 4).
#include <cstdio>
#include <cmath>
#include <vector>
struct M { float m[4][4]; };
static M Mul(const M& a, const M& b) { M r; for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) { r.m[i][j] = 0; for (int k = 0; k < 4; ++k) r.m[i][j] += a.m[i][k] * b.m[k][j]; } return r; }
static M Key(float angle, float tx, float ty, float tz) {   // rotation about Z then translation (row-vector form)
    M r = { { { std::cos(angle), std::sin(angle), 0, 0 }, { -std::sin(angle), std::cos(angle), 0, 0 }, { 0, 0, 1, 0 }, { tx, ty, tz, 1 } } }; return r; }
struct KF { float angle, x, y, z; };
struct Anim { bool relative, partial; float startT, endT, startW, endW; std::vector<bool> mask; float add; };
static void Calc(const std::vector<long>& parents, const std::vector<KF>& base, const std::vector<Anim>& blends, float blend, bool morph,
                 const std::vector<float>& boneScale, unsigned long bone, M& out, float scale[3], std::vector<unsigned long>& chain, float& lastTime) {
    chain.clear(); chain.push_back(bone);
    for (long p = parents[bone]; p >= 0; p = parents[p]) chain.push_back((unsigned long)p);
    std::vector<KF> k; for (size_t j = 0; j < chain.size(); ++j) k.push_back(base[chain[j]]);
    for (size_t i = 0; i < blends.size(); ++i) {
        const Anim& a = blends[i];
        lastTime = a.startT;
        float w = a.startW;
        bool full = w == 1.0f;
        for (size_t j = 0; j < chain.size(); ++j) {
            if (a.relative) k[j].angle += full ? a.add : a.add * w;
            else if (a.partial && a.mask[chain[j]]) k[j].angle = full ? a.add : k[j].angle + (a.add - k[j].angle) * w;
        }
    }
    if (morph) {
        for (size_t j = 1; j < chain.size(); ++j) { float s = boneScale[chain[j]]; k[j - 1].x *= s; k[j - 1].y *= s; k[j - 1].z *= s; }
        scale[0] = scale[1] = scale[2] = boneScale[chain[0]];
    } else scale[0] = scale[1] = scale[2] = 1.0f;
    out = Key(k[0].angle, k[0].x, k[0].y, k[0].z);
    for (size_t j = 1; j < chain.size(); ++j) out = Mul(out, Key(k[j].angle, k[j].x, k[j].y, k[j].z));
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main() {
    std::vector<long> parents; parents.push_back(-1); parents.push_back(0); parents.push_back(1); parents.push_back(0);
    std::vector<KF> base; KF b0 = { 0, 0, 0, 0 }, b1 = { 0, 1, 0, 0 }, b2 = { 0, 2, 0, 0 }, b3 = { 0, 0, 5, 0 };
    base.push_back(b0); base.push_back(b1); base.push_back(b2); base.push_back(b3);
    std::vector<float> scales; scales.push_back(2.0f); scales.push_back(3.0f); scales.push_back(4.0f); scales.push_back(5.0f);
    std::vector<Anim> none; std::vector<unsigned long> chain; M out; float sc[3]; float t = 0;
    Calc(parents, base, none, 0.5f, false, scales, 2, out, sc, chain, t);
    check(chain.size() == 3 && chain[0] == 2 && chain[1] == 1 && chain[2] == 0, "chain bone then parents");
    check(Near(out.m[3][0], 3.0f) && Near(sc[0], 1.0f), "translations concatenate, no morph scale 1");
    Calc(parents, base, none, 0.5f, true, scales, 2, out, sc, chain, t);
    check(Near(sc[0], 4.0f) && Near(out.m[3][0], 2.0f * 3.0f + 1.0f * 2.0f), "morph: positions scaled by the parent's scale, own scale returned");
    std::vector<Anim> bl; std::vector<bool> mask(4, true); mask[1] = false;
    Anim rel = { true, false, 0.0f, 1.0f, 1.0f, 1.0f, mask, 0.5f }; bl.push_back(rel);
    Calc(parents, base, bl, 0.25f, false, scales, 2, out, sc, chain, t);
    check(Near(t, 0.0f), "time taken from the state");
    float a = 1.5f; // three chain bones each +0.5 rad
    check(Near(out.m[0][0], std::cos(a)) && Near(out.m[0][1], std::sin(a)), "full-weight relative rotation on every chain bone");
    bl[0].startW = 0.5f; bl[0].endW = 0.5f;
    Calc(parents, base, bl, 0.5f, false, scales, 2, out, sc, chain, t);
    check(Near(out.m[0][0], std::cos(0.75f)), "half weight relative");
    Anim par = { false, true, 0.0f, 0.0f, 1.0f, 1.0f, mask, 1.0f }; bl[0] = par;
    Calc(parents, base, bl, 0.5f, false, scales, 2, out, sc, chain, t);
    check(Near(out.m[0][0], std::cos(2.0f)), "partial replaces only unmasked chain bones (bone 1 masked off)");
    Calc(parents, base, bl, 0.5f, false, scales, 3, out, sc, chain, t);
    check(chain.size() == 2 && Near(out.m[3][1], 5.0f * std::cos(1.0f) + 0.0f) && Near(out.m[3][0], -5.0f * std::sin(1.0f)), "other branch: bone 3 then root, root rotation applied after");
    if (!fails) std::printf("00aa0fa0_TEST PASS\n");
    return fails;
}
