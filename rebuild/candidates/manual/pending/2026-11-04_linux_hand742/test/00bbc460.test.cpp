// Behaviour test for CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive (0x00BBC460): a standalone model of the per-pass
// render-state choice (6 displacement: blend on / z-write off; 7: both off; otherwise z-write = group flag, blend = !z-write,
// z-test = group flag), the fixed states (wrap UVs, alpha test GREATER ref 0, no fog, dither = group flag), one world-transform
// reset + RenderSubPrimGroups per sub-group, and the stage-1 detach for pass 6 or a z-writing group (in any pass).
#include <cstdio>
#include <vector>
struct States { States() : uvU(0), uvV(0), alphaTest(0), alphaFunc(0), alphaRef(0), fog(0), dither(0), blend(0), zwrite(0), ztest(0), resets(0), groups(0) { }
    int uvU, uvV, alphaTest, alphaFunc, alphaRef, fog, dither, blend, zwrite, ztest; std::vector<int> detached; int resets, groups; };
struct Group { bool readZ, writeZ, dither; int subGroups; };
static void Render(const Group& g, int pass, States& s)
{
    s.uvU = 0; s.uvV = 0; s.alphaTest = 1; s.alphaFunc = 4; s.alphaRef = 0; s.fog = 0; s.dither = g.dither;
    bool writeZ = g.writeZ, readZ = g.readZ;
    if (pass == 6) { s.blend = 1; s.zwrite = 0; }
    else if (pass == 7) { s.blend = 0; s.zwrite = 0; }
    else { s.zwrite = writeZ; s.blend = !writeZ; s.ztest = readZ; }
    for (int i = 0; i < g.subGroups; ++i) { ++s.resets; ++s.groups; }
    s.detached.push_back(0);
    if (pass == 6 || writeZ) s.detached.push_back(1);
}
int main()
{
    bool ok = true;
    { States s; s.ztest = -1; Group g = { true, false, true, 3 }; Render(g, 6, s);
      ok = ok && s.blend == 1 && s.zwrite == 0 && s.ztest == -1 && s.detached.size() == 2 && s.groups == 3 && s.resets == 3 && s.dither == 1; }
    { States s; s.ztest = -1; Group g = { true, true, false, 2 }; Render(g, 7, s);
      ok = ok && s.blend == 0 && s.zwrite == 0 && s.ztest == -1 && s.detached.size() == 2 && s.groups == 2; }
    { States s; Group g = { true, true, false, 1 }; Render(g, 0, s);
      ok = ok && s.blend == 0 && s.zwrite == 1 && s.ztest == 1 && s.detached.size() == 2 && s.alphaFunc == 4 && s.alphaRef == 0; }
    { States s; Group g = { false, false, false, 0 }; Render(g, 1, s);
      ok = ok && s.blend == 1 && s.zwrite == 0 && s.ztest == 0 && s.detached.size() == 1 && s.groups == 0; }
    printf(ok ? "00bbc460_TEST PASS\n" : "00bbc460_TEST FAIL\n");
    return ok ? 0 : 1;
}
