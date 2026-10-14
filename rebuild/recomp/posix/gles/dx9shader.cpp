#include "dx9shader.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>

namespace host::gles {

const char* kVertexPrologue =
    "#version 300 es\n"
    "precision highp float;\nprecision highp int;\n"
    "uniform vec4 c[128];\nuniform int uColorBgra;\nuniform vec2 uHalfPixel;\n"
    "out vec4 vColor0;\nout vec4 vColor1;\nout vec4 vTex0;\nout vec4 vTex1;\nout vec4 vTex2;\nout vec4 vTex3;\n"
    "out vec4 vTex4;\nout vec4 vTex5;\nout vec4 vTex6;\nout vec4 vTex7;\nout float vFog;\n";

const char* kFragmentPrologue =
    "#version 300 es\n"
    "precision mediump float;\nprecision highp int;\n"
    "uniform sampler2D s0, s1, s2, s3, s4, s5;\n"
    "uniform vec4 pc[8];\nuniform vec4 uBump[6];\nuniform int uProjMask;\n"
    "uniform vec4 uFogColor;\nuniform int uFogEnable;\nuniform int uAlphaFunc;\nuniform float uAlphaRef;\n"
    "in vec4 vColor0;\nin vec4 vColor1;\nin vec4 vTex0;\nin vec4 vTex1;\nin vec4 vTex2;\nin vec4 vTex3;\n"
    "in vec4 vTex4;\nin vec4 vTex5;\nin vec4 vTex6;\nin vec4 vTex7;\nin float vFog;\n"
    "out vec4 fragColor;\n"
    "vec4 texStage(int s, vec4 tc) {\n"  // D3DTTFF_PROJECTED: divide by w
    "  vec2 uv = (uProjMask & (1 << s)) != 0 ? tc.xy / tc.w : tc.xy;\n"
    "  if (s == 0) return texture(s0, uv); if (s == 1) return texture(s1, uv); if (s == 2) return texture(s2, uv);\n"
    "  if (s == 3) return texture(s3, uv); if (s == 4) return texture(s4, uv); return texture(s5, uv);\n"
    "}\n"
    "vec4 texStageUV(int s, vec2 uv) {\n"
    "  if (s == 0) return texture(s0, uv); if (s == 1) return texture(s1, uv); if (s == 2) return texture(s2, uv);\n"
    "  if (s == 3) return texture(s3, uv); if (s == 4) return texture(s4, uv); return texture(s5, uv);\n"
    "}\n"
    "void finish(vec4 color) {\n"
    // Only the alpha-test variant (withAlphaTest) can discard: on a Mali GPU a shader that may
    // discard gets no early depth/stencil update and no forward pixel kill, whatever the uniform.
    "#ifdef FABLE_ALPHA_TEST\n"
    "  if (uAlphaFunc != 8) {\n"  // D3DCMP_*: 1 never 2 less 3 equal 4 lessequal 5 greater 6 notequal 7 greaterequal 8 always
    "    float a = color.a, r = uAlphaRef; bool pass = true;\n"
    "    if (uAlphaFunc == 1) pass = false; else if (uAlphaFunc == 2) pass = a < r; else if (uAlphaFunc == 3) pass = abs(a - r) < 0.002;\n"
    "    else if (uAlphaFunc == 4) pass = a <= r; else if (uAlphaFunc == 5) pass = a > r; else if (uAlphaFunc == 6) pass = abs(a - r) >= 0.002;\n"
    "    else if (uAlphaFunc == 7) pass = a >= r;\n"
    "    if (!pass) discard;\n"
    "  }\n"
    "#endif\n"
    "  if (uFogEnable != 0) color.rgb = mix(uFogColor.rgb, color.rgb, clamp(vFog, 0.0, 1.0));\n"
    "  fragColor = color;\n"
    "}\n";

namespace {

enum RegType { R_TEMP = 0, R_INPUT = 1, R_CONST = 2, R_ADDR = 3, R_RASTOUT = 4, R_ATTROUT = 5, R_TEXCRDOUT = 6, R_CONSTINT = 7, R_COLOROUT = 8, R_DEPTHOUT = 9, R_SAMPLER = 10 };

struct Param {
    uint32_t tok;
    int type() const { return ((tok >> 28) & 7) | ((tok >> 8) & 0x18); }
    int num() const { return tok & 0x7FF; }
    bool relative() const { return (tok >> 13) & 1; }
    int srcMod() const { return (tok >> 24) & 0xF; }
    int swizzle(int i) const { return (tok >> (16 + 2 * i)) & 3; }
    int writeMask() const { return (tok >> 16) & 0xF; }
    int dstMod() const { return (tok >> 20) & 0xF; }
    int shift() const { return (tok >> 24) & 0xF; }
};

struct Translator {
    bool pixel;
    int major, minor;
    bool ps14() const { return pixel && minor >= 4; }
    std::string body;
    std::string err;
    Dx9Shader* out;
    bool outWritten[16] = {};  // vertex outputs written (oPos, oD0, oD1, oT0-7, oFog, oPts)
    bool texReg[8] = {};       // ps_1_1: tN holds a sample (after tex) or coordinates
    std::string pad3x2;        // texm3x2pad result (u) awaiting texm3x2tex
    int phase = 0;
    bool fail(const std::string& m) { if (err.empty()) err = m; return false; }

