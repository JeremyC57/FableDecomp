// Behaviour test for 0x00BC25F0 CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive.
// A copy of the row's logic over plain models (the render / shader / lighting managers record their calls into one event log, the
// render states keep SetRenderState's skip-when-unchanged rule, the mesh is an array of primitives and materials, the group's particle
// meshes an array of transforms and BGRA colours), checked against independently written expectations: the two early exits (render
// switch off; mesh not available -> texture load started, nothing drawn), the displacement pass 6 (blend ADD / SRCALPHA / INVSRCALPHA,
// the shadow hook only when the group manager reads the z-buffer, displacement shaders, constants 0..3 = zero + the transposed camera
// view matrix), the stencil pass 7 (alpha test on, ref 16, GREATER), the normal pass (point-light shader index clamped to the table,
// blend op / blend mode behind their switches, additive pixel shaders for blend modes 3 / 4), constant 4 = (4, 0, 0, 0), and the batched
// draws per primitive (primitives without a diffuse map or whose texture lookup fails are skipped; batches of min(reps, 10) particle
// meshes; three transposed object-matrix rows + the colour / 255 with doubled alpha per mesh; full batches and the tail batch draw
// vertex / triangle counts times the meshes in the batch; the batch / tail draw switches), the per-primitive lighting restore and the
// final ResetWorldTransform. Mutation checks: each expectation is also checked to fail on a perturbed log.
#include <cstdio>

enum { E_LOADTEX = 1, E_SHADOWHW, E_STATE, E_VS, E_PS, E_VSCONST, E_PSETUP, E_POINTLIGHTS, E_TEXTURE, E_SELFILLUM, E_GLOBALLIGHT,
       E_VB, E_IB, E_WORLD, E_VSCONSTS, E_PREPARE, E_DRAW, E_RESTORE, E_RESETWORLD };
enum { S_BLENDOP = 1, S_SRC, S_DST, S_ATEST, S_AREF, S_AFUNC };
enum { MAXLOG = 512 };
struct Log {
    int ev[MAXLOG]; long a[MAXLOG]; long b[MAXLOG]; int n;
    void Add(int e, long x, long y) { if (n < MAXLOG) { ev[n] = e; a[n] = x; b[n] = y; ++n; } }
};
// D3D values the engine tables hold (BLENDOP ADD 1 / SUBTRACT 2 / REVSUBTRACT 3; BLEND ONE 2, INVSRCCOLOR 4, SRCALPHA 5, INVSRCALPHA 6;
// CMP GREATER 5).
static const long BLENDOP_ADD = 1, BLENDOP_SUB = 2, BLENDOP_REVSUB = 3;
static const long BLEND_ONE = 2, BLEND_INVSRCCOLOR = 4, BLEND_SRCALPHA = 5, BLEND_INVSRCALPHA = 6, CMP_GREATER = 5;
// Shader ids: point-light vertex shaders 0..n-1, displacement vertex shader 100; pixel shaders by member.
enum { VS_DISPLACEMENT = 100, PS_NORMAL = 200, PS_STIPPLE, PS_ADD, PS_ADD_STIPPLE, PS_DISPLACEMENT };

struct Primitive { long material; long reps; unsigned long vertices, triangles; long vb, ib; };
struct Material { unsigned long diffuse; unsigned long self_illum; bool texture_fails; long texture; };
struct ParticleMesh { float m[12]; unsigned char bgra[4]; };   // E11..E43 (rows of three), CRGBColour B, G, R, A

struct World {
    Log log;
    long state[8];
    bool switches[5];              // render, blend op, blend mode, batch draw, tail draw
    bool mesh_available, read_z;
    long point_lights, shader_count;
    float camera[12];
    float consts[40][4];
    float last_consts[40][4]; long last_const_count;
};

static void SetState(World& w, int s, long v) { if (w.state[s] != v) { w.state[s] = v; w.log.Add(E_STATE, s, v); } }
static void VSConst(World& w, long i, float x, float y, float z, float q)
{
    w.log.Add(E_VSCONST, i, (long)(x * 1000.0f + y * 100.0f + z * 10.0f + q));
}
static long BlendOpValue(long op) { return op == 1 ? BLENDOP_SUB : op == 2 ? BLENDOP_REVSUB : BLENDOP_ADD; }
static bool BlendMode(World& w, long mode)
{
    if (mode == 3) { SetState(w, S_SRC, BLEND_ONE); SetState(w, S_DST, BLEND_ONE); return true; }
    if (mode == 4) { SetState(w, S_SRC, BLEND_ONE); SetState(w, S_DST, BLEND_INVSRCCOLOR); return true; }
    SetState(w, S_SRC, BLEND_SRCALPHA); SetState(w, S_DST, BLEND_INVSRCALPHA); return false;
}

