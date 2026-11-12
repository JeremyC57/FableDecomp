// Behaviour test for 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive(): a self-contained host model of the
// row's logic -- disabled manager renders nothing; the camera distance of the bounding sphere; in an occlusion pass a persistent
// primitive sets up its layer record and an occluded primitive queues a bounding box (result 1); the LOD is the last one whose
// error is within nearest / (radius * screen size) * tolerance (nearest clamped to DELTA_NOTIONAL_ZERO); a mesh that is not
// resident queues a bounding box and starts the texture load (result 1); otherwise the mesh and lights are recorded, the box is
// clipped against the shadow receiver volume and one sub-primitive per textured mesh primitive is queued with its render mode
// (map flags) and the CalculateSubPrimitiveRenderFlags word (fading / translucent / boolean alpha / primitive flags / shadowed)
// (result 2). Host mocks only; no engine headers.
#include <cstdio>
#include <cmath>

static const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct Vec { float X, Y, Z; };
struct Material { unsigned long DiffuseMapBankIndex, MapFlags; bool Transparent, BooleanAlpha;
                  bool IsTransparent() const { return Transparent; } bool IsBooleanAlpha() const { return BooleanAlpha; }
                  unsigned long GetMapFlags() const { return MapFlags; } unsigned long GetDiffuseMapBankIndex() const { return DiffuseMapBankIndex; } };
struct Mesh { unsigned long PrimitiveCount; long MaterialIndex[4]; Material Materials[4]; };
struct MeshInfo { unsigned long LODDataSizes[4]; float LODErrors[4]; bool resident[4]; Mesh meshes[4]; long textureLoads; long fetchedLod;
                  bool HasLOD(unsigned long l) const { return l < 4 && LODDataSizes[l] != 0; }
                  float GetLODError(unsigned long l) const { return LODErrors[l]; } };
struct Queued { long kind; long renderMode; unsigned long flags; float distance; unsigned char alpha; };
struct World {
    Vec camera; bool occlusionEnable; long occlusionLayer; float tolerance, screenSize; bool blockOnLoad; float drawFactor;
    bool shadowEnabled; long shadowMode; bool clipResult; long setups; bool occluded;
    Queued q[8]; long nq; const Mesh* recordedMesh; bool lightsRecorded;
};
struct Primitive { Vec center; float radius; bool persistent; unsigned long EnginePrimitiveFlags; float FadeBeginDistance; MeshInfo* info; };

static unsigned long CalculateSubPrimitiveRenderFlags(World& w, bool fading, bool translucent, bool alpha_test, unsigned long primitive_flags, bool shadowed)
{
    bool alpha = alpha_test;
    if ((primitive_flags & 0x100) && translucent)
        alpha = true;
    unsigned long flags = 0;
    if (fading)
        flags = ((unsigned char)(primitive_flags >> 9) & 1) ? 10 : 5;
    if (alpha)
        flags |= 2;
    else if (translucent)
        flags |= 5;
    if (shadowed && w.shadowEnabled) {
        flags |= 0x200;
        if (w.shadowMode == 2)
            flags |= 0x10;
    }
    if (primitive_flags & 0x800)
        flags |= 0xc0;
    if (primitive_flags & 8)
        flags |= 0x180;
    return flags;
}

static long RenderPrimitive(bool enabled, World& w, Primitive& p)
{
    if (!enabled)
        return 0;
    Vec d = { p.center.X - w.camera.X, p.center.Y - w.camera.Y, p.center.Z - w.camera.Z };
    float distance = std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
    float radius = p.radius;
    if (w.occlusionEnable) {
        if (p.persistent)
            ++w.setups;
        if (!(p.EnginePrimitiveFlags & 0x4000) && w.occluded) {
            Queued e = { 7, 0, 0, distance, 0 }; w.q[w.nq++] = e;
            return 1;
        }
    }
    float nearest = distance - radius;
    if (!(nearest > DELTA_NOTIONAL_ZERO))
        nearest = DELTA_NOTIONAL_ZERO;
    float lod_error = nearest / (radius * w.screenSize) * w.tolerance;
    unsigned long lod = 0;
    while (p.info->HasLOD(lod + 1) && !(p.info->GetLODError(lod + 1) > lod_error))
        ++lod;
    p.info->fetchedLod = (long)lod;
    if (!p.info->resident[lod] && !w.blockOnLoad) {
        Queued e = { 7, 0, 0, distance, 0 }; w.q[w.nq++] = e;
        ++p.info->textureLoads;
        return 1;
    }
    const Mesh* mesh = &p.info->meshes[lod];
    w.recordedMesh = mesh; w.lightsRecorded = true;
    bool shadowed = w.clipResult;
    for (unsigned long i = 0; i < mesh->PrimitiveCount; ++i) {
        const Material* material = &mesh->Materials[mesh->MaterialIndex[i]];
        if (material->GetDiffuseMapBankIndex() == 0)
            continue;
        long render_mode;
        unsigned long map_flags = material->GetMapFlags();
        if (map_flags & 1) {
            switch (map_flags & ~1) {
            case 2: case 6: render_mode = 8; break;
            case 4: render_mode = 7; break;
            default: render_mode = 4; break;
            }
        } else {
            render_mode = 1;
        }
        bool fading = radius + distance > w.drawFactor * p.FadeBeginDistance;
        Queued e = { 9, render_mode, CalculateSubPrimitiveRenderFlags(w, fading, material->IsTransparent(), material->IsBooleanAlpha(),
                                                                       p.EnginePrimitiveFlags | 0x200, shadowed), distance, 255 };
        w.q[w.nq++] = e;
    }
    return 2;
}