    static const char* swz(int i) { return "xyzw" + i; }

    std::string regName(const Param& p, bool asDst) {
        const int t = p.type(), n = p.num();
        if (pixel) {
            switch (t) {
                case R_TEMP: return "r" + std::to_string(n);
                case R_INPUT: return "v" + std::to_string(n);
                case R_CONST: return "pc[" + std::to_string(n & 7) + "]";
                case R_ADDR: return "t" + std::to_string(n);  // texture register
                case R_COLOROUT: return "r0";
                default: break;
            }
        } else {
            switch (t) {
                case R_TEMP: return "r" + std::to_string(n);
                case R_INPUT: return "v" + std::to_string(n);
                case R_CONST:
                    if (p.relative()) return "c[clamp(int(a0) + " + std::to_string(n) + ", 0, 127)]";
                    return "c[" + std::to_string(n) + "]";
                case R_ADDR: return "a0";
                case R_RASTOUT: return n == 0 ? "oPos" : n == 1 ? "oFog" : "oPts";
                case R_ATTROUT: return "oD" + std::to_string(n);
                case R_TEXCRDOUT: return "oT" + std::to_string(n);
                default: break;
            }
        }
        fail("register type " + std::to_string(t));
        return "r0";
    }

    // A source operand as a vec4 expression with swizzle and modifier applied.
    std::string src(const Param& p) {
        std::string s = regName(p, false);
        if (!pixel && p.type() == R_ADDR) s = "vec4(a0, 0.0, 0.0, 0.0)";  // a0 as a source: rare
        char sw[6] = {'.', 0, 0, 0, 0, 0};
        for (int i = 0; i < 4; ++i) sw[1 + i] = "xyzw"[p.swizzle(i)];
        if (std::strcmp(sw, ".xyzw") != 0) s += sw;
        switch (p.srcMod()) {
            case 0: break;
            case 1: s = "(-" + s + ")"; break;
            case 2: s = "(" + s + " - 0.5)"; break;
            case 3: s = "(0.5 - " + s + ")"; break;
            case 4: s = "(" + s + " * 2.0 - 1.0)"; break;
            case 5: s = "(1.0 - " + s + " * 2.0)"; break;
            case 6: s = "(1.0 - " + s + ")"; break;
            case 7: s = "(" + s + " * 2.0)"; break;
            case 8: s = "(" + s + " * -2.0)"; break;
            case 9: s = "(" + s + " / " + s + ".z)"; break;   // _dz
            case 10: s = "(" + s + " / " + s + ".w)"; break;  // _dw
            case 11: s = "abs(" + s + ")"; break;
            case 12: s = "(-abs(" + s + "))"; break;
            default: fail("source modifier " + std::to_string(p.srcMod()));
        }
        return s;
    }
    // A source as a vec2 for texture coordinates (projective modifiers apply to xy by w/z).
    std::string srcUV(const Param& p) {
        std::string s = regName(p, false);
        char sw[6] = {'.', 0, 0, 0, 0, 0};
        for (int i = 0; i < 4; ++i) sw[1 + i] = "xyzw"[p.swizzle(i)];
        const std::string v = s + sw;
        if (p.srcMod() == 10) return "(" + v + ".xy / " + v + ".w)";
        if (p.srcMod() == 9) return "(" + v + ".xy / " + v + ".z)";
        return v + ".xy";
    }