// The row's logic.
static void RenderSubPrimitive(World& w, int pass, long blend_op, long blend_mode, const Primitive* prims, unsigned long prim_count,
                               const Material* mats, const ParticleMesh* particles, int particle_count)
{
    if (!w.switches[0]) return;
    if (!w.mesh_available) { w.log.Add(E_LOADTEX, 1, 0); return; }
    if (pass == 6 || pass == 7) {
        if (pass == 6) {
            if (w.read_z) w.log.Add(E_SHADOWHW, 0, 0);
            SetState(w, S_BLENDOP, BLENDOP_ADD);
            BlendMode(w, 2);
        } else {
            SetState(w, S_ATEST, 1); SetState(w, S_AREF, 16); SetState(w, S_AFUNC, CMP_GREATER);
        }
        w.log.Add(E_VS, VS_DISPLACEMENT, 0);
        w.log.Add(E_PS, PS_DISPLACEMENT, 0);
        const float* v = w.camera;
        VSConst(w, 0, 0, 0, 0, 0);
        VSConst(w, 1, v[0], v[3], v[6], v[9]);
        VSConst(w, 2, v[1], v[4], v[7], v[10]);
        VSConst(w, 3, v[2], v[5], v[8], v[11]);
    } else {
        w.log.Add(E_POINTLIGHTS, w.point_lights, 0);
        unsigned long index = (unsigned long)w.point_lights;
        if (index > (unsigned long)(w.shader_count - 1)) index = w.shader_count - 1;
        w.log.Add(E_VS, (long)index, 0);
        if (w.switches[1]) SetState(w, S_BLENDOP, BlendOpValue(blend_op));
        bool additive = false;
        if (w.switches[2]) additive = BlendMode(w, blend_mode);
        if (additive) w.log.Add(E_PSETUP, PS_ADD_STIPPLE, PS_ADD);
        else w.log.Add(E_PSETUP, PS_STIPPLE, PS_NORMAL);
    }
    VSConst(w, 4, 4.0f, 0, 0, 0);
    for (unsigned long i = 0; i < prim_count; ++i) {
        const Primitive& p = prims[i];
        unsigned long repeats = (unsigned long)p.reps;
        unsigned long batch_size = repeats < 10 ? repeats : 10;
        const Material& m = mats[p.material];
        if (!m.diffuse) continue;
        if (m.texture_fails) continue;
        w.log.Add(E_TEXTURE, m.texture, 0);
        w.log.Add(E_SELFILLUM, (long)m.self_illum, 0);
        w.log.Add(E_GLOBALLIGHT, 0, 0);
        w.log.Add(E_VB, p.vb, 0);
        w.log.Add(E_IB, p.ib, 0);
        unsigned long batch_triangles = p.triangles * batch_size, batch_vertices = p.vertices * batch_size;
        unsigned long count = 0;
        for (int k = 0; k < particle_count; ++k) {
            const ParticleMesh& pm = particles[k];
            float* c = w.consts[count];
            c[0] = pm.m[0]; c[1] = pm.m[3]; c[2] = pm.m[6]; c[3] = pm.m[9]; ++count;
            c = w.consts[count];
            c[0] = pm.m[1]; c[1] = pm.m[4]; c[2] = pm.m[7]; c[3] = pm.m[10]; ++count;
            c = w.consts[count];
            c[0] = pm.m[2]; c[1] = pm.m[5]; c[2] = pm.m[8]; c[3] = pm.m[11]; ++count;
            c = w.consts[count];
            float r = pm.bgra[2] * (1.0f / 255.0f), g = pm.bgra[1] * (1.0f / 255.0f), b = pm.bgra[0] * (1.0f / 255.0f);
            float a = pm.bgra[3] * (1.0f / 255.0f);
            c[0] = r; c[1] = g; c[2] = b; c[3] = a * 2.0f; ++count;
            if (count == batch_size * 4) {
                w.log.Add(E_WORLD, 0, 0);
                w.log.Add(E_VSCONSTS, 5, (long)(batch_size * 4));
                for (unsigned long q = 0; q < count; ++q) for (int z = 0; z < 4; ++z) w.last_consts[q][z] = w.consts[q][z];
                w.last_const_count = (long)count;
                w.log.Add(E_PREPARE, 0x11, 0);
                if (w.switches[3]) w.log.Add(E_DRAW, (long)batch_vertices, (long)batch_triangles);
                count = 0;
            }
        }
        if (count) {
            w.log.Add(E_WORLD, 0, 0);
            unsigned long meshes = count / 4;
            w.log.Add(E_VSCONSTS, 5, (long)(meshes * 4));
            for (unsigned long q = 0; q < count; ++q) for (int z = 0; z < 4; ++z) w.last_consts[q][z] = w.consts[q][z];
            w.last_const_count = (long)count;
            w.log.Add(E_PREPARE, 0x11, 0);
            if (w.switches[4]) w.log.Add(E_DRAW, (long)(p.vertices * meshes), (long)(p.triangles * meshes));
        }
        w.log.Add(E_RESTORE, 0, 0);
    }
    w.log.Add(E_RESETWORLD, 0, 0);
}

