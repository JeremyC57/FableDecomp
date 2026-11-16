// Behaviour test for 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData: frees the old data, then loads from the primitive chunk:
// material index = chunk material 0; vertices either from the vertex-buffer chunk (count / flags taken from it) or from the vertex list
// with every non-zero normal normalised (zero normals stay zero); triangle lists appended in chunk order; animated block count = number
// of XAPB sub-chunks (0 without XAPR); static block count = triangle-list count; one pooled allocation of
// animated*sizeof(CAnimatedBlock) + static*sizeof(CStaticBlock) bytes (fails: size outside 0..0x7fffffff or allocation refused ->
// return false); the animated blocks start right after the static ones; static block i gets chunk material index i; groups and cloth
// primitives copied in order; UV tangents copied when present; with XAPR the triangle lists are replaced by the per-block lists; the
// bounding sphere only when present; texture stretch defaults to 0; triangle / index totals from the data; mirrored-texture highlights
// only while the global switch is on; returns true.
#include <cstdio>
#include <cmath>
#include <vector>
struct V3 { float x, y, z; };
struct Vert { V3 pos, normal; };
struct Chunk {
    std::vector<long> materials;
    bool hasVB; unsigned long vbCount, vbFlags;
    std::vector<Vert> verts;
    std::vector<unsigned long> triLists;            // triangle count of each list
    bool hasXAPR; std::vector<unsigned long> xapbTris;
    std::vector<int> groups, cloth;
    std::vector<V3> uTan, vTan;
    bool hasSphere; float radius;
    bool hasStretch; float stretch;
    std::vector<std::vector<unsigned long> > highlights;
};
struct Prim {
    long material; unsigned long vertexCount, initFlags; std::vector<Vert> verts;
    std::vector<unsigned long> lists; unsigned long staticCount, animatedCount;
    std::vector<long> blockMaterial; long animatedOffset;   // animated blocks' byte offset in the pooled block memory
    std::vector<int> groups, cloth; std::vector<V3> uTan, vTan; float radius, stretch;
    unsigned long triangleCount; std::vector<unsigned long> mirrored;
};
static const unsigned long kStatic = 32, kAnimated = 48;
static bool Load(const Chunk& c, Prim& p, unsigned long initFlags, bool loadHighlights, long poolLimit) {
    p = Prim(); p.radius = -1.0f;
    p.initFlags = initFlags; p.material = c.materials[0];
    if (c.hasVB) { p.vertexCount = c.vbCount; p.initFlags = c.vbFlags; }
    else {
        p.verts = c.verts;
        if (p.verts.size())
            for (size_t i = 0; i < p.verts.size(); ++i) {
                V3& n = p.verts[i].normal;
                if (n.x != 0 || n.y != 0 || n.z != 0) { float l = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z); n.x /= l; n.y /= l; n.z /= l; }
            }
        p.vertexCount = (unsigned long)c.verts.size();
    }
    p.lists = c.triLists;
    p.animatedCount = c.hasXAPR ? (unsigned long)c.xapbTris.size() : 0;
    p.staticCount = (unsigned long)p.lists.size();
    __int64 size = (__int64)(p.animatedCount * kAnimated + p.staticCount * kStatic);
    if (!(size >= 0 && size <= 0x7fffffff)) return false;
    if ((long)size > poolLimit) return false;
    p.animatedOffset = p.animatedCount ? (long)(p.staticCount * kStatic) : -1;
    for (unsigned long i = 0; i < p.staticCount; ++i) p.blockMaterial.push_back(c.materials[i]);
    p.groups = c.groups;
    if (!c.uTan.empty()) { p.uTan = c.uTan; p.vTan = c.vTan; }
    if (c.hasXAPR) p.lists = c.xapbTris;
    if (c.hasSphere) p.radius = c.radius;
    p.stretch = 0.0f; if (c.hasStretch) p.stretch = c.stretch;
    p.triangleCount = 0; for (size_t i = 0; i < p.lists.size(); ++i) p.triangleCount += p.lists[i];
    p.cloth = c.cloth;
    if (loadHighlights) for (size_t i = 0; i < c.highlights.size(); ++i) p.mirrored.insert(p.mirrored.end(), c.highlights[i].begin(), c.highlights[i].end());
    return true;
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    Chunk c; c.materials.push_back(7); c.materials.push_back(3); c.materials.push_back(9);
    c.hasVB = false; c.vbCount = 0; c.vbFlags = 0;
    Vert v0 = { { 1, 2, 3 }, { 0, 3, 4 } }, v1 = { { 4, 5, 6 }, { 0, 0, 0 } }; c.verts.push_back(v0); c.verts.push_back(v1);
    c.triLists.push_back(10); c.triLists.push_back(20); c.triLists.push_back(5);
    c.hasXAPR = false; c.groups.push_back(1); c.groups.push_back(2); c.cloth.push_back(4);
    c.hasSphere = true; c.radius = 2.5f; c.hasStretch = false; c.stretch = 0;
    std::vector<unsigned long> h; h.push_back(11); h.push_back(12); c.highlights.push_back(h);
    Prim p;
    check(Load(c, p, 0x21, false, 0x10000), "loads");
    check(p.material == 7 && p.vertexCount == 2 && p.initFlags == 0x21, "material 0, vertex count, flags kept");
    check(std::fabs(p.verts[0].normal.y - 0.6f) < 1e-6f && std::fabs(p.verts[0].normal.z - 0.8f) < 1e-6f, "non-zero normal normalised");
    check(p.verts[1].normal.x == 0 && p.verts[1].normal.y == 0 && p.verts[1].normal.z == 0, "zero normal left alone");
    check(p.staticCount == 3 && p.animatedCount == 0 && p.animatedOffset == -1, "static blocks = triangle lists, no animated blocks");
    check(p.blockMaterial.size() == 3 && p.blockMaterial[0] == 7 && p.blockMaterial[1] == 3 && p.blockMaterial[2] == 9, "block i gets material i");
    check(p.triangleCount == 35 && p.radius == 2.5f && p.stretch == 0.0f, "totals, sphere, default stretch");
    check(p.groups.size() == 2 && p.cloth.size() == 1 && p.mirrored.empty(), "groups, cloth; highlights off");
    check(Load(c, p, 0, true, 0x10000) && p.mirrored.size() == 2 && p.mirrored[1] == 12, "highlights loaded when switched on");
    c.hasXAPR = true; c.xapbTris.push_back(8); c.xapbTris.push_back(4);
    check(Load(c, p, 0, false, 0x10000), "animated load");
    check(p.animatedCount == 2 && p.animatedOffset == 3 * 32 && p.triangleCount == 12, "animated blocks after the static ones, per-block lists");
    check(!Load(c, p, 0, false, 3 * 32 + 2 * 48 - 1), "allocation refused -> false");
    check(Load(c, p, 0, false, 3 * 32 + 2 * 48), "exact pool size fits");
    c.hasVB = true; c.vbCount = 77; c.vbFlags = 0x5;
    check(Load(c, p, 0x21, false, 0x10000) && p.vertexCount == 77 && p.initFlags == 5 && p.verts.empty(), "vertex-buffer chunk: count and flags from it");
    c.hasStretch = true; c.stretch = 1.5f; c.hasSphere = false;
    check(Load(c, p, 0, false, 0x10000) && p.stretch == 1.5f && p.radius == -1.0f, "stretch from chunk, sphere untouched when absent");
    if (fails == 0) std::printf("00a91640_TEST PASS\n");
    return fails;
}