    // dst = expr (a vec4 expression), with write mask, shift and saturation.
    void assign(const Param& d, std::string expr) {
        const int shift = d.shift();
        if (shift == 1) expr = "(" + expr + ") * 2.0";
        else if (shift == 2) expr = "(" + expr + ") * 4.0";
        else if (shift == 15) expr = "(" + expr + ") * 0.5";
        else if (shift) fail("destination shift " + std::to_string(shift));
        if (d.dstMod() & 1) expr = "clamp(" + expr + ", 0.0, 1.0)";
        const int mask = d.writeMask();
        std::string name = regName(d, true);
        if (!pixel && d.type() == R_ADDR) {  // mov a0.x, ...: round to nearest (D3D9: vs_1_1 mov and mova both round)
            body += "  a0 = floor((" + expr + ").x + 0.5);\n";
            return;
        }
        if (!pixel) {
            if (d.type() == R_RASTOUT) outWritten[d.num() == 0 ? 0 : d.num() == 1 ? 11 : 12] = true;
            if (d.type() == R_ATTROUT) outWritten[1 + d.num()] = true;
            if (d.type() == R_TEXCRDOUT) outWritten[3 + d.num()] = true;
            if (d.type() == R_RASTOUT && d.num() == 1) {  // oFog is a scalar
                body += "  oFog = (" + expr + ").x;\n";
                return;
            }
            if (d.type() == R_RASTOUT && d.num() == 2) { body += "  oPts = (" + expr + ").x;\n"; return; }
        }
        if (mask == 0xF) {
            body += "  " + name + " = " + expr + ";\n";
            return;
        }
        std::string m;
        for (int i = 0; i < 4; ++i)
            if (mask >> i & 1) m += "xyzw"[i];
        body += "  " + name + "." + m + " = (" + expr + ")." + m + ";\n";
    }

