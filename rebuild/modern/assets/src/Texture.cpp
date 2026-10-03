#include "fable/assets/Texture.hpp"

#include "fable/assets/ByteReader.hpp"
#include "fable/assets/Lzo.hpp"

#include <algorithm>
#include <string>

namespace fable::assets {
namespace {

using Rgba = std::array<std::uint8_t, 4>;

std::uint8_t u8At(std::span<const std::byte> s, std::size_t i) {
    return std::to_integer<std::uint8_t>(s[i]);
}

std::uint16_t u16At(std::span<const std::byte> s, std::size_t i) {
    return static_cast<std::uint16_t>(u8At(s, i) | (u8At(s, i + 1) << 8U));
}

Rgba expand565(std::uint16_t c) {
    const auto r = static_cast<unsigned>((c >> 11U) & 31U);
    const auto g = static_cast<unsigned>((c >> 5U) & 63U);
    const auto b = static_cast<unsigned>(c & 31U);
    return {static_cast<std::uint8_t>((r << 3U) | (r >> 2U)),
            static_cast<std::uint8_t>((g << 2U) | (g >> 4U)),
            static_cast<std::uint8_t>((b << 3U) | (b >> 2U)), 255};
}

std::uint8_t mix(unsigned a, unsigned b, unsigned wa, unsigned wb, unsigned div) {
    return static_cast<std::uint8_t>((a * wa + b * wb) / div);
}

// Decodes the 8-byte colour half of a DXT block into 16 texels.
void decodeColourBlock(std::span<const std::byte> blk, bool allowPunchThrough,
                       std::array<Rgba, 16>& texels) {
    const auto c0 = u16At(blk, 0);
    const auto c1 = u16At(blk, 2);
    std::array<Rgba, 4> pal{expand565(c0), expand565(c1), Rgba{}, Rgba{}};
    for (std::size_t ch = 0; ch < 3; ++ch) {
        if (c0 > c1 || !allowPunchThrough) {
            pal[2][ch] = mix(pal[0][ch], pal[1][ch], 2, 1, 3);
            pal[3][ch] = mix(pal[0][ch], pal[1][ch], 1, 2, 3);
        } else {
            pal[2][ch] = mix(pal[0][ch], pal[1][ch], 1, 1, 2);
            pal[3][ch] = 0;
        }
    }
    pal[2][3] = 255;
    pal[3][3] = (c0 > c1 || !allowPunchThrough) ? 255 : 0;

    std::uint32_t indices = static_cast<std::uint32_t>(u16At(blk, 4)) |
                            (static_cast<std::uint32_t>(u16At(blk, 6)) << 16U);
    for (auto& texel : texels) {
        texel = pal[indices & 3U];
        indices >>= 2U;
    }
}

} // namespace

std::string_view toString(PixelFormat format) noexcept {
    switch (format) {
    case PixelFormat::A8R8G8B8: return "A8R8G8B8";
    case PixelFormat::Dxt1: return "DXT1";
    case PixelFormat::Dxt3: return "DXT3";
    }
    return "unknown";
}

bool isKnown(std::uint32_t formatIndex) noexcept {
    return formatIndex == 0x01 || formatIndex == 0x1F || formatIndex == 0x20;
}

TextureInfo TextureInfo::parse(std::span<const std::byte> info) {
    if (info.size() != kSize) {
        throw AssetError("texture Info is " + std::to_string(info.size()) + " bytes, expected 34");
    }
    ByteReader r(info);
    TextureInfo t;
    t.allocWidth = r.u16();
    t.allocHeight = r.u16();
    t.depth = r.u16();
    t.frameWidth = r.u16();
    t.frameHeight = r.u16();
    t.frameCount = r.u16();
    t.formatIndex = r.u32();
    t.transparency = r.u8();
    t.mipLevels = r.u8();
    t.flags = r.u8();
    r.skip(1);
    t.frameDataSize = r.u32();
    t.mipSize0 = r.u32();
    for (auto& b : t.pixelFormatInit) {
        b = r.u8();
    }
    return t;
}

PixelFormat TextureInfo::format() const {
    if (!isKnown(formatIndex)) {
        throw AssetError("unsupported texture pixel format 0x" + std::to_string(formatIndex));
    }
    return static_cast<PixelFormat>(formatIndex);
}

std::size_t surfaceSize(PixelFormat format, std::uint32_t width, std::uint32_t height) {
    const std::size_t bw = std::max<std::size_t>(1, (std::size_t{width} + 3) / 4);
    const std::size_t bh = std::max<std::size_t>(1, (std::size_t{height} + 3) / 4);
    switch (format) {
    case PixelFormat::A8R8G8B8: return std::size_t{width} * height * 4;
    case PixelFormat::Dxt1: return bw * bh * 8;
    case PixelFormat::Dxt3: return bw * bh * 16;
    }
    return 0;
}

std::vector<std::byte> decodeMip0(const TextureInfo& info, std::span<const std::byte> payload) {
    const std::size_t raw = info.frameDataSize;
    if (info.mipSize0 == 0) {  // stored uncompressed
        if (payload.size() < raw) {
            throw AssetError("raw texture payload shorter than its mip 0");
        }
        return {payload.begin(), payload.begin() + static_cast<std::ptrdiff_t>(raw)};
    }
    if (payload.size() < info.mipSize0) {
        throw AssetError("texture payload shorter than MipSize0");
    }
    auto decoded = decodeChunkedLzo(payload.first(info.mipSize0), raw);
    if (decoded.consumed != info.mipSize0) {
        throw AssetError("mip 0 consumed " + std::to_string(decoded.consumed) +
                         " bytes but MipSize0 is " + std::to_string(info.mipSize0));
    }
    return std::move(decoded.output);
}

std::vector<std::uint8_t> toRgba8(PixelFormat format, std::uint32_t width, std::uint32_t height,
                                  std::span<const std::byte> surface) {
    if (surface.size() < surfaceSize(format, width, height)) {
        throw AssetError("surface smaller than " + std::to_string(width) + "x" +
                         std::to_string(height) + " " + std::string(toString(format)));
    }
    std::vector<std::uint8_t> out(std::size_t{width} * height * 4);

    if (format == PixelFormat::A8R8G8B8) {
        for (std::size_t i = 0; i < std::size_t{width} * height; ++i) {
            out[i * 4 + 0] = u8At(surface, i * 4 + 2);
            out[i * 4 + 1] = u8At(surface, i * 4 + 1);
            out[i * 4 + 2] = u8At(surface, i * 4 + 0);
            out[i * 4 + 3] = u8At(surface, i * 4 + 3);
        }
        return out;
    }

    const std::size_t blockBytes = format == PixelFormat::Dxt1 ? 8 : 16;
    const std::uint32_t bw = std::max(1U, (width + 3) / 4);
    const std::uint32_t bh = std::max(1U, (height + 3) / 4);
    std::array<Rgba, 16> texels{};
    for (std::uint32_t by = 0; by < bh; ++by) {
        for (std::uint32_t bx = 0; bx < bw; ++bx) {
            const auto blk = surface.subspan((std::size_t{by} * bw + bx) * blockBytes, blockBytes);
            if (format == PixelFormat::Dxt1) {
                decodeColourBlock(blk, true, texels);
            } else {
                decodeColourBlock(blk.subspan(8), false, texels);
                for (std::size_t i = 0; i < 16; ++i) {  // explicit 4-bit alpha
                    const auto a = static_cast<unsigned>((u8At(blk, i / 2) >> ((i % 2) * 4U)) & 0xFU);
                    texels[i][3] = static_cast<std::uint8_t>(a * 17U);
                }
            }
            for (std::uint32_t py = 0; py < 4; ++py) {
                for (std::uint32_t px = 0; px < 4; ++px) {
                    const auto x = bx * 4 + px;
                    const auto y = by * 4 + py;
                    if (x >= width || y >= height) {
                        continue;
                    }
                    const auto& tex = texels[py * 4 + px];
                    std::copy(tex.begin(), tex.end(),
                              out.begin() + static_cast<std::ptrdiff_t>((std::size_t{y} * width + x) * 4));
                }
            }
        }
    }
    return out;
}

} // namespace fable::assets
