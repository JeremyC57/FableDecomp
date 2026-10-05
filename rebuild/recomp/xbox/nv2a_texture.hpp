// NV2A texture formats: decoding guest texture memory to BGRA8.
#pragma once

#include <cstdint>

namespace xb::gpu {

uint32_t texelBytes(uint32_t colorFormat);  // 0 for block-compressed formats
bool isLinear(uint32_t colorFormat);        // pitch-linear (LU_IMAGE) rather than swizzled
bool isCompressed(uint32_t colorFormat);    // DXT1/3/5
uint32_t levelBytes(uint32_t colorFormat, uint32_t w, uint32_t h, uint32_t d);
// Decodes one mip level (or volume) to BGRA8 texels (w * h * d words).
void decodeLevel(uint32_t colorFormat, const uint8_t* src, uint32_t w, uint32_t h, uint32_t d, uint32_t pitch,
                 const uint32_t* palette, uint32_t* out);

} // namespace xb::gpu
