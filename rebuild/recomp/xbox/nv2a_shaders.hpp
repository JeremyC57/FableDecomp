// NV2A shader translation to GLSL (Vulkan): vertex programs and register combiners.
#pragma once

#include "gpu.hpp"

#include <string>

namespace xb::gpu {

// GLSL statements for the vertex program starting at `start` (reads v0..v15, C(i); writes
// oPos, oD0, oD1, oFog, oPts, oB0, oB1, oT0..oT3).
std::string translateVertexProgram(const uint32_t (*program)[4], uint32_t start);
const char* vertexProgramPrelude();

struct CombinerInfo {
    uint32_t texMode[4] = {};
};
// GLSL statements computing fragColor from v0, v1, pFog, vTex0..3 and samplers tex0..3.
std::string translateCombiners(const State& s, CombinerInfo* info);

} // namespace xb::gpu
