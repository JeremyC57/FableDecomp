#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace fable::assets {

struct LzoResult {
    std::vector<std::byte> output;
    std::size_t consumed = 0;  ///< input bytes up to and including the end marker
};

/// Bounds-checked LZO1X decompression (the codec of the engine's
/// `lzo1x_decompress` @ 0x00C06B90). Stops at the end-of-stream marker.
/// Throws AssetError on corrupt input or if output would exceed `maxOutput`.
[[nodiscard]] LzoResult lzo1xDecompress(std::span<const std::byte> input, std::size_t maxOutput);

/// Fable's chunked-LZO framing (docs/formats/BIG.md section 6):
///   block := u16 clen | u16 0xFFFF, u32 clen ; then clen bytes of LZO1X
/// Blocks are decoded until `rawSize - 3` bytes exist, then 3 plain tail bytes
/// follow. Returns exactly `rawSize` bytes; `consumed` reports bytes read.
[[nodiscard]] LzoResult decodeChunkedLzo(std::span<const std::byte> input, std::size_t rawSize);

} // namespace fable::assets
