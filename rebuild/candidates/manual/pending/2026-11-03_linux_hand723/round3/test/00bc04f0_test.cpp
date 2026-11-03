// Behaviour test for CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite (0x00BC04F0): the pass / flag decisions (blend
// op, blend mode and premultiply, displacement vs point-light shader family, the light-count clamp), the colour premultiply and the
// GFFastSin / GFFastCos table lookup, through a copy of the row's logic.
#include <cstdio>
#include <cmath>

enum Shader { FACE_ME, CROSSED, BILLBOARD };
enum { BLEND_OP_ADD, BLEND_OP_SUBTRACT, BLEND_OP_REV_SUBTRACT };
enum { MODE_ONE = 1, MODE_INV_SRC_COLOUR = 3, MODE_SRC_ALPHA = 4, MODE_INV_SRC_ALPHA = 5 };
struct Flags { bool Sprite3D, RotateAroundZAxis; };
struct Result { bool displacement, premultiply, cullNone; int blendOp, src, dest; Shader shader; };

static Result Decide(int pass, bool displacementEnabled, int blendOp, int blendMode, Flags f)
{
    Result r = { false, false, false, -1, -1, -1, BILLBOARD };
    if ((pass == 6 && displacementEnabled) || pass == 7) {
        r.displacement = true;
        if (pass == 6) { r.blendOp = BLEND_OP_ADD; r.src = MODE_SRC_ALPHA; r.dest = MODE_INV_SRC_ALPHA; }
    } else {
        r.blendOp = blendOp == 1 ? BLEND_OP_SUBTRACT : blendOp == 2 ? BLEND_OP_REV_SUBTRACT : BLEND_OP_ADD;
        if (blendMode == 3) { r.src = MODE_ONE; r.dest = MODE_ONE; r.premultiply = true; }
        else if (blendMode == 4) { r.src = MODE_ONE; r.dest = MODE_INV_SRC_COLOUR; r.premultiply = true; }
        else { r.src = MODE_SRC_ALPHA; r.dest = MODE_INV_SRC_ALPHA; }
    }
    if (f.Sprite3D) r.shader = FACE_ME;
    else if (f.RotateAroundZAxis) { r.shader = CROSSED; r.cullNone = true; }
    return r;
}

static unsigned long ClampIndex(unsigned long lights, unsigned long count)
{
    unsigned long index = lights;
    if (lights > count - 1)
        index = count - 1;
    return index;
}

static float g_CosTable[1025];
static long Near(float v) { return (long)std::floor(v + 0.5f); }   // fistp under round-to-nearest (no ties in these inputs)
static float FastCos(float a) { a *= 1024.0f; long i = Near(a - 0.5f); float f = a - (float)i; i &= 0x3ff;
                                return (1.0f - f) * g_CosTable[i] + f * g_CosTable[i + 1]; }
static float FastSin(float a) { a *= 1024.0f; long i = Near(a - 0.5f); float f = a - (float)i; i = (i - 256) & 0x3ff;
                                return (1.0f - f) * g_CosTable[i] + f * g_CosTable[i + 1]; }

int main()
{
    for (int k = 0; k <= 1024; ++k) g_CosTable[k] = (float)std::cos(6.283185307179586 * k / 1024.0);
    bool ok = true;
    Flags none = { false, false }, crossed = { false, true }, faceMe = { true, true };
    Result r = Decide(6, true, 2, 3, crossed);
    ok = ok && r.displacement && !r.premultiply && r.blendOp == BLEND_OP_ADD && r.shader == CROSSED && r.cullNone;
    r = Decide(6, false, 2, 3, none);                            // displacement off: the default pass
    ok = ok && !r.displacement && r.premultiply && r.blendOp == BLEND_OP_REV_SUBTRACT && r.src == MODE_ONE && r.dest == MODE_ONE;
    r = Decide(7, false, 0, 0, faceMe);
    ok = ok && r.displacement && r.blendOp == -1 && r.shader == FACE_ME;
    r = Decide(0, false, 1, 4, none);
    ok = ok && r.blendOp == BLEND_OP_SUBTRACT && r.dest == MODE_INV_SRC_COLOUR && r.premultiply && r.shader == BILLBOARD;
    r = Decide(0, false, 0, 0, none);
    ok = ok && r.src == MODE_SRC_ALPHA && r.dest == MODE_INV_SRC_ALPHA && !r.premultiply;
    ok = ok && ClampIndex(0, 5) == 0 && ClampIndex(3, 5) == 3 && ClampIndex(9, 5) == 4;
    // colour: bytes * 1/255, RGB premultiplied by alpha only when the blend mode asks for it
    float a = 128 * (1.0f / 255.0f), rr = 255 * (1.0f / 255.0f), scale = true ? a : 1.0f;
    ok = ok && std::fabs(rr * scale - a) < 1e-6f;
    for (float t = 0.0f; t < 1.0f; t += 0.0371f) {
        ok = ok && std::fabs(FastCos(t) - (float)std::cos(6.283185307179586 * t)) < 1e-3f;
        ok = ok && std::fabs(FastSin(t) - (float)std::sin(6.283185307179586 * t)) < 1e-3f;
    }
    if (!ok) { std::printf("00bc04f0_TEST FAIL\n"); return 1; }
    std::printf("00bc04f0_TEST PASS\n");
    return 0;
}
