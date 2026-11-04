// Behaviour test for 0x0086BE40 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
static unsigned long Rand(unsigned long range, unsigned long* seed) { *seed = *seed * 69069u + 1u; return (*seed >> 8) % range; }
struct Mesh { int MeshIndex; };
struct Set { int MeshIndex; unsigned long Variant; };
static void Parts(const std::vector<Mesh>* body, unsigned long maxGroup, unsigned long& seed, std::vector<Set>& out, bool skipFirst) {
    unsigned long variant = 0;
    if (maxGroup) variant = Rand(maxGroup, &seed) + 1;
    for (long part = 0; part != 3; ++part)
        if (!body[part].empty()) {
            Set s; s.MeshIndex = body[part][Rand((unsigned long)body[part].size(), &seed)].MeshIndex; s.Variant = variant;
            if (!skipFirst || part != 0) out.push_back(s);
        }
}
int main() {
    std::vector<Mesh> body[3]; Mesh m0 = { 10 }, m2 = { 30 }; body[0].push_back(m0); body[2].push_back(m2);
    unsigned long seed = 1; std::vector<Set> out; Parts(body, 0, seed, out, false);
    bool ok = out.size() == 2 && out[0].MeshIndex == 10 && out[1].MeshIndex == 30 && out[0].Variant == 0;
    out.clear(); Parts(body, 3, seed, out, true);
    ok = ok && out.size() == 1 && out[0].MeshIndex == 30 && out[0].Variant >= 1 && out[0].Variant <= 3;
    std::printf(ok ? "0086be40_TEST PASS\n" : "0086be40_TEST FAIL\n");
    return ok ? 0 : 1;
}
