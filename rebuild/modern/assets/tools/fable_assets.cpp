// fable_assets: inspect and export data from a retail Fable: TLC install.
//
//   fable_assets install <game-dir>
//   fable_assets list    <file.big> [--entries]
//   fable_assets verify  <file.big>                 decode every texture mip 0
//   fable_assets texture <file.big> <ENTRY> <out.tga>
//   fable_assets payload <file.big> <ENTRY> <out.bin>
//
// Exports are written for local inspection only; never redistribute them.

#include "fable/assets/BigArchive.hpp"
#include "fable/assets/ByteReader.hpp"
#include "fable/assets/GameInstall.hpp"
#include "fable/assets/Texture.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using namespace fable::assets;

namespace {

int usage() {
    std::cerr << "usage:\n"
                 "  fable_assets install <game-dir>\n"
                 "  fable_assets list    <file.big> [--entries]\n"
                 "  fable_assets verify  <file.big>\n"
                 "  fable_assets texture <file.big> <ENTRY> <out.tga>\n"
                 "  fable_assets payload <file.big> <ENTRY> <out.bin>\n";
    return 2;
}

bool isTextureBank(const BigSubBank& sb) { return sb.name.rfind("GBANK_", 0) == 0; }

void writeTga(const std::string& path, std::uint32_t w, std::uint32_t h,
              const std::vector<std::uint8_t>& rgba) {
    std::ofstream out(path, std::ios::binary);
    std::uint8_t header[18] = {};
    header[2] = 2;  // uncompressed true-colour
    header[12] = static_cast<std::uint8_t>(w & 0xFFU);
    header[13] = static_cast<std::uint8_t>(w >> 8U);
    header[14] = static_cast<std::uint8_t>(h & 0xFFU);
    header[15] = static_cast<std::uint8_t>(h >> 8U);
    header[16] = 32;
    header[17] = 0x28;  // top-left origin, 8 alpha bits
    out.write(reinterpret_cast<const char*>(header), sizeof header);
    for (std::size_t i = 0; i < std::size_t{w} * h; ++i) {
        const char px[4] = {static_cast<char>(rgba[i * 4 + 2]), static_cast<char>(rgba[i * 4 + 1]),
                            static_cast<char>(rgba[i * 4 + 0]), static_cast<char>(rgba[i * 4 + 3])};
        out.write(px, 4);
    }
    if (!out) {
        throw AssetError("failed to write " + path);
    }
}

int cmdInstall(const std::string& dir) {
    GameInstall::Validation report;
    const auto install = GameInstall::open(dir, &report);
    if (!install) {
        std::cout << "NOT a usable Fable: TLC install. Missing:\n";
        for (const auto& m : report.missing) {
            std::cout << "  " << m << '\n';
        }
        return 1;
    }
    std::cout << "OK: " << install->root().string() << '\n';
    if (const auto size = install->exeSize()) {
        std::cout << "Fable.exe: " << *size << " bytes"
                  << (install->isKnownSteamBuild() ? " (Steam build targeted by FableDecomp)" : " (other build)")
                  << '\n';
    }
    std::cout << "languages:";
    for (const auto& l : install->languages()) {
        std::cout << ' ' << l;
    }
    std::cout << '\n';
    return 0;
}

int cmdList(const std::string& file, bool entries) {
    const auto big = BigArchive::open(file);
    std::cout << big.path().filename().string() << ": " << big.subBanks().size() << " sub-banks, "
              << big.entryCount() << " entries\n";
    for (const auto& sb : big.subBanks()) {
        std::cout << "  " << sb.name << "  v" << sb.version << "  entries=" << sb.entries.size()
                  << "  align=" << sb.align << '\n';
        if (!entries) {
            continue;
        }
        for (const auto& e : sb.entries) {
            std::cout << "    " << e.id << '\t' << e.type << '\t' << e.size << '\t' << e.name;
            if (isTextureBank(sb) && e.info.size() == TextureInfo::kSize) {
                const auto t = TextureInfo::parse(e.info);
                std::cout << '\t' << t.frameWidth << 'x' << t.frameHeight << " in " << t.allocWidth
                          << 'x' << t.allocHeight << " fmt=0x" << std::hex << t.formatIndex
                          << std::dec << " mips=" << unsigned{t.mipLevels};
            }
            std::cout << '\n';
        }
    }
    return 0;
}

int cmdVerify(const std::string& file) {
    const auto big = BigArchive::open(file);
    std::size_t ok = 0;
    std::size_t failed = 0;
    std::size_t skipped = 0;
    for (const auto& sb : big.subBanks()) {
        if (!isTextureBank(sb)) {
            continue;
        }
        for (const auto& e : sb.entries) {
            try {
                const auto info = TextureInfo::parse(e.info);
                if (!isKnown(info.formatIndex)) {
                    ++skipped;
                    continue;
                }
                const auto surface = decodeMip0(info, big.readPayload(e));
                if (surface.size() != info.frameDataSize) {
                    throw AssetError("wrong mip-0 size");
                }
                ++ok;
            } catch (const AssetError& err) {
                ++failed;
                std::cout << "FAIL " << sb.name << '/' << e.name << ": " << err.what() << '\n';
            }
        }
    }
    std::cout << "textures decoded: " << ok << " ok, " << failed << " failed, " << skipped
              << " skipped (unsupported format)\n";
    return failed == 0 ? 0 : 1;
}

int cmdTexture(const std::string& file, const std::string& name, const std::string& outPath) {
    const auto big = BigArchive::open(file);
    const auto* entry = big.find(name);
    if (entry == nullptr) {
        std::cerr << "no entry named " << name << '\n';
        return 1;
    }
    const auto info = TextureInfo::parse(entry->info);
    const auto surface = decodeMip0(info, big.readPayload(*entry));
    const auto rgba = toRgba8(info.format(), info.allocWidth, info.allocHeight, surface);

    // Crop the authored frame out of the power-of-two surface.
    const std::uint32_t w = info.frameWidth;
    const std::uint32_t h = info.frameHeight;
    std::vector<std::uint8_t> frame(std::size_t{w} * h * 4);
    for (std::uint32_t y = 0; y < h; ++y) {
        const auto* src = rgba.data() + std::size_t{y} * info.allocWidth * 4;
        std::copy(src, src + std::size_t{w} * 4, frame.begin() + static_cast<std::ptrdiff_t>(std::size_t{y} * w * 4));
    }
    writeTga(outPath, w, h, frame);
    std::cout << name << ": " << w << 'x' << h << ' ' << toString(info.format()) << " -> " << outPath << '\n';
    return 0;
}

int cmdPayload(const std::string& file, const std::string& name, const std::string& outPath) {
    const auto big = BigArchive::open(file);
    const auto* entry = big.find(name);
    if (entry == nullptr) {
        std::cerr << "no entry named " << name << '\n';
        return 1;
    }
    const auto bytes = big.readPayload(*entry);
    std::ofstream out(outPath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    return out ? 0 : 1;
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        return usage();
    }
    const std::string_view cmd = argv[1];
    try {
        if (cmd == "install") {
            return cmdInstall(argv[2]);
        }
        if (cmd == "list") {
            return cmdList(argv[2], argc > 3 && std::string_view(argv[3]) == "--entries");
        }
        if (cmd == "verify") {
            return cmdVerify(argv[2]);
        }
        if (cmd == "texture" && argc == 5) {
            return cmdTexture(argv[2], argv[3], argv[4]);
        }
        if (cmd == "payload" && argc == 5) {
            return cmdPayload(argv[2], argv[3], argv[4]);
        }
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return usage();
}