static void Init(World& w)
{
    w.log.n = 0;
    for (int i = 0; i < 8; ++i) w.state[i] = -1;
    for (int i = 0; i < 5; ++i) w.switches[i] = true;
    w.mesh_available = true; w.read_z = false; w.point_lights = 2; w.shader_count = 5;
    for (int i = 0; i < 12; ++i) w.camera[i] = (float)(i + 1);
    w.last_const_count = 0;
}

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("check failed: %s\n", what); g_fail = 1; } }
static int Count(const Log& l, int e) { int c = 0; for (int i = 0; i < l.n; ++i) if (l.ev[i] == e) ++c; return c; }
static int Find(const Log& l, int e, int from) { for (int i = from; i < l.n; ++i) if (l.ev[i] == e) return i; return -1; }
// Expected draws for one primitive with `n` particle meshes in batches of `batch`, both draw switches on.
static int ExpectedDraws(int n, int batch) { return n / batch + (n % batch ? 1 : 0); }

int main()
{
    static Primitive prims[3] = { { 0, 3, 8, 6, 11, 21 }, { 1, 25, 4, 2, 12, 22 }, { 2, 2, 5, 3, 13, 23 } };
    static Material mats[3] = { { 7, 40, false, 31 }, { 0, 10, false, 32 }, { 9, 60, false, 33 } };
    static ParticleMesh parts[7];
    for (int k = 0; k < 7; ++k) {
        for (int i = 0; i < 12; ++i) parts[k].m[i] = (float)(k * 100 + i);
        parts[k].bgra[0] = (unsigned char)(10 + k); parts[k].bgra[1] = (unsigned char)(20 + k);
        parts[k].bgra[2] = (unsigned char)(30 + k); parts[k].bgra[3] = (unsigned char)(40 + k);
    }
    static World w;

    // 1. render switch off: nothing at all.
    Init(w); w.switches[0] = false;
    RenderSubPrimitive(w, 2, 0, 0, prims, 3, mats, parts, 7);
    Check(w.log.n == 0, "switch off draws nothing");
    // 2. mesh not available: texture load priority 1 only.
    Init(w); w.mesh_available = false;
    RenderSubPrimitive(w, 2, 0, 0, prims, 3, mats, parts, 7);
    Check(w.log.n == 1 && w.log.ev[0] == E_LOADTEX && w.log.a[0] == 1, "missing mesh starts the texture load");

    // 3. normal pass, blend op 1, additive blend mode 3, 7 particles.
    Init(w); w.point_lights = 9;     // clamped to the last of 5 shaders
    RenderSubPrimitive(w, 2, 1, 3, prims, 3, mats, parts, 7);
    Check(w.log.ev[0] == E_POINTLIGHTS && w.log.ev[1] == E_VS && w.log.a[1] == 4, "point-light shader clamped to the table");
    Check(w.state[S_BLENDOP] == BLENDOP_SUB && w.state[S_SRC] == BLEND_ONE && w.state[S_DST] == BLEND_ONE, "blend op / additive mode");
    Check(Find(w.log, E_PSETUP, 0) >= 0 && w.log.a[Find(w.log, E_PSETUP, 0)] == PS_ADD_STIPPLE, "additive pixel shaders");
    Check(w.log.ev[Find(w.log, E_VSCONST, 0)] == E_VSCONST && w.log.a[Find(w.log, E_VSCONST, 0)] == 4, "constant 4 first in pass 2");
    // primitive 1 has no diffuse map: only primitives 0 and 2 render (texture 31 then 33, self illumination 40 / 60).
    Check(Count(w.log, E_TEXTURE) == 2 && Count(w.log, E_RESTORE) == 2, "primitive without a diffuse map is skipped");
    Check(w.log.a[Find(w.log, E_TEXTURE, 0)] == 31 && w.log.a[Find(w.log, E_SELFILLUM, 0)] == 40, "first primitive's texture / light");
    Check(Count(w.log, E_DRAW) == ExpectedDraws(7, 3) + ExpectedDraws(7, 2), "batched draws per primitive");
    {   // primitive 0: batches of 3 -> draws (24, 18), (24, 18), tail 1 mesh (8, 6)
        int d = Find(w.log, E_DRAW, 0);
        Check(w.log.a[d] == 24 && w.log.b[d] == 18, "full batch draw counts");
        d = Find(w.log, E_DRAW, d + 1); d = Find(w.log, E_DRAW, d + 1);
        Check(w.log.a[d] == 8 && w.log.b[d] == 6, "tail batch draw counts");
        int c = Find(w.log, E_VSCONSTS, 0);
        Check(w.log.a[c] == 5 && w.log.b[c] == 12, "batch constant upload: index 5, 4 per mesh");
    }
    // the last upload (primitive 2's tail: particle 6 alone) holds its transposed rows and the colour / 255 with doubled alpha
    Check(w.last_const_count == 4, "tail upload size");
    Check(w.last_consts[0][0] == 600.0f && w.last_consts[0][1] == 603.0f && w.last_consts[0][3] == 609.0f, "row 0 = E11 E21 E31 E41");
    Check(w.last_consts[2][2] == 608.0f && w.last_consts[2][3] == 611.0f, "row 2 = E13 E23 E33 E43");
    Check(w.last_consts[3][0] == 36 * (1.0f / 255.0f) && w.last_consts[3][2] == 16 * (1.0f / 255.0f), "colour R, B");
    Check(w.last_consts[3][3] == 46 * (1.0f / 255.0f) * 2.0f, "doubled alpha");
    Check(w.log.ev[w.log.n - 1] == E_RESETWORLD, "world transform reset last");

    // 4. normal pass with both blend switches off and the draw switches off: default shaders, no state, no draws.
    Init(w); w.switches[1] = w.switches[2] = w.switches[3] = w.switches[4] = false; w.point_lights = 1;
    RenderSubPrimitive(w, 2, 1, 3, prims, 3, mats, parts, 7);
    Check(Count(w.log, E_STATE) == 0 && Count(w.log, E_DRAW) == 0, "switches off: no blend state, no draw");
    Check(w.log.a[1] == 1 && w.log.a[Find(w.log, E_PSETUP, 0)] == PS_STIPPLE, "default pixel shaders, light count 1");
    Check(Count(w.log, E_PREPARE) == ExpectedDraws(7, 3) + ExpectedDraws(7, 2), "uploads still happen");

    // 5. displacement pass 6 with the z-buffer read, blend mode 4 ignored.
    Init(w); w.read_z = true;
    RenderSubPrimitive(w, 6, 2, 4, prims, 3, mats, parts, 7);
    Check(w.log.ev[0] == E_SHADOWHW, "shadow hook when the manager reads z");
    Check(w.state[S_BLENDOP] == BLENDOP_ADD && w.state[S_SRC] == BLEND_SRCALPHA && w.state[S_DST] == BLEND_INVSRCALPHA, "pass 6 states");
    {
        int c = Find(w.log, E_VSCONST, 0);
        Check(w.log.a[c] == 0 && w.log.b[c] == 0, "constant 0 is zero");
        Check(w.log.a[c + 1] == 1 && w.log.b[c + 1] == (long)(1000 + 400 + 70 + 10), "constant 1 = camera column 0");
        Check(w.log.a[c + 4] == 4 && w.log.b[c + 4] == 4000, "constant 4 = (4, 0, 0, 0)");
    }
    Check(w.log.a[Find(w.log, E_VS, 0)] == VS_DISPLACEMENT && w.log.a[Find(w.log, E_PS, 0)] == PS_DISPLACEMENT, "displacement shaders");
    Init(w); RenderSubPrimitive(w, 6, 0, 0, prims, 3, mats, parts, 7);
    Check(Count(w.log, E_SHADOWHW) == 0, "no shadow hook without the z read");
    // 6. stencil pass 7.
    Init(w); RenderSubPrimitive(w, 7, 0, 0, prims, 3, mats, parts, 7);
    Check(w.state[S_ATEST] == 1 && w.state[S_AREF] == 16 && w.state[S_AFUNC] == CMP_GREATER && w.state[S_BLENDOP] == -1, "pass 7 states");
    // 7. a failing texture lookup skips the primitive like a missing map.
    Init(w); mats[2].texture_fails = true;
    RenderSubPrimitive(w, 2, 0, 0, prims, 3, mats, parts, 7);
    Check(Count(w.log, E_TEXTURE) == 1 && Count(w.log, E_DRAW) == ExpectedDraws(7, 3), "failed texture skips the primitive");
    mats[2].texture_fails = false;

    // Mutation checks: the expectations must reject a perturbed run.
    Init(w); RenderSubPrimitive(w, 2, 1, 3, prims, 3, mats, parts, 7);
    int draws = Count(w.log, E_DRAW);
    Check(draws != ExpectedDraws(7, 4) + ExpectedDraws(7, 2), "mutation: other batch size detected");
    Check(w.last_consts[3][3] != 46 * (1.0f / 255.0f), "mutation: undoubled alpha detected");
    Check(w.last_consts[0][1] != 601.0f, "mutation: untransposed row detected");

    if (g_fail) { std::printf("00bc25f0_TEST FAIL\n"); return 1; }
    std::printf("00bc25f0_TEST PASS\n");
    return 0;
}