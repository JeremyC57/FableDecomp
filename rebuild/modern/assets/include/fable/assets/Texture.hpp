#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

namespace fable::assets {

/// Info[+12] pixel-format enum (docs/formats/TEXTURE.md section 3).
enum class PixelFormat : std::uint32_t {
    A8R8G8B8 = 0x01,  ///< 32 bpp, stored as B,G,R,A bytes
    Dxt1 = 0x1F,
    Dxt3 = 0x20,
};

[[nodiscard]] std::string_view toString(PixelFormat format) noexcept;
[[nodiscard]] bool isKnown(std::uint32_t formatIndex) noexcept;

/// The 34-byte GBANK texture Info descriptor (CGraphicHeader + CPixelFormatInit).
struct TextureInfo {
    static constexpr std::size_t kSize = 34;

    std::uint16_t allocWidth = 0;   ///< power-of-two GPU surface
    std::uint16_t allocHeight = 0;
    std::uint16_t depth = 0;
    std::uint16_t frameWidth = 0;   ///< authored image size inside the surface
    std::uint16_t frameHeight = 0;
    std::uint16_t frameCount = 0;
    std::uint32_t formatIndex = 0;
    std::uint8_t transparency = 0;
    std::uint8_t mipLevels = 0;
    std::uint8_t flags = 0;
    std::uint32_t frameDataSize = 0;  ///< uncompressed mip-0 bytes
    std::uint32_t mipSize0 = 0;       ///< on-disk mip-0 bytes, 0 = stored raw
    std::array<std::uint8_t, 6> pixelFormatInit{};

    [[nodiscard]] static TextureInfo parse(std::span<const std::byte> info);
    [[nodiscard]] PixelFormat format() const;
};

/// Surface bytes for one mip level in the texture's native format.
[[nodiscard]] std::size_t surfaceSize(PixelFormat format, std::uint32_t width, std::uint32_t height);

/// Mip 0 of a GBANK payload, decompressed to `info.frameDataSize` native bytes.
[[nodiscard]] std::vector<std::byte> decodeMip0(const TextureInfo& info,
                                                std::span<const std::byte> payload);

/// Convert a native surface to tightly packed RGBA8 (for export and for
/// renderers without S3TC, e.g. some Android GLES devices).
[[nodiscard]] std::vector<std::uint8_t> toRgba8(PixelFormat format, std::uint32_t width,
                                                std::uint32_t height,
                                                std::span<const std::byte> surface);

} // namespace fable::assets
