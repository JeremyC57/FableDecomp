// PC texture replacement (pc_textures = <Fable: The Lost Chapters install folder>).
//
// The Xbox and PC versions ship the same texture banks (BIGB, GBANK_MAIN / GBANK_GUI /
// GBANK_FRONT_END) under the same entry names, with larger images on PC for about 40% of them.
// The renderer only sees texture memory, so a background thread maps each Xbox texture's mip 0
// contents (from the disc's textures.biz / frontend.biz, zlib-wrapped banks) to its entry name,
// cached in <cache>/pc_texture_names.txt; a DXT texture the game uploads is looked up by the hash
// of its mip 0 and, when the PC bank has a larger version of that name, replaced by it.
#pragma once

#include <cstdint>
#include <vector>

namespace xb::pctex {

// Starts the background indexing (no-op when the setting is empty or the folder has no banks).
void init(const char* gameDir, const char* cacheDir);

// 64-bit content hash used for the lookup (same function on both sides).
uint64_t hashBytes(const uint8_t* p, size_t n);

struct Replacement {
    uint32_t w = 0, h = 0;
    std::vector<uint8_t> bgra;  // w * h texels, B,G,R,A bytes (VK_FORMAT_B8G8R8A8_UNORM)
};
// A DXT texture of w x h whose mip 0 (mip0Bytes long) hashes to `hash`: fills `out` with the PC
// version when one with more texels exists. Thread: the renderer's.
bool lookup(uint64_t hash, uint32_t mip0Bytes, uint32_t w, uint32_t h, Replacement& out);

} // namespace xb::pctex
