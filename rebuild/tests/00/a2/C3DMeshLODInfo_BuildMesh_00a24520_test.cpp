// Behaviour test for C3DMeshLODInfo::BuildMesh 0x00A24520: a copy of the model's control flow over fakes.
#include <cstdio>
struct Prim { long MaterialIndex; int kind; unsigned long flags; bool ret; };
struct Mat { unsigned long MapFlags; };
struct Mesh { Prim prims[4]; Mat mats[2]; unsigned long count; unsigned long id; int loads; int failFirst; int failAll; bool texChanged; };
struct Bank { bool dp3; int texBank; int statsAdds; int meshAdds; };
static unsigned long g_NextMeshID = 7;
static int g_pool = 1, g_cloth = 2;
static bool Load(Mesh& m, bool flag) { ++m.loads; (void)flag; if (m.failAll) return false; return !(m.failFirst && m.loads == 1); }
static bool Upload(Prim& p, int kind, unsigned long f) { p.kind = kind; p.flags = f; return p.ret; }
static bool BuildMesh(Mesh& m, Bank& b, unsigned long type, unsigned char lod, bool flag, bool& released)
{
    type &= 0xf;
    bool animated = type == 5, repeated = type == 2, type4 = type == 4;
    unsigned long id = g_NextMeshID++;
    int oldPool = g_pool, oldCloth = g_cloth; g_pool = 10; g_cloth = 20;
    released = false;
    if (!Load(m, flag)) { if (!Load(m, false)) { released = true; return false; } }
    m.id = id;
    if (b.texBank) m.texChanged = true;
    bool ok = true;
    for (unsigned long i = 0; i < m.count && ok; ++i) {
        Prim& p = m.prims[i];
        unsigned long uf = 0;
        if ((m.mats[p.MaterialIndex].MapFlags & 2) && b.dp3) uf = 2;
        if (animated) ok &= Upload(p, 1, uf | 8);
        else if (repeated) ok &= Upload(p, 2, uf);
        else if (type4) ok &= Upload(p, 2, uf);
        else ok &= Upload(p, 3, uf | 8);
    }
    g_pool = oldPool; g_cloth = oldCloth;
    if (!ok) { released = true; return false; }
    if (lod == 4) ++b.statsAdds; else ++b.meshAdds;
    return true;
}
int main()
{
    bool fail = false, rel;
    Mesh m = {}; Bank b = {true, 1, 0, 0};
    m.count = 2; m.mats[1].MapFlags = 2; m.prims[0].MaterialIndex = 1; m.prims[0].ret = true; m.prims[1].ret = true;
    if (!BuildMesh(m, b, 0x15, 0, true, rel) || rel || m.id != 7 || m.prims[0].kind != 1 || m.prims[0].flags != 10 || m.prims[1].flags != 8 || !m.texChanged || b.meshAdds != 1 || g_pool != 1) fail = true;
    Mesh m2 = {}; m2.count = 3; m2.failFirst = 1; m2.prims[0].ret = true; m2.prims[1].ret = false; m2.prims[2].ret = true; b.dp3 = false;
    if (BuildMesh(m2, b, 2, 4, false, rel) || !rel || m2.loads != 2 || m2.prims[2].kind != 0 || m2.prims[0].flags != 0 || b.statsAdds != 0) fail = true;
    Mesh m3 = {}; m3.failAll = 1;
    if (BuildMesh(m3, b, 0, 0, false, rel) || !rel || m3.loads != 2 || g_NextMeshID != 10) fail = true;
    if (fail) { std::printf("00a24520_TEST FAIL\n"); return 1; }
    std::printf("00a24520_TEST PASS\n");
    return 0;
}