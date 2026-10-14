// Direct3D 9 shader bytecode (vs_1_1, ps_1_1 .. ps_1_4) to GLSL ES 3.00.
//
// What the game uses (see census.cpp's report): vs_1_1 with mov/add/sub/mad/mul/dp3/dp4/
// rcp/rsq/min/max/sge/slt and relative constant addressing (c[a0.x + n]); ps_1_1 with
// mov/add/sub/mul/mad/lrp/dp3/cnd and the texture ops tex/texcoord/texbem/texm3x2pad/
// texm3x2tex; ps_1_4 with texld/texcrd/phase/cmp/bem and projective loads (_dw).
//
// Conventions of the generated code (gl_renderer.cpp supplies the uniforms):
//   vertex:   in vec4 vN (attribute N, one per dcl; D3DCOLOR inputs are swizzled .bgra
//             when bit N of uniform int uColorBgra is set); uniform vec4 c[96]; outputs
//             vColor0/1, vTex0..7, vFog; gl_Position = D3D clip position with y flipped
//             and z remapped (uniform vec4 uPosFix is applied as x*fix.x ..., see code).
//   fragment: uniform sampler2D s0..s5; uniform vec4 pc[8] (ps constants); uniform vec4
//             uBump[6] (BUMPENVMAT00,01,10,11 per stage); uniform int uProjMask (stages
//             whose texture coordinates are projected); common epilogue for fog
//             (uniform vec4 uFogColor; uniform int uFogEnable) and alpha test (uniform
//             int uAlphaFunc; uniform float uAlphaRef). Writes out vec4 fragColor.
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace host::gles {

struct Dx9Input {
    uint8_t usage;  // D3DDECLUSAGE
    uint8_t index;
    uint8_t reg;    // vN
};

struct Dx9Shader {
    bool pixel = false;
    int major = 0, minor = 0;
    std::string glsl;
    std::vector<Dx9Input> inputs;  // vertex shaders: the dcl'd inputs
    uint32_t samplers = 0;         // pixel shaders: stages sampled (bit n)
    uint32_t bumpStages = 0;       // stages whose BUMPENVMAT the shader reads
    bool writesFog = false;
    bool writesPointSize = false;
};

// Returns false with `err` set on an unsupported instruction.
bool translateDx9Shader(const uint32_t* code, Dx9Shader& out, std::string& err);

// The fixed-function pixel pipeline (SetPixelShader(NULL)): a fragment shader built from the
// texture stage states, with the same epilogue as translated pixel shaders.
struct FfpStage {
    uint32_t colorOp = 1, colorArg1 = 2, colorArg2 = 0, alphaOp = 1, alphaArg1 = 2, alphaArg2 = 0;
    uint32_t texCoordIndex = 0;
    bool projected = false;
    bool operator==(const FfpStage&) const = default;
};
struct FfpPixelKey {
    FfpStage stage[4];  // Direct3D allows 8; the game uses up to 4 (bump-mapped water: bump, BUMPENVMAP, blend)
    bool specular = false;  // D3DRS_SPECULARENABLE
    bool operator==(const FfpPixelKey&) const = default;
};
std::string ffpPixelShader(const FfpPixelKey& key);

// The fixed-function vertex path used for pre-transformed (XYZRHW / POSITIONT) vertices:
// attribute 0 = position (x, y, z, rhw), 1 = diffuse, 2 = specular, 3.. = tex0..3;
// uniform vec4 uViewport (x, y, w, h).
std::string ffpVertexShaderRhw(int texCoords);

// A fragment shader's text (pixel shader or fixed-function) with the alpha test compiled in.
// Without it the epilogue never discards: the renderer builds this variant only for draws with
// D3DRS_ALPHATESTENABLE and a function other than ALWAYS.
std::string withAlphaTest(const std::string& glsl);

// The shared epilogue pieces.
extern const char* kVertexPrologue;
extern const char* kFragmentPrologue;

}  // namespace host::gles