static int fails = 0;
static void check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(c) check((c), __LINE__)

int main()
{
    MeshInfo info = {};
    for (int l = 0; l < 4; ++l) { info.LODDataSizes[l] = 1; info.LODErrors[l] = (float)l; info.resident[l] = true; }
    info.LODDataSizes[3] = 0;
    for (int l = 0; l < 4; ++l) {
        Mesh& m = info.meshes[l];
        m.PrimitiveCount = 4;
        m.MaterialIndex[0] = 0; m.MaterialIndex[1] = 1; m.MaterialIndex[2] = 2; m.MaterialIndex[3] = 3;
        Material a = { 5, 0, false, false };   // opaque: mode 1, flags only from primitive bits
        Material b = { 6, 3, true, false };    // MapFlags 3 -> mode 8, translucent -> |5 (or alpha with 0x100)
        Material c = { 0, 5, true, true };     // no diffuse map: skipped
        Material e = { 7, 9, false, true };    // MapFlags 9 -> mode 4, boolean alpha -> |2
        m.Materials[0] = a; m.Materials[1] = b; m.Materials[2] = c; m.Materials[3] = e;
    }
    World w = {};
    w.camera.X = 0; w.camera.Y = 0; w.camera.Z = 0; w.tolerance = 1.0f; w.screenSize = 1.0f; w.drawFactor = 1.0f;
    w.shadowEnabled = true; w.shadowMode = 2; w.clipResult = true; w.occlusionLayer = 1;
    Primitive p = {}; p.center.X = 3; p.center.Y = 4; p.center.Z = 0; p.radius = 1.0f; p.FadeBeginDistance = 100.0f; p.info = &info;

    // disabled
    CHECK(RenderPrimitive(false, w, p) == 0 && w.nq == 0);

    // occluded in an occlusion pass: one bounding box, result 1; persistent primitive sets up its record
    w.occlusionEnable = true; w.occluded = true; p.persistent = true;
    CHECK(RenderPrimitive(true, w, p) == 1 && w.nq == 1 && w.q[0].kind == 7 && std::fabs(w.q[0].distance - 5.0f) < 1e-5f && w.setups == 1);
    // flag 0x4000 overrides occlusion
    w.nq = 0; p.EnginePrimitiveFlags = 0x4000;
    CHECK(RenderPrimitive(true, w, p) == 2);
    p.EnginePrimitiveFlags = 0; w.occluded = false; w.occlusionEnable = false; w.nq = 0;

    // LOD: nearest = 5 - 1 = 4, lod_error = 4 -> LODs 1,2 qualify (3 has no data) -> lod 2
    CHECK(RenderPrimitive(true, w, p) == 2 && info.fetchedLod == 2 && w.recordedMesh == &info.meshes[2] && w.lightsRecorded);
    // three textured primitives queued
    CHECK(w.nq == 3);
    CHECK(w.q[0].renderMode == 1 && w.q[0].flags == (0x200 | 0x10) && w.q[0].alpha == 255);
    CHECK(w.q[1].renderMode == 8 && w.q[1].flags == (5 | 0x200 | 0x10));
    CHECK(w.q[2].renderMode == 4 && w.q[2].flags == (2 | 0x200 | 0x10));

    // tighter tolerance: lod_error 1.5 -> lod 1; not resident -> bounding box + texture load, result 1
    w.nq = 0; w.tolerance = 0.375f; info.resident[1] = false;
    CHECK(RenderPrimitive(true, w, p) == 1 && info.fetchedLod == 1 && w.nq == 1 && w.q[0].kind == 7 && info.textureLoads == 1);
    // block-on-load fetches it anyway
    w.nq = 0; w.blockOnLoad = true;
    CHECK(RenderPrimitive(true, w, p) == 2 && w.nq == 3);
    w.blockOnLoad = false; info.resident[1] = true; w.tolerance = 1.0f;

    // fading (radius + distance 6 > 1 * 5), primitive flags 0x100 | 0x800 | 8 | (0x200 forced), no shadow
    w.nq = 0; p.FadeBeginDistance = 5.0f; p.EnginePrimitiveFlags = 0x100 | 0x800 | 8; w.clipResult = false;
    CHECK(RenderPrimitive(true, w, p) == 2 && w.nq == 3);
    // 0x200 bit set -> fading gives 10; translucent with 0x100 -> alpha -> |2; 0x800 -> |0xc0; 8 -> |0x180
    CHECK(w.q[0].flags == (10 | 0xc0 | 0x180));
    CHECK(w.q[1].flags == (10 | 2 | 0xc0 | 0x180));
    CHECK(w.q[2].flags == (10 | 2 | 0xc0 | 0x180));
    // shadow mode other than 2: no 0x10
    w.nq = 0; p.EnginePrimitiveFlags = 0; p.FadeBeginDistance = 100.0f; w.clipResult = true; w.shadowMode = 1;
    CHECK(RenderPrimitive(true, w, p) == 2 && w.q[0].flags == 0x200 && w.q[1].flags == (5 | 0x200));
    // shadow renderer disabled: no shadow bits
    w.nq = 0; w.shadowEnabled = false;
    CHECK(RenderPrimitive(true, w, p) == 2 && w.q[0].flags == 0 && w.q[1].flags == 5);
    // nearest clamped: camera inside the sphere -> lod_error tiny -> lod 0
    w.nq = 0; p.radius = 10.0f;
    CHECK(RenderPrimitive(true, w, p) == 2 && info.fetchedLod == 0);

    if (fails) { std::printf("00b9a4a0_TEST FAIL\n"); return 1; }
    std::printf("00b9a4a0_TEST PASS\n");
    return 0;
}