    bool instruction(uint32_t tok, const Param* p, int np) {
        const int op = tok & 0xFFFF;
        const std::string s1 = np > 1 ? src(p[1]) : "", s2 = np > 2 ? src(p[2]) : "", s3 = np > 3 ? src(p[3]) : "";
        auto need = [&](int n) { if (np < n) { fail("operands of op " + std::to_string(op)); return false; } return true; };
        switch (op) {
            case 1: if (!need(2)) return false; assign(p[0], s1); return true;                                    // mov
            case 2: if (!need(3)) return false; assign(p[0], s1 + " + " + s2); return true;                        // add
            case 3: if (!need(3)) return false; assign(p[0], s1 + " - " + s2); return true;                        // sub
            case 4: if (!need(4)) return false; assign(p[0], s1 + " * " + s2 + " + " + s3); return true;           // mad
            case 5: if (!need(3)) return false; assign(p[0], s1 + " * " + s2); return true;                        // mul
            case 6: if (!need(2)) return false; assign(p[0], "vec4(1.0 / (" + s1 + ").x)"); return true;           // rcp
            case 7: if (!need(2)) return false; assign(p[0], "vec4(inversesqrt(abs((" + s1 + ").x)))"); return true;  // rsq
            case 8: if (!need(3)) return false; assign(p[0], "vec4(dot((" + s1 + ").xyz, (" + s2 + ").xyz))"); return true;  // dp3
            case 9: if (!need(3)) return false; assign(p[0], "vec4(dot(" + s1 + ", " + s2 + "))"); return true;    // dp4
            case 10: if (!need(3)) return false; assign(p[0], "min(" + s1 + ", " + s2 + ")"); return true;
            case 11: if (!need(3)) return false; assign(p[0], "max(" + s1 + ", " + s2 + ")"); return true;
            case 12: if (!need(3)) return false; assign(p[0], "vec4(lessThan(" + s1 + ", " + s2 + "))"); return true;          // slt
            case 13: if (!need(3)) return false; assign(p[0], "vec4(greaterThanEqual(" + s1 + ", " + s2 + "))"); return true;  // sge
            case 14: if (!need(2)) return false; assign(p[0], "vec4(exp2((" + s1 + ").x))"); return true;          // exp
            case 15: if (!need(2)) return false; assign(p[0], "vec4(log2(abs((" + s1 + ").x)))"); return true;     // log
            case 18: if (!need(4)) return false; assign(p[0], "mix(" + s3 + ", " + s2 + ", " + s1 + ")"); return true;  // lrp: s1*s2 + (1-s1)*s3
            case 19: if (!need(2)) return false; assign(p[0], "fract(" + s1 + ")"); return true;                   // frc
            case 78: if (!need(2)) return false; assign(p[0], "vec4(exp2((" + s1 + ").x))"); return true;          // expp
            case 79: if (!need(2)) return false; assign(p[0], "vec4(log2(abs((" + s1 + ").x)))"); return true;     // logp
            case 80:  // cnd: src0 > 0.5 ? src1 : src2 (ps_1_1: src0 is r0.a)
                if (!need(4)) return false;
                assign(p[0], "mix(" + s3 + ", " + s2 + ", vec4(greaterThan(" + s1 + ", vec4(0.5))))");
                return true;
            case 88:  // cmp: src0 >= 0 ? src1 : src2
                if (!need(4)) return false;
                assign(p[0], "mix(" + s3 + ", " + s2 + ", vec4(greaterThanEqual(" + s1 + ", vec4(0.0))))");
                return true;
            case 89: {  // bem dst.rg, src0, src1: dst = src0.rg + bumpmat(stage dst) * src1.rg
                if (!need(3)) return false;
                const int st = p[0].num();
                out->bumpStages |= 1u << st;
                const std::string m = "uBump[" + std::to_string(st) + "]";
                assign(p[0], "vec4((" + s1 + ").xy + vec2(dot(" + m + ".xz, (" + s2 + ").xy), dot(" + m + ".yw, (" + s2 + ").xy)), 0.0, 0.0)");
                return true;
            }
            // ---- ps_1_x texture ops ----
            case 64:  // texcoord tN (1.1) / texcrd rN, tM (1.4)
                if (ps14()) { if (!need(2)) return false; assign(p[0], "vec4((" + src(p[1]) + ").xyz, 1.0)"); return true; }
                if (!need(1)) return false;
                body += "  t" + std::to_string(p[0].num()) + " = clamp(vTex" + std::to_string(p[0].num()) + ", 0.0, 1.0);\n";
                texReg[p[0].num()] = false;
                return true;
            case 65:  // texkill
                if (!need(1)) return false;
                body += "  if (any(lessThan((" + regName(p[0], false) + ").xyz, vec3(0.0)))) discard;\n";
                return true;
            case 66: {  // tex tN (1.1) / texld rN, src (1.4): sample stage N
                const int st = p[0].num();
                out->samplers |= 1u << st;
                if (ps14()) {
                    if (!need(2)) return false;
                    assign(p[0], "texStageUV(" + std::to_string(st) + ", " + srcUV(p[1]) + ")");
                    return true;
                }
                if (!need(1)) return false;
                body += "  t" + std::to_string(st) + " = texStage(" + std::to_string(st) + ", vTex" + std::to_string(st) + ");\n";
                texReg[st] = true;
                return true;
            }
            case 67: case 68: {  // texbem tN, tM: sample stage N at tN.xy + bumpmat(N) * tM.xy
                if (!need(2)) return false;
                const int st = p[0].num(), sm = p[1].num();
                out->samplers |= 1u << st;
                out->bumpStages |= 1u << st;
                // the bump operand carries its modifier (_bx2 for an unsigned normal map: 2x - 1)
                const std::string m = "uBump[" + std::to_string(st) + "]", b = "(" + src(p[1]) + ")";
                (void)sm;
                body += "  t" + std::to_string(st) + " = texStageUV(" + std::to_string(st) + ", vTex" + std::to_string(st) + ".xy + vec2(dot(" + m +
                        ".xz, " + b + ".xy), dot(" + m + ".yw, " + b + ".xy)));\n";
                return true;
            }
            case 71:  // texm3x2pad tN, tM: u = dot(tN coords, tM)
                if (!need(2)) return false;
                pad3x2 = "dot(vTex" + std::to_string(p[0].num()) + ".xyz, (" + src(p[1]) + ").xyz)";  // the operand's modifier (_bx2) applies
                return true;
            case 72: {  // texm3x2tex tN, tM: v = dot(tN coords, tM); tN = sample stage N at (u, v)
                if (!need(2)) return false;
                if (pad3x2.empty()) return fail("texm3x2tex without texm3x2pad");
                const int st = p[0].num();
                out->samplers |= 1u << st;
                body += "  t" + std::to_string(st) + " = texStageUV(" + std::to_string(st) + ", vec2(" + pad3x2 + ", dot(vTex" + std::to_string(st) +
                        ".xyz, (" + src(p[1]) + ").xyz)));\n";
                pad3x2.clear();
                return true;
            }
            default:
                return fail("opcode " + std::to_string(op));
        }
    }

