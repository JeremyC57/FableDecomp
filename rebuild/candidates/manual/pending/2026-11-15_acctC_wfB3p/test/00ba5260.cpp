// Behaviour test for 0x00BA5260 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive: a host model of the row's decisions with
// recording stand-ins for the engine, checked against hand-written expected call sequences: specular + fog on first; no diffuse map ->
// nothing else; composite surfaces only for a morphed static mesh (type 10); alpha states always, the stencil mask only for render flag
// 0x80; a failing SetupRenderModeShadersAndConstants or SetupStaticBlocks restores the default primitive parameters and flushes, nothing
// drawn; otherwise first textures, CCW culling, the occlusion test bracket only for a valid handle (!= -1), vertex buffer, world
// transform, realise, PrepareForRender(shader flags from the shader set-up), the block draw with its degenerate-triangle flag, flush,
// restore defaults.
#include <cstdio>
#include <cstring>

struct SInput {
    unsigned long diffuse_index; long type; unsigned long render_flags; bool shaders_ok, blocks_ok; unsigned long occlusion_handle;
    unsigned long shader_flags_out; bool degenerate_out;
};
static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }

static void Model(const SInput& in)
{
    g_log[0] = 0;
    Log("spec"); Log("fog");
    bool composite = (in.type == 10);
    if (in.diffuse_index == 0)
        return;
    Log(composite ? "mip(c)" : "mip");
    unsigned long shader_flags = 0;
    Log("alpha");
    if (in.render_flags & 0x80)
        Log("stencil");
    bool degenerate = false;
    if (!in.shaders_ok) { Log("restore"); Log("flush"); return; }
    shader_flags = in.shader_flags_out; degenerate = in.degenerate_out;
    if (!in.blocks_ok) { Log("restore"); Log("flush"); return; }
    Log("first"); Log("ccw"); Log("occprep");
    if (in.occlusion_handle != (unsigned long)-1) Log("occstart");
    Log("vb"); Log("world"); Log("realise");
    char b[32]; sprintf(b, "prep%lx", shader_flags); Log(b);
    Log(degenerate ? "blocks(d)" : "blocks");
    if (in.occlusion_handle != (unsigned long)-1) Log("occend");
    Log("flush"); Log("restore");
}

struct SCase { SInput in; const char* expect; };
static const SCase cases[] = {
    { { 0, 0, 0, true, true, 5, 0x109, false }, "spec fog " },
    { { 3, 10, 0, true, true, (unsigned long)-1, 0x109, false },
      "spec fog mip(c) alpha first ccw occprep vb world realise prep109 blocks flush restore " },
    { { 3, 0, 0x80, true, true, 7, 0x12b, true },
      "spec fog mip alpha stencil first ccw occprep occstart vb world realise prep12b blocks(d) occend flush restore " },
    { { 3, 0, 0x7f, false, true, 7, 0x109, false }, "spec fog mip alpha restore flush " },
    { { 3, 0, 0x80, true, false, 7, 0x109, false }, "spec fog mip alpha stencil restore flush " },
    { { 1, 10, 0x180, true, true, 0, 0x10b, false },
      "spec fog mip(c) alpha stencil first ccw occprep occstart vb world realise prep10b blocks occend flush restore " },
};

int main()
{
    int bad = 0;
    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        Model(cases[i].in);
        if (strcmp(g_log, cases[i].expect) != 0) { printf("case %u: got '%s'\n", i, g_log); ++bad; }
    }
    if (bad == 0) printf("00ba5260_TEST PASS\n");
    return bad;
}
