// NV2A shader translation to GLSL (Vulkan): vertex programs and register combiners.
#pragma once

#include "gpu.hpp"

#include <string>

namespace xb::gpu {

// GLSL statements for the vertex program starting at `start` (reads v0..v15, C(i); writes
// oPos, oD0, oD1, oFog, oPts, oB0, oB1, oT0..oT3).
std::string translateVertexProgram(const uint32_t (*program)[4], uint32_t start);
const char* vertexProgramPrelude();

// Runs the vertex program on the CPU for one vertex (inputs v[16], constants c[192]) and returns
// oPos, with the same semantics as the GLSL translation. Used to classify small draws (screen-
// space UI vs world) for the 16:9 layout.
void evalVertexPosition(const uint32_t (*program)[4], uint32_t start, const float (*c)[4], const float (*v)[4], float oPos[4]);

struct CombinerInfo {
    uint32_t texMode[4] = {};
};
// GLSL statements computing fragColor from v0, v1, pFog, vTex0..3 and samplers tex0..3.
// shadowMask: stages whose texture is a depth surface (PROJECT2D/3D become depth compares).
std::string translateCombiners(const State& s, CombinerInfo* info, uint32_t shadowMask = 0);

} // namespace xb::gpu
