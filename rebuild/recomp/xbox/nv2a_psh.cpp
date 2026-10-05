// NV2A register combiners -> GLSL fragment code.
//
// Up to 8 general combiner stages, each with RGB and alpha halves: inputs A..D (register,
// channel, mapping) give AB and CD products (or dot products) and AB+CD / mux(AB,CD), with an
// output scale/bias, written to registers. The final combiner computes
// rgb = A*B + (1-A)*C + D (with E*F and V1+R0 as extra inputs) and alpha = G.
// Texture stages are sampled first according to the per-stage shader mode.
#include "nv2a_shaders.hpp"
#include "nv2a_methods.h"

#include <string>

namespace xb::gpu {

namespace {
struct In { uint32_t reg, alphaChan, map; };
In parseIn(uint32_t v) { return {v & 0xF, (v >> 4) & 1, v & 0xE0}; }

struct Ctx {
    int stage = 0;
    uint32_t flags = 0;   // combiner-count flags (mux msb, unique c0/c1)
    std::string e, f;     // final combiner E and F (for EF_PROD)
    bool clampSum = false, invV1 = false, invR0 = false;
};

std::string reg(Ctx& c, uint32_t r, bool dest) {
    switch (r) {
    case 0x0: return dest ? "" : "vec4(0.0)";
    case 0x1: return (c.flags & 0x10) || c.stage == 8 ? "cf.c0[" + std::to_string(c.stage) + "]" : "cf.c0[0]";
    case 0x2: return (c.flags & 0x100) || c.stage == 8 ? "cf.c1[" + std::to_string(c.stage) + "]" : "cf.c1[0]";
    case 0x3: return "pFog";
    case 0x4: return "v0";
    case 0x5: return "v1";
    case 0x8: return "t0";
    case 0x9: return "t1";
    case 0xA: return "t2";
    case 0xB: return "t3";
    case 0xC: return "r0";
    case 0xD: return "r1";
    case 0xE: {
        const std::string v1 = c.invV1 ? "(1.0 - v1)" : "v1", r0 = c.invR0 ? "(1.0 - r0)" : "r0";
        const std::string s = "vec4(" + v1 + ".rgb + " + r0 + ".rgb, 0.0)";
        return c.clampSum ? "clamp(" + s + ", 0.0, 1.0)" : s;
    }
    case 0xF: return "vec4(" + c.e + " * " + c.f + ", 0.0)";
    default: return "vec4(0.0)";
    }
}

std::string input(Ctx& c, In in, bool alpha) {
    std::string r = reg(c, in.reg, false);
    if (!alpha) r += in.alphaChan ? ".aaa" : ".rgb";
    else r += in.alphaChan ? ".a" : ".b";
    switch (in.map) {
    case 0x00: return "max(" + r + ", 0.0)";
    case 0x20: return "(1.0 - clamp(" + r + ", 0.0, 1.0))";
    case 0x40: return "(2.0 * max(" + r + ", 0.0) - 1.0)";
    case 0x60: return "(-2.0 * max(" + r + ", 0.0) + 1.0)";
    case 0x80: return "(max(" + r + ", 0.0) - 0.5)";
    case 0xA0: return "(-max(" + r + ", 0.0) + 0.5)";
    case 0xC0: return r;
    default: return "(-" + r + ")";
    }
}

std::string outMap(const std::string& x, uint32_t mapping) {
    switch (mapping) {
    case 0x08: return "(" + x + " - 0.5)";
    case 0x10: return "(" + x + " * 2.0)";
    case 0x18: return "((" + x + " - 0.5) * 2.0)";
    case 0x20: return "(" + x + " * 4.0)";
    case 0x30: return "(" + x + " / 2.0)";
    default: return x;
    }
}

// One half (RGB or alpha) of a general combiner stage.
void stageHalf(Ctx& c, std::string& code, uint32_t inputs, uint32_t outputs, bool alpha) {
    const In a = parseIn(inputs >> 24), b = parseIn(inputs >> 16), cc = parseIn(inputs >> 8), d = parseIn(inputs);
    const std::string A = input(c, a, alpha), B = input(c, b, alpha), C = input(c, cc, alpha), D = input(c, d, alpha);
    const uint32_t cdDst = outputs & 0xF, abDst = (outputs >> 4) & 0xF, sumDst = (outputs >> 8) & 0xF, fl = outputs >> 12;
    const bool cdDot = fl & 1, abDot = fl & 2, mux = fl & 4;
    const uint32_t mapping = fl & 0x38;
    const std::string mask = alpha ? "a" : "rgb", type = alpha ? "float" : "vec3";
    const std::string ab = abDot && !alpha ? "vec3(dot(" + A + ", " + B + "))" : "(" + A + " * " + B + ")";
    const std::string cd = cdDot && !alpha ? "vec3(dot(" + C + ", " + D + "))" : "(" + C + " * " + D + ")";
    const std::string tag = std::to_string(c.stage) + (alpha ? "a" : "c");
    code += "  { " + type + " ab = " + ab + ", cd = " + cd + ";\n";
    std::string sum;
    if (mux) {
        const std::string sel = (c.flags & 1) ? "r0.a >= 0.5" : "(uint(r0.a * 255.0) & 1u) == 1u";
        sum = "((" + sel + ") ? cd : ab)";
    } else {
        sum = "(ab + cd)";
    }
    code += "    " + type + " oab = clamp(" + outMap("ab", mapping) + ", -1.0, 1.0), ocd = clamp(" + outMap("cd", mapping) +
            ", -1.0, 1.0), osum = clamp(" + outMap(sum, mapping) + ", -1.0, 1.0);\n";
    // Writes happen after all reads of the stage half.
    const std::string dAb = reg(c, abDst, true), dCd = reg(c, cdDst, true), dSum = reg(c, sumDst, true);
    if (!dAb.empty()) {
        code += "    n" + dAb + "." + mask + " = oab;\n";
        if (!alpha && (fl & 0x80)) code += "    n" + dAb + ".a = oab.b;\n";
    }
    if (!dCd.empty()) {
        code += "    n" + dCd + "." + mask + " = ocd;\n";
        if (!alpha && (fl & 0x40)) code += "    n" + dCd + ".a = ocd.b;\n";
    }
    if (!dSum.empty()) code += "    n" + dSum + "." + mask + " = osum;\n";
    code += "  }\n";
    (void)tag;
}
}  // namespace

std::string translateCombiners(const State& s, CombinerInfo* info, uint32_t shadowMask) {
    const uint32_t* R = s.regs;
    Ctx c;
    const uint32_t control = R[NV097_SET_COMBINER_CONTROL / 4];
    const int stages = static_cast<int>(control & 0xFF);
    c.flags = control >> 8;
    std::string code;
    // Texture stages.
    const uint32_t prog = R[NV097_SET_SHADER_STAGE_PROGRAM / 4];
    for (int i = 0; i < 4; ++i) {
        const uint32_t mode = (prog >> (5 * i)) & 0x1F;
        const std::string t = "t" + std::to_string(i), tc = "vTex" + std::to_string(i), smp = "tex" + std::to_string(i);
        if (info) info->texMode[i] = mode;
        if (((shadowMask >> i) & 1) && (mode == 1 || mode == 2)) {
            // Shadow map (as xemu): the depth (24-bit) against z/w, with the shadow depth func.
            static const char* kCmp[8] = {"", "<", "==", "<=", ">", "!=", ">=", ""};
            const uint32_t func = R[NV097_SET_SHADOW_DEPTH_FUNC / 4] & 7;
            if (func == 0 || func == 7) {
                code += "  vec4 " + t + " = vec4(" + (func ? "1.0" : "0.0") + ");\n";
            } else {
                code += "  float " + t + "d = textureProj(" + smp + ", " + tc + ".xyw).r * 16777215.0;\n";
                const std::string z = mode == 2 ? "clamp(" + tc + ".z / " + tc + ".w, 0.0, 16777215.0)" : "0.0";
                code += "  vec4 " + t + " = vec4(" + t + "d " + kCmp[func] + " " + z + " ? 1.0 : 0.0);\n";
            }
            continue;
        }
        switch (mode) {
        case 0: code += "  vec4 " + t + " = vec4(0.0);\n"; break;
        case 1: code += "  vec4 " + t + " = textureProj(" + smp + ", " + tc + ".xyw);\n"; break;
        case 2: code += "  vec4 " + t + " = texture(" + smp + "3d, " + tc + ".xyz / " + tc + ".w);\n"; break;
        case 3: code += "  vec4 " + t + " = texture(" + smp + "cube, " + tc + ".xyz);\n"; break;
        case 4: code += "  vec4 " + t + " = clamp(" + tc + ", 0.0, 1.0);\n"; break;
        case 5:  // clip plane: discard on negative texcoord components
            code += "  vec4 " + t + " = vec4(0.0);\n  if (any(lessThan(" + tc + ", vec4(0.0)))) discard;\n";
            break;
        case 6: case 7: {  // bump environment map from the previous stage
            const std::string prev = "t" + std::to_string(i - 1), m = "bump[" + std::to_string(i) + "]";
            code += "  vec2 dsdt" + std::to_string(i) + " = mat2(" + m + ".xy, " + m + ".zw) * (" + prev + ".rg * 2.0 - 1.0);\n";
            code += "  vec4 " + t + " = texture(" + smp + ", " + tc + ".xy / " + tc + ".w + dsdt" + std::to_string(i) + ");\n";
            if (mode == 7) code += "  " + t + ".rgb *= clamp(" + prev + ".b * bumpLum[" + std::to_string(i) + "].x + bumpLum[" +
                                   std::to_string(i) + "].y, 0.0, 1.0);\n";
            break;
        }
        default:  // dot-product modes: approximate with a plain 2D lookup for now
            code += "  vec4 " + t + " = texture(" + smp + ", " + tc + ".xy);\n";
            break;
        }
    }
    code += "  vec4 r0 = vec4(0.0, 0.0, 0.0, t0.a), r1 = vec4(0.0);\n";
    code += "  vec4 nr0, nr1, nt0, nt1, nt2, nt3, nv0, nv1;\n";
    for (int i = 0; i < stages && i < 8; ++i) {
        c.stage = i;
        code += "  nr0 = r0; nr1 = r1; nt0 = t0; nt1 = t1; nt2 = t2; nt3 = t3; nv0 = v0; nv1 = v1;\n";
        stageHalf(c, code, R[NV097_SET_COMBINER_COLOR_ICW / 4 + i], R[NV097_SET_COMBINER_COLOR_OCW / 4 + i], false);
        stageHalf(c, code, R[NV097_SET_COMBINER_ALPHA_ICW / 4 + i], R[NV097_SET_COMBINER_ALPHA_OCW / 4 + i], true);
        code += "  r0 = nr0; r1 = nr1; t0 = nt0; t1 = nt1; t2 = nt2; t3 = nt3; v0 = nv0; v1 = nv1;\n";
    }
    const uint32_t f0 = R[NV097_SET_COMBINER_SPECULAR_FOG_CW0 / 4], f1 = R[NV097_SET_COMBINER_SPECULAR_FOG_CW1 / 4];
    if (f0 || f1) {
        c.stage = 8;
        c.clampSum = f1 & 0x80;
        c.invV1 = f1 & 0x40;
        c.invR0 = f1 & 0x20;
        c.e = input(c, parseIn(f1 >> 24), false);
        c.f = input(c, parseIn(f1 >> 16), false);
        const std::string A = input(c, parseIn(f0 >> 24), false), B = input(c, parseIn(f0 >> 16), false);
        const std::string C = input(c, parseIn(f0 >> 8), false), D = input(c, parseIn(f0), false);
        const std::string G = input(c, parseIn(f1 >> 8), true);
        code += "  fragColor.rgb = " + D + " + mix(vec3(" + C + "), vec3(" + B + "), vec3(" + A + "));\n";
        code += "  fragColor.a = " + G + ";\n";
    } else {
        code += "  fragColor = r0;\n";
    }
    return code;
}

} // namespace xb::gpu
