// NV2A vertex program -> GLSL (Vulkan) translator.
//
// An instruction is 4 words. Each carries a MAC (vector) op and an ILU (scalar/special) op
// over three inputs A, B, C, each an R (temp), V (attribute) or C (constant) register with a
// swizzle and negation. Results go to a temp register and/or an output register. Paired
// MAC+ILU instructions execute concurrently: the ILU result can only go to R1.
#include "nv2a_shaders.hpp"

#include <cstdlib>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <string>

namespace xb::gpu {

namespace {
struct Field { int word, shift, bits; };
// Bit fields of an instruction (word index, shift, width).
constexpr Field F_ILU{1, 25, 3}, F_MAC{1, 21, 4}, F_CONST{1, 13, 8}, F_V{1, 9, 4};
constexpr Field F_A_NEG{1, 8, 1}, F_A_SWZ{1, 0, 8}, F_A_R{2, 28, 4}, F_A_MUX{2, 26, 2};
constexpr Field F_B_NEG{2, 25, 1}, F_B_SWZ{2, 17, 8}, F_B_R{2, 13, 4}, F_B_MUX{2, 11, 2};
constexpr Field F_C_NEG{2, 10, 1}, F_C_SWZ{2, 2, 8}, F_C_R_HI{2, 0, 2}, F_C_R_LO{3, 30, 2}, F_C_MUX{3, 28, 2};
constexpr Field F_OUT_MAC_MASK{3, 24, 4}, F_OUT_R{3, 20, 4}, F_OUT_ILU_MASK{3, 16, 4}, F_OUT_O_MASK{3, 12, 4};
constexpr Field F_OUT_ORB{3, 11, 1}, F_OUT_ADDRESS{3, 3, 8}, F_OUT_MUX{3, 2, 1}, F_A0X{3, 1, 1}, F_FINAL{3, 0, 1};

uint32_t get(const uint32_t* t, Field f) { return (t[f.word] >> f.shift) & ((1u << f.bits) - 1); }

enum { MAC_NOP, MAC_MOV, MAC_MUL, MAC_ADD, MAC_MAD, MAC_DP3, MAC_DPH, MAC_DP4, MAC_DST, MAC_MIN, MAC_MAX, MAC_SLT, MAC_SGE, MAC_ARL };
enum { ILU_NOP, ILU_MOV, ILU_RCP, ILU_RCC, ILU_RSQ, ILU_EXP, ILU_LOG, ILU_LIT };

const char* kOut[16] = {"oPos", "oUnk1", "oUnk2", "oD0", "oD1", "oFog", "oPts", "oB0", "oB1", "oT0", "oT1", "oT2", "oT3", "oUnk13", "oUnk14", "oUnk15"};

std::string mask(uint32_t m) {  // bit 3 = x ... bit 0 = w
    std::string s;
    if (m & 8) s += 'x';
    if (m & 4) s += 'y';
    if (m & 2) s += 'z';
    if (m & 1) s += 'w';
    return s;
}

std::string swizzle(uint32_t swz) {
    static const char c[] = "xyzw";
    std::string s;
    s += c[(swz >> 6) & 3];
    s += c[(swz >> 4) & 3];
    s += c[(swz >> 2) & 3];
    s += c[swz & 3];
    return s;
}

std::string operand(const uint32_t* t, Field mux, Field neg, Field swz, uint32_t reg, bool scalar) {
    std::string base;
    switch (get(t, mux)) {
    case 1: base = "R" + std::to_string(reg); break;
    case 2: base = "v" + std::to_string(get(t, F_V)); break;
    case 3: {
        const uint32_t c = get(t, F_CONST);
        base = get(t, F_A0X) ? "C(A0 + " + std::to_string(c) + ")" : "C(" + std::to_string(c) + ")";
        break;
    }
    default: base = "vec4(0.0)"; break;
    }
    std::string sw = swizzle(get(t, swz));
    if (scalar) sw = std::string(4, sw[0]);  // scalar ILU ops read C.x of the swizzle
    std::string r = "(" + base + ")." + sw;
    return get(t, neg) ? "(-" + r + ")" : r;
}
}  // namespace

std::string translateVertexProgram(const uint32_t (*prog)[4], uint32_t start) {
    std::string body;
    char buf[160];
    for (uint32_t slot = start; slot < 136; ++slot) {
        const uint32_t* t = prog[slot];
        const uint32_t mac = get(t, F_MAC), ilu = get(t, F_ILU);
        const bool scalarIlu = ilu == ILU_RCP || ilu == ILU_RCC || ilu == ILU_RSQ || ilu == ILU_EXP || ilu == ILU_LOG;
        const std::string a = operand(t, F_A_MUX, F_A_NEG, F_A_SWZ, get(t, F_A_R), false);
        const std::string b = operand(t, F_B_MUX, F_B_NEG, F_B_SWZ, get(t, F_B_R), false);
        const std::string c = operand(t, F_C_MUX, F_C_NEG, F_C_SWZ, (get(t, F_C_R_HI) << 2) | get(t, F_C_R_LO), false);
        const std::string cs = operand(t, F_C_MUX, F_C_NEG, F_C_SWZ, (get(t, F_C_R_HI) << 2) | get(t, F_C_R_LO), scalarIlu);
        // Relative index, not the slot: identical programs at different slots give identical text.
        snprintf(buf, sizeof buf, "  // %u: %08X %08X %08X %08X\n", slot - start, t[0], t[1], t[2], t[3]);
        body += buf;
        // Both units read their inputs before either writes.
        std::string macExpr, iluExpr;
        switch (mac) {
        case MAC_MOV: macExpr = a; break;
        case MAC_MUL: macExpr = "vMUL(" + a + ", " + b + ")"; break;
        case MAC_ADD: macExpr = "(" + a + " + " + c + ")"; break;
        case MAC_MAD: macExpr = "(vMUL(" + a + ", " + b + ") + " + c + ")"; break;
        case MAC_DP3: macExpr = "vec4(dot((" + a + ").xyz, (" + b + ").xyz))"; break;
        case MAC_DPH: macExpr = "vec4(dot(vec4((" + a + ").xyz, 1.0), " + b + "))"; break;
        case MAC_DP4: macExpr = "vec4(dot(" + a + ", " + b + "))"; break;
        case MAC_DST: macExpr = "vDST(" + a + ", " + b + ")"; break;
        case MAC_MIN: macExpr = "min(" + a + ", " + b + ")"; break;
        case MAC_MAX: macExpr = "max(" + a + ", " + b + ")"; break;
        case MAC_SLT: macExpr = "vec4(lessThan(" + a + ", " + b + "))"; break;
        case MAC_SGE: macExpr = "vec4(greaterThanEqual(" + a + ", " + b + "))"; break;
        case MAC_ARL: macExpr = a; break;
        default: break;
        }
        switch (ilu) {
        case ILU_MOV: iluExpr = c; break;
        case ILU_RCP: iluExpr = "vRCP(" + cs + ")"; break;
        case ILU_RCC: iluExpr = "vRCC(" + cs + ")"; break;
        case ILU_RSQ: iluExpr = "vRSQ(" + cs + ")"; break;
        case ILU_EXP: iluExpr = "vEXP(" + cs + ")"; break;
        case ILU_LOG: iluExpr = "vLOG(" + cs + ")"; break;
        case ILU_LIT: iluExpr = "vLIT(" + c + ")"; break;
        default: break;
        }
        body += "  {\n";
        if (!macExpr.empty()) body += "    vec4 m = " + macExpr + ";\n";
        if (!iluExpr.empty()) body += "    vec4 i = " + iluExpr + ";\n";
        const uint32_t r = get(t, F_OUT_R);
        if (mac == MAC_ARL) {
            body += "    A0 = int(floor(m.x + 0.001));\n";
        } else if (!macExpr.empty()) {
            const uint32_t mm = get(t, F_OUT_MAC_MASK);
            // Paired with an ILU op, a MAC write to R1 is dropped (the ILU owns R1).
            if (mm && !(ilu != ILU_NOP && r == 1)) {
                const std::string ms = mask(mm);
                body += "    R" + std::to_string(r) + "." + ms + " = m." + ms + ";\n";
            }
        }
        if (!iluExpr.empty()) {
            const uint32_t im = get(t, F_OUT_ILU_MASK);
            if (im) {
                const std::string ms = mask(im);
                body += "    R" + std::to_string(mac != MAC_NOP ? 1u : r) + "." + ms + " = i." + ms + ";\n";
            }
        }
        const uint32_t om = get(t, F_OUT_O_MASK);
        if (om) {
            const std::string src = get(t, F_OUT_MUX) ? "i" : "m";
            if (get(t, F_OUT_ORB) == 0) {  // write to a constant register (c[addr])
                body += "    // writeable constant c[" + std::to_string(get(t, F_OUT_ADDRESS)) + "] ignored\n";
            } else if (!(get(t, F_OUT_MUX) ? iluExpr : macExpr).empty()) {
                const uint32_t o = get(t, F_OUT_ADDRESS) & 0xF;
                const std::string ms = mask(om);
                if (o == 5) {  // oFog: a write lands in x; which result component is under test
                    static const bool first = getenv("FABLE_OFOG_FIRST") != nullptr;
                    body += "    oFog.x = " + src + "." + (first ? ms.substr(0, 1) : std::string("x")) + ";\n";
                } else {
                    body += "    " + std::string(kOut[o]) + "." + ms + " = " + src + "." + ms + ";\n";
                }
            }
        }
        body += "  }\n";
        if (get(t, F_FINAL)) break;
    }
    return body;
}

void evalVertexPosition(const uint32_t (*prog)[4], uint32_t start, const float (*cst)[4], const float (*v)[4], float oPos[4]) {
    struct V { float x[4]; };
    V R[13] = {};  // R12 = oPos
    V out[16] = {};
    out[0].x[3] = 1.0f;  // oPos starts as (0, 0, 0, 1), as in the GLSL
    int a0 = 0;
    auto C = [&](int i) { V r{}; if (i >= 0 && i < 192) std::memcpy(r.x, cst[i], 16); return r; };
    auto src = [&](const uint32_t* t, Field mux, Field neg, Field swz, uint32_t reg, bool scalar) {
        V b{};
        switch (get(t, mux)) {
        case 1: b = reg == 12 ? out[0] : R[reg < 13 ? reg : 0]; break;
        case 2: std::memcpy(b.x, v[get(t, F_V)], 16); break;
        case 3: b = C(static_cast<int>(get(t, F_CONST)) + (get(t, F_A0X) ? a0 : 0)); break;
        default: break;
        }
        const uint32_t s = get(t, swz);
        V r;
        for (int k = 0; k < 4; ++k) r.x[k] = b.x[(s >> (6 - 2 * (scalar ? 0 : k))) & 3];
        if (get(t, neg)) for (float& f : r.x) f = -f;
        return r;
    };
    auto mul = [](const V& a, const V& b) { V r; for (int k = 0; k < 4; ++k) r.x[k] = (a.x[k] == 0.0f || b.x[k] == 0.0f) ? 0.0f : a.x[k] * b.x[k]; return r; };
    auto splat = [](float f) { V r; for (float& x : r.x) x = f; return r; };
    for (uint32_t slot = start; slot < 136; ++slot) {
        const uint32_t* t = prog[slot];
        const uint32_t mac = get(t, F_MAC), ilu = get(t, F_ILU);
        const bool scalarIlu = ilu == ILU_RCP || ilu == ILU_RCC || ilu == ILU_RSQ || ilu == ILU_EXP || ilu == ILU_LOG;
        const uint32_t cr = (get(t, F_C_R_HI) << 2) | get(t, F_C_R_LO);
        const V a = src(t, F_A_MUX, F_A_NEG, F_A_SWZ, get(t, F_A_R), false), b = src(t, F_B_MUX, F_B_NEG, F_B_SWZ, get(t, F_B_R), false);
        const V c = src(t, F_C_MUX, F_C_NEG, F_C_SWZ, cr, false), cs = src(t, F_C_MUX, F_C_NEG, F_C_SWZ, cr, scalarIlu);
        V m{}, i{};
        bool hasM = true, hasI = true;
        switch (mac) {
        case MAC_MOV: case MAC_ARL: m = a; break;
        case MAC_MUL: m = mul(a, b); break;
        case MAC_ADD: for (int k = 0; k < 4; ++k) m.x[k] = a.x[k] + c.x[k]; break;
        case MAC_MAD: m = mul(a, b); for (int k = 0; k < 4; ++k) m.x[k] += c.x[k]; break;
        case MAC_DP3: m = splat(a.x[0] * b.x[0] + a.x[1] * b.x[1] + a.x[2] * b.x[2]); break;
        case MAC_DPH: m = splat(a.x[0] * b.x[0] + a.x[1] * b.x[1] + a.x[2] * b.x[2] + b.x[3]); break;
        case MAC_DP4: m = splat(a.x[0] * b.x[0] + a.x[1] * b.x[1] + a.x[2] * b.x[2] + a.x[3] * b.x[3]); break;
        case MAC_DST: m = {{1.0f, a.x[1] * b.x[1], a.x[2], b.x[3]}}; break;
        case MAC_MIN: for (int k = 0; k < 4; ++k) m.x[k] = std::min(a.x[k], b.x[k]); break;
        case MAC_MAX: for (int k = 0; k < 4; ++k) m.x[k] = std::max(a.x[k], b.x[k]); break;
        case MAC_SLT: for (int k = 0; k < 4; ++k) m.x[k] = a.x[k] < b.x[k] ? 1.0f : 0.0f; break;
        case MAC_SGE: for (int k = 0; k < 4; ++k) m.x[k] = a.x[k] >= b.x[k] ? 1.0f : 0.0f; break;
        default: hasM = false; break;
        }
        const float x = cs.x[0];
        switch (ilu) {
        case ILU_MOV: i = c; break;
        case ILU_RCP: i = splat(x == 0.0f ? INFINITY : 1.0f / x); break;
        case ILU_RCC: {
            float r = 1.0f / x;
            r = r >= 0.0f ? std::clamp(r, 5.42101e-20f, 1.84467e19f) : std::clamp(r, -1.84467e19f, -5.42101e-20f);
            i = splat(r);
            break;
        }
        case ILU_RSQ: i = splat(std::fabs(x) == 0.0f ? INFINITY : 1.0f / std::sqrt(std::fabs(x))); break;
        case ILU_EXP: { const float f = std::floor(x); i = {{std::exp2(f), x - f, std::exp2(x), 1.0f}}; break; }
        case ILU_LOG: {
            const float ax = std::fabs(x);
            if (ax == 0.0f) i = {{-INFINITY, 1.0f, -INFINITY, 1.0f}};
            else { const float e = std::floor(std::log2(ax)); i = {{e, ax / std::exp2(e), std::log2(ax), 1.0f}}; }
            break;
        }
        case ILU_LIT: {
            const float d = std::max(c.x[0], 0.0f), s = std::max(c.x[1], 0.0f), p = std::clamp(c.x[3], -127.9961f, 127.9961f);
            i = {{1.0f, d, c.x[0] > 0.0f ? std::pow(s, p) : 0.0f, 1.0f}};
            break;
        }
        default: hasI = false; break;
        }
        const uint32_t r = get(t, F_OUT_R);
        auto writeMasked = [](V& dst, const V& val, uint32_t msk) { for (int k = 0; k < 4; ++k) if (msk & (8u >> k)) dst.x[k] = val.x[k]; };
        auto reg = [&](uint32_t n) -> V& { return n == 12 ? out[0] : R[n < 13 ? n : 0]; };
        if (mac == MAC_ARL) {
            a0 = static_cast<int>(std::floor(m.x[0] + 0.001f));
        } else if (hasM) {
            const uint32_t mm = get(t, F_OUT_MAC_MASK);
            if (mm && !(ilu != ILU_NOP && r == 1)) writeMasked(reg(r), m, mm);
        }
        if (hasI) {
            const uint32_t im = get(t, F_OUT_ILU_MASK);
            if (im) writeMasked(reg(mac != MAC_NOP ? 1u : r), i, im);
        }
        const uint32_t om = get(t, F_OUT_O_MASK);
        if (om && get(t, F_OUT_ORB) && (get(t, F_OUT_MUX) ? hasI : hasM)) writeMasked(out[get(t, F_OUT_ADDRESS) & 0xF], get(t, F_OUT_MUX) ? i : m, om);
        if (get(t, F_FINAL)) break;
    }
    std::memcpy(oPos, out[0].x, 16);
}

// Helper functions with the hardware's special cases (anything * 0 = 0, 1/0 = inf...).
const char* vertexProgramPrelude() {
    return R"(
vec4 R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11;
#define R12 oPos
int A0 = 0;
vec4 C(int i) { return (i >= 0 && i < 192) ? cst.c[i] : vec4(0.0); }
vec4 vMUL(vec4 a, vec4 b) {
    vec4 r = a * b;
    bvec4 z = bvec4(a.x == 0.0 || b.x == 0.0, a.y == 0.0 || b.y == 0.0, a.z == 0.0 || b.z == 0.0, a.w == 0.0 || b.w == 0.0);
    return mix(r, vec4(0.0), z);
}
vec4 vDST(vec4 a, vec4 b) { return vec4(1.0, a.y * b.y, a.z, b.w); }
vec4 vRCP(vec4 c) { return vec4(c.x == 0.0 ? 1.0 / 0.0 : 1.0 / c.x); }
vec4 vRCC(vec4 c) {
    float r = 1.0 / c.x;
    r = r >= 0.0 ? clamp(r, 5.42101e-20, 1.84467e19) : clamp(r, -1.84467e19, -5.42101e-20);
    return vec4(r);
}
vec4 vRSQ(vec4 c) { float x = abs(c.x); return vec4(x == 0.0 ? 1.0 / 0.0 : inversesqrt(x)); }
vec4 vEXP(vec4 c) { float f = floor(c.x); return vec4(exp2(f), c.x - f, exp2(c.x), 1.0); }
vec4 vLOG(vec4 c) {
    float x = abs(c.x);
    if (x == 0.0) return vec4(-1.0 / 0.0, 1.0, -1.0 / 0.0, 1.0);
    float e = floor(log2(x));
    return vec4(e, x / exp2(e), log2(x), 1.0);
}
vec4 vLIT(vec4 c) {
    float d = max(c.x, 0.0), s = max(c.y, 0.0), p = clamp(c.w, -127.9961, 127.9961);
    return vec4(1.0, d, (c.x > 0.0) ? pow(s, p) : 0.0, 1.0);
}
)";
}

} // namespace xb::gpu
