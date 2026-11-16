// Behaviour model of CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks 0x00BA50A0:
//  - stages asking for blank textures: the blank-texture setup runs and the result is true (nothing else touched);
//  - no composite surfaces, the bank's current frame equal to the primitive's recorded frame and equal recorded stages: *already_set_up
//    = true, true, no texture fetched;
//  - otherwise *already_set_up = false; block 0 takes the material's diffuse / bump textures for the stages in use; every later block
//    whose change flag 1 is set takes its own material's diffuse texture (or marks a failure when there is no diffuse stage), and with
//    change flag 2 its bump texture when there is a bump stage; the result is "no fetch failed".
#include <cstdio>

struct Texture { int id; };
struct Material { int diffuse; int bump; bool bad; };
struct Block { int material; unsigned flags; const Texture* diffuse; const Texture* bump; };
struct Stages { bool blank; long diffuse_stage; long bump_stage; int sig; };

static Texture g_tex[16];
static int g_fetches = 0, g_blank_calls = 0;

static const Texture* GetDiffuse(const Material* m, bool& failed) { ++g_fetches; if (m->bad) failed = true; return &g_tex[m->diffuse]; }
static const Texture* GetBump(const Material* m, bool& failed) { ++g_fetches; if (m->bad) failed = true; return &g_tex[m->bump]; }

struct Prim { Block blocks[4]; unsigned count; int recorded_frame; Stages recorded; };

static bool SetupStaticBlocks(Prim& p, const Material* mats, const Stages& st, const Material* material, bool composite,
                              int bank_frame, bool* already)
{
    if (st.blank) { ++g_blank_calls; return true; }
    bool failed = false;
    if (!composite && bank_frame == p.recorded_frame && st.diffuse_stage == p.recorded.diffuse_stage &&
        st.bump_stage == p.recorded.bump_stage && st.sig == p.recorded.sig) { *already = true; return true; }
    *already = false;
    if (st.diffuse_stage != -1) p.blocks[0].diffuse = GetDiffuse(material, failed);
    if (st.bump_stage != -1) p.blocks[0].bump = GetBump(material, failed);
    for (unsigned i = 1; i < p.count; ++i) {
        Block& b = p.blocks[i]; const Material* m = &mats[b.material];
        if (b.flags & 1) { if (st.diffuse_stage != -1) b.diffuse = GetDiffuse(m, failed); else failed = true; }
        if ((b.flags & 2) && st.bump_stage != -1) b.bump = GetBump(m, failed);
    }
    return !failed;
}

static int fail(int n) { std::printf("FAIL %d\n", n); return 1; }

int main()
{
    Material mats[3] = { { 1, 2, false }, { 3, 4, false }, { 5, 6, true } };
    Stages st = { false, 0, 1, 7 };
    Prim p = { { { 0, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 2, 0, 0 }, { 1, 3, 0, 0 } }, 4, 10, st };
    bool already = false;

    Stages blank = st; blank.blank = true;
    if (!SetupStaticBlocks(p, mats, blank, &mats[0], false, 0, &already) || g_blank_calls != 1 || g_fetches != 0) return fail(1);

    if (!SetupStaticBlocks(p, mats, st, &mats[0], false, 10, &already) || !already || g_fetches != 0) return fail(2);

    if (!SetupStaticBlocks(p, mats, st, &mats[0], true, 10, &already) || already) return fail(3);
    if (p.blocks[0].diffuse != &g_tex[1] || p.blocks[0].bump != &g_tex[2]) return fail(4);
    if (p.blocks[1].diffuse != &g_tex[3] || p.blocks[1].bump != 0) return fail(5);
    if (p.blocks[2].diffuse != 0 || p.blocks[2].bump != &g_tex[2]) return fail(6);
    if (p.blocks[3].diffuse != &g_tex[3] || p.blocks[3].bump != &g_tex[4]) return fail(7);

    Stages nodiffuse = st; nodiffuse.diffuse_stage = -1;
    if (SetupStaticBlocks(p, mats, nodiffuse, &mats[0], true, 10, &already)) return fail(8);

    p.blocks[1].material = 2;
    if (SetupStaticBlocks(p, mats, st, &mats[0], true, 11, &already)) return fail(9);

    std::printf("PASS_00ba50a0\n");
    return 0;
}