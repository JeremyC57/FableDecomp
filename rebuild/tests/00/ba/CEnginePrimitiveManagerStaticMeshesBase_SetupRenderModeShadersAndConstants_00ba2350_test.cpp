// Behaviour test for 0x00BA2350 CEnginePrimitiveManagerStaticMeshesBase::SetupRenderModeShadersAndConstants: a host model of the row's
// decisions checked against hand-written expectations: the tint flag (tint RGB not white; the alpha byte is masked off); the combined
// pass' render mode (render flag 8 forces the stipple-alpha mode 5, whose alpha is StippleAlphaSolidFactor * alpha clamped to 255 for an
// opaque material and left alone for a transparent one) and the texture-factor alpha; the shadow pixel shader of modes 9 / 11 / 12 (the
// output-factor variant only when tinted); the shader flags per mode (0x109 default, 0x129 env modes 6 / 7 / 17, 0x10b shadow modes
// 9 / 11 / 12, 0x12b env-shadow modes 10 / 13) written to *shader_flags for every pass; *render_degenerate_triangles set by the five
// outline passes and the final else arm (every other pass but the Z write); the result is always true.
#include <cstdio>

enum EPass { Z_WRITE = 1, COMBINED = 2, OUTLINE_SMOOTH_STENCIL = 8, OUTLINE_SMOOTH_ALPHA_STENCIL_MASK = 9, OUTLINE_SMOOTH_FILL = 10,
             OUTLINE_STENCIL = 12, OUTLINE_FILL = 13 };   // engine/CEngineRenderEnums.h ERenderPassType
enum { PS_NONE, PS_FOG_SHADOW, PS_FOG_SHADOW_FACTOR, PS_DP3_FOG_SHADOW, PS_DP3_FOG_SHADOW_FACTOR };

struct SIn { int pass; long render_mode; unsigned long render_flags; unsigned char alpha; bool transparent; long stipple_factor;
             unsigned long tint; };
struct SOut { bool result; long mode; unsigned char factor_alpha; int shadow_ps; unsigned long flags; bool degenerate; };

static SOut Model(const SIn& in)
{
    SOut o; o.mode = -1; o.factor_alpha = 0; o.shadow_ps = PS_NONE; o.degenerate = false;
    bool tinted = (in.tint & 0xffffff) != 0xffffff;
    unsigned long flags = 0x109;
    if (in.pass == COMBINED) {
        long mode = in.render_mode;
        unsigned char alpha;
        if (in.render_flags & 8) {
            mode = 5;
            alpha = in.alpha;
            if (!in.transparent) {
                long solid = in.stipple_factor * (long)alpha;
                alpha = solid >= 255 ? 255 : (unsigned char)solid;
            }
        } else
            alpha = in.alpha;
        o.factor_alpha = alpha; o.mode = mode;
        switch (mode) {
        case 6: case 7: case 17: flags = 0x129; break;
        case 9: case 12: o.shadow_ps = tinted ? PS_FOG_SHADOW_FACTOR : PS_FOG_SHADOW; flags = 0x10b; break;
        case 11: o.shadow_ps = tinted ? PS_DP3_FOG_SHADOW_FACTOR : PS_DP3_FOG_SHADOW; flags = 0x10b; break;
        case 10: case 13: flags = 0x12b; break;
        default: break;
        }
    } else if (in.pass != Z_WRITE)
        o.degenerate = true;   // the three smooth outline passes, the stencil pass and the final `else` (outline fill) arm
    o.flags = flags; o.result = true;
    return o;
}

struct SCase { SIn in; SOut expect; };
static const SCase cases[] = {
    // pass, mode, rflags, alpha, transp, factor, tint      -> result, mode, falpha, shadow ps, flags, degenerate
    { { COMBINED, 9, 0, 0x80, false, 2, 0xffffffff },          { true, 9, 0x80, PS_FOG_SHADOW, 0x10b, false } },
    { { COMBINED, 9, 0, 0x80, false, 2, 0x00ffffff },          { true, 9, 0x80, PS_FOG_SHADOW, 0x10b, false } },
    { { COMBINED, 9, 0, 0x80, false, 2, 0xfffeffff },          { true, 9, 0x80, PS_FOG_SHADOW_FACTOR, 0x10b, false } },
    { { COMBINED, 12, 0, 0x10, true, 2, 0xff00ff00 },          { true, 12, 0x10, PS_FOG_SHADOW_FACTOR, 0x10b, false } },
    { { COMBINED, 11, 0, 0x10, true, 2, 0x80ffffff },          { true, 11, 0x10, PS_DP3_FOG_SHADOW, 0x10b, false } },
    { { COMBINED, 11, 0, 0x10, true, 2, 0x80fffffe },          { true, 11, 0x10, PS_DP3_FOG_SHADOW_FACTOR, 0x10b, false } },
    { { COMBINED, 9, 8, 100, false, 3, 0xffffffff },           { true, 5, 255, PS_NONE, 0x109, false } },
    { { COMBINED, 9, 8, 100, false, 2, 0xffffffff },           { true, 5, 200, PS_NONE, 0x109, false } },
    { { COMBINED, 9, 8, 100, true, 3, 0xffffffff },            { true, 5, 100, PS_NONE, 0x109, false } },
    { { COMBINED, 9, 8, 85, false, 3, 0xffffffff },            { true, 5, 255, PS_NONE, 0x109, false } },
    { { COMBINED, 6, 0, 1, false, 1, 0 },                      { true, 6, 1, PS_NONE, 0x129, false } },
    { { COMBINED, 17, 0, 1, false, 1, 0 },                     { true, 17, 1, PS_NONE, 0x129, false } },
    { { COMBINED, 10, 0, 1, false, 1, 0 },                     { true, 10, 1, PS_NONE, 0x12b, false } },
    { { COMBINED, 13, 0, 1, false, 1, 0 },                     { true, 13, 1, PS_NONE, 0x12b, false } },
    { { COMBINED, 2, 0, 1, false, 1, 0 },                      { true, 2, 1, PS_NONE, 0x109, false } },
    { { Z_WRITE, 9, 8, 1, false, 1, 0 },                       { true, -1, 0, PS_NONE, 0x109, false } },
    { { OUTLINE_SMOOTH_STENCIL, 9, 0, 1, false, 1, 0 },        { true, -1, 0, PS_NONE, 0x109, true } },
    { { OUTLINE_FILL, 9, 0, 1, false, 1, 0 },                  { true, -1, 0, PS_NONE, 0x109, true } },
    { { OUTLINE_SMOOTH_ALPHA_STENCIL_MASK, 9, 0, 1, false, 1, 0 }, { true, -1, 0, PS_NONE, 0x109, true } },
    { { OUTLINE_STENCIL, 9, 0, 1, false, 1, 0 },               { true, -1, 0, PS_NONE, 0x109, true } },
    { { 4, 9, 0, 1, false, 1, 0 },                             { true, -1, 0, PS_NONE, 0x109, true } },
};

int main()
{
    int bad = 0;
    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        SOut o = Model(cases[i].in); const SOut& e = cases[i].expect;
        if (o.result != e.result || o.mode != e.mode || o.factor_alpha != e.factor_alpha || o.shadow_ps != e.shadow_ps ||
            o.flags != e.flags || o.degenerate != e.degenerate) {
            printf("case %u: mode %ld alpha %u ps %d flags %lx degenerate %d\n", i, o.mode, o.factor_alpha, o.shadow_ps, o.flags, o.degenerate);
            ++bad;
        }
    }
    if (bad == 0) printf("00ba2350_TEST PASS\n");
    return bad;
}