    bool run(const uint32_t* code) {
        const uint32_t ver = code[0];
        pixel = (ver >> 16) == 0xFFFF;
        major = (ver >> 8) & 0xFF;
        minor = ver & 0xFF;
        out->pixel = pixel, out->major = major, out->minor = minor;
        if (major != 1) return fail("shader model " + std::to_string(major) + "." + std::to_string(minor));
        size_t i = 1;
        while (true) {
            const uint32_t tok = code[i];
            if (tok == 0x0000FFFF) break;
            const int op = tok & 0xFFFF;
            if (op == 0xFFFE) { i += 1 + ((tok >> 16) & 0x7FFF); continue; }  // comment
            if (op == 0xFFFD) { ++i; phase = 1; continue; }                     // phase
            if (op == 81) {  // def cN, a, b, c, d
                const Param d{code[i + 1]};
                float f[4];
                std::memcpy(f, &code[i + 2], 16);
                char buf[160];
                std::snprintf(buf, sizeof buf, "  %s = vec4(%.9g, %.9g, %.9g, %.9g);\n", regName(d, true).c_str(), f[0], f[1], f[2], f[3]);
                body += buf;
                i += 6;
                continue;
            }
            if (op == 31) {  // dcl
                const uint32_t u = code[i + 1];
                const Param d{code[i + 2]};
                if (!pixel && d.type() == R_INPUT) out->inputs.push_back({static_cast<uint8_t>(u & 0x1F), static_cast<uint8_t>((u >> 16) & 0xF), static_cast<uint8_t>(d.num())});
                i += 3;
                continue;
            }
            Param p[4];
            int np = 0;
            size_t j = i + 1;
            while (code[j] & 0x80000000 && np < 4) p[np++].tok = code[j++];
            if (!instruction(tok, p, np)) return false;
            i = j;
        }
        return true;
    }
};

std::string vertexShaderText(Translator& t) {
    std::string s = kVertexPrologue;
    for (const Dx9Input& in : *&t.out->inputs) s += "layout(location = " + std::to_string(in.reg) + ") in vec4 v" + std::to_string(in.reg) + "_in;\n";
    s += "void main() {\n";
    for (const Dx9Input& in : t.out->inputs) {
        const std::string n = std::to_string(in.reg);
        s += "  vec4 v" + n + " = (uColorBgra & " + std::to_string(1 << in.reg) + ") != 0 ? v" + n + "_in.bgra : v" + n + "_in;\n";
    }
    s += "  vec4 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11; float a0 = 0.0;\n"
         "  vec4 oPos = vec4(0.0), oD0 = vec4(1.0), oD1 = vec4(0.0), oT0 = vec4(0.0), oT1 = vec4(0.0), oT2 = vec4(0.0), oT3 = vec4(0.0);\n"
         "  vec4 oT4 = vec4(0.0), oT5 = vec4(0.0), oT6 = vec4(0.0), oT7 = vec4(0.0); float oFog = 1.0, oPts = 1.0;\n"
         "  r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = r8 = r9 = r10 = r11 = vec4(0.0);\n";
    s += t.body;
    s += "  vColor0 = oD0; vColor1 = oD1; vTex0 = oT0; vTex1 = oT1; vTex2 = oT2; vTex3 = oT3; vTex4 = oT4; vTex5 = oT5; vTex6 = oT6; vTex7 = oT7;\n"
         "  vFog = oFog; gl_PointSize = oPts;\n"
         // Direct3D 9 samples pixels at integer coordinates, GL at half-integers: shift by half a
         // pixel (uHalfPixel = 1/viewport size) so texture-space tricks line up as on Direct3D
         "  gl_Position = vec4(oPos.x + oPos.w * uHalfPixel.x, -oPos.y + oPos.w * uHalfPixel.y, 2.0 * oPos.z - oPos.w, oPos.w);\n"
         "}\n";
    return s;
}

std::string pixelShaderText(Translator& t) {
    std::string s = kFragmentPrologue;
    s += "void main() {\n";
    s += "  vec4 v0 = clamp(vColor0, 0.0, 1.0), v1 = clamp(vColor1, 0.0, 1.0);\n";
    s += "  vec4 r0 = vec4(0.0), r1 = vec4(0.0), r2 = vec4(0.0), r3 = vec4(0.0), r4 = vec4(0.0), r5 = vec4(0.0);\n";
    s += "  vec4 t0 = vTex0, t1 = vTex1, t2 = vTex2, t3 = vTex3, t4 = vTex4, t5 = vTex5;\n";
    s += t.body;
    s += "  finish(r0);\n}\n";
    return s;
}

}  // namespace

std::string withAlphaTest(const std::string& glsl) {
    const size_t eol = glsl.find('\n');  // after "#version 300 es": the define must follow it
    if (eol == std::string::npos) return glsl;
    return glsl.substr(0, eol + 1) + "#define FABLE_ALPHA_TEST 1\n" + glsl.substr(eol + 1);
}

bool translateDx9Shader(const uint32_t* code, Dx9Shader& out, std::string& err) {
    Translator t;
    t.out = &out;
    out = Dx9Shader();
    if (!t.run(code)) { err = t.err; return false; }
    out.writesFog = t.outWritten[11];
    out.writesPointSize = t.outWritten[12];
    out.glsl = t.pixel ? pixelShaderText(t) : vertexShaderText(t);
    return true;
}

// ---- fixed function ---------------------------------------------------------------------
namespace {
std::string ffpArg(uint32_t arg, int stage, bool alpha) {
    std::string v;
    switch (arg & 0xF) {
        case 0: v = "v0"; break;                                    // DIFFUSE
        case 1: v = "cur"; break;                                   // CURRENT
        case 2: v = "tex" + std::to_string(stage); break;           // TEXTURE
        case 3: v = "uTFactor"; break;                              // TFACTOR
        case 4: v = "v1"; break;                                    // SPECULAR
        case 5: v = "tmp"; break;                                   // TEMP
        default: v = "uTFactor"; break;
    }
    if (arg & 0x20) v = v + ".aaaa";   // ALPHAREPLICATE
    if (arg & 0x10) v = "(1.0 - " + v + ")";  // COMPLEMENT
    return v;
}
std::string ffpOp(uint32_t op, const std::string& a, const std::string& b, const std::string& cur) {
    switch (op) {
        case 2: return a;                                               // SELECTARG1
        case 3: return b;                                               // SELECTARG2
        case 4: return a + " * " + b;                                   // MODULATE
        case 5: return "(" + a + " * " + b + ") * 2.0";                 // MODULATE2X
        case 6: return "(" + a + " * " + b + ") * 4.0";                 // MODULATE4X
        case 7: return a + " + " + b;                                   // ADD
        case 8: return a + " + " + b + " - 0.5";                        // ADDSIGNED
        case 9: return "(" + a + " + " + b + " - 0.5) * 2.0";           // ADDSIGNED2X
        case 10: return a + " - " + b;                                  // SUBTRACT
        case 11: return a + " + " + b + " - " + a + " * " + b;          // ADDSMOOTH
        case 12: return "mix(" + b + ", " + a + ", v0.a)";              // BLENDDIFFUSEALPHA
        case 13: return "mix(" + b + ", " + a + ", texA)";              // BLENDTEXTUREALPHA
        case 14: return "mix(" + b + ", " + a + ", uTFactor.a)";        // BLENDFACTORALPHA
        case 15: return a + " + " + b + " * (1.0 - texA)";              // BLENDTEXTUREALPHAPM
        case 16: return "mix(" + b + ", " + a + ", cur.a)";             // BLENDCURRENTALPHA
        case 24: return "vec4(dot((" + a + ").rgb - 0.5, (" + b + ").rgb - 0.5) * 4.0)";  // DOTPRODUCT3
        case 25: return a + " * " + b + " + " + cur;                      // MULTIPLYADD (arg0 = current)
        case 26: return "mix(" + b + ", " + a + ", (" + cur + ").a)";     // LERP (arg0 = current)
        case 17: return a + " * " + b;                                   // PREMODULATE: approximated as MODULATE
        default: return a;
    }
}
}  // namespace

std::string ffpPixelShader(const FfpPixelKey& key) {
    std::string s = kFragmentPrologue;
    s += "uniform vec4 uTFactor;\nvoid main() {\n";
    s += "  vec4 v0 = clamp(vColor0, 0.0, 1.0), v1 = clamp(vColor1, 0.0, 1.0);\n  vec4 cur = v0, tmp = vec4(0.0);\n";
    bool bumpNext = false;  // the previous stage was BUMPENVMAP: this stage's coordinates are perturbed by its texture
    int bumpStage = 0;
    for (int i = 0; i < 4; ++i) {
        const FfpStage& st = key.stage[i];
        if (st.colorOp == 1) break;  // DISABLE ends the chain
        const std::string tc = "vTex" + std::to_string(st.texCoordIndex & 7);
        std::string uv = st.projected ? tc + ".xy / " + tc + ".w" : tc + ".xy";
        if (bumpNext) {  // D3DTOP_BUMPENVMAP on stage i-1: uv += bumpmat(i-1) * bump.rg (the bump texture is signed RG)
            const std::string m = "uBump[" + std::to_string(bumpStage) + "]", b = "tex" + std::to_string(bumpStage);
            uv = "(" + uv + ") + vec2(dot(" + m + ".xy, " + b + ".rg), dot(" + m + ".zw, " + b + ".rg))";
            bumpNext = false;
        }
        s += "  vec4 tex" + std::to_string(i) + " = texStageUV(" + std::to_string(i) + ", " + uv + ");\n";
        if (st.colorOp == 22 || st.colorOp == 23) {  // BUMPENVMAP(LUMINANCE): no colour result of its own; perturbs the next stage
            bumpNext = true, bumpStage = i;
            continue;
        }
        s += "  { float texA = tex" + std::to_string(i) + ".a;\n";
        s += "    vec4 col = clamp(" + ffpOp(st.colorOp, ffpArg(st.colorArg1, i, false), ffpArg(st.colorArg2, i, false), "cur") + ", 0.0, 1.0);\n";
        if (st.alphaOp == 1) s += "    float alp = cur.a;\n";
        else s += "    float alp = clamp((" + ffpOp(st.alphaOp, ffpArg(st.alphaArg1, i, true), ffpArg(st.alphaArg2, i, true), "cur") + ").a, 0.0, 1.0);\n";
        s += "    cur = vec4(col.rgb, alp); }\n";
    }
    if (key.specular) s += "  cur.rgb += v1.rgb;\n";
    s += "  finish(cur);\n}\n";
    return s;
}

std::string ffpVertexShaderRhw(int texCoords) {
    std::string s =
        "#version 300 es\nprecision highp float;\n"
        "layout(location = 0) in vec4 aPos;\nlayout(location = 1) in vec4 aColor0;\nlayout(location = 2) in vec4 aColor1;\n"
        "layout(location = 3) in vec4 aTex0;\nlayout(location = 4) in vec4 aTex1;\nlayout(location = 5) in vec4 aTex2;\nlayout(location = 6) in vec4 aTex3;\n"
        "uniform vec4 uViewport;\nuniform int uColorBgra;\n"
        "out vec4 vColor0;\nout vec4 vColor1;\nout vec4 vTex0;\nout vec4 vTex1;\nout vec4 vTex2;\nout vec4 vTex3;\n"
        "out vec4 vTex4;\nout vec4 vTex5;\nout vec4 vTex6;\nout vec4 vTex7;\nout float vFog;\n"
        "void main() {\n"
        "  vColor0 = (uColorBgra & 2) != 0 ? aColor0.bgra : aColor0;\n"
        "  vColor1 = (uColorBgra & 4) != 0 ? aColor1.bgra : aColor1;\n"
        "  vTex0 = aTex0; vTex1 = aTex1; vTex2 = aTex2; vTex3 = aTex3; vTex4 = vTex5 = vTex6 = vTex7 = vec4(0.0); vFog = 1.0;\n"
        // D3D9 pre-transformed: pixel centres at integer coordinates; GL: at half-integers.
        "  float x = (aPos.x + 0.5 - uViewport.x) / uViewport.z * 2.0 - 1.0;\n"
        "  float y = (aPos.y + 0.5 - uViewport.y) / uViewport.w * 2.0 - 1.0;\n"
        "  gl_Position = vec4(x, y, 2.0 * aPos.z - 1.0, 1.0);\n"
        "}\n";
    (void)texCoords;
    return s;
}

}  // namespace host::gles
