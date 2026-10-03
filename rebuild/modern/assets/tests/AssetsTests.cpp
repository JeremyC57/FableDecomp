// Self-contained tests: synthetic data only, no retail bytes.
// Set FABLE_INSTALL_DIR to also run the read-only retail checks.

#include "fable/assets/BigArchive.hpp"
#include "fable/assets/ByteReader.hpp"
#include "fable/assets/GameInstall.hpp"
#include "fable/assets/Lzo.hpp"
#include "fable/assets/TextBank.hpp"
#include "fable/assets/Texture.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

using namespace fable::assets;
namespace fs = std::filesystem;

namespace {

int g_failures = 0;

#define CHECK(cond)                                                                    \
    do {                                                                               \
        if (!(cond)) {                                                                 \
            std::cerr << __FILE__ << ':' << __LINE__ << ": CHECK failed: " #cond "\n"; \
            ++g_failures;                                                              \
        }                                                                              \
    } while (false)

template <typename F>
bool throwsAssetError(F&& f) {
    try {
        f();
    } catch (const AssetError&) {
        return true;
    }
    return false;
}

std::vector<std::byte> bytes(std::initializer_list<unsigned> v) {
    std::vector<std::byte> out;
    for (const auto b : v) {
        out.push_back(static_cast<std::byte>(b));
    }
    return out;
}

std::string asString(const std::vector<std::byte>& v) {
    return {reinterpret_cast<const char*>(v.data()), v.size()};
}

struct Writer {
    std::vector<std::byte> buf;
    void u8(unsigned v) { buf.push_back(static_cast<std::byte>(v & 0xFFU)); }
    void u16(unsigned v) { u8(v); u8(v >> 8U); }
    void u32(std::uint32_t v) { u16(v & 0xFFFFU); u16(v >> 16U); }
    void raw(std::string_view s) { for (const char c : s) u8(static_cast<unsigned char>(c)); }
    void lpstr(std::string_view s) { u32(static_cast<std::uint32_t>(s.size() + 1)); raw(s); u8(0); }
    void put32(std::size_t at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) buf[at + static_cast<std::size_t>(i)] = static_cast<std::byte>((v >> (8 * i)) & 0xFFU);
    }
};

void testByteReader() {
    const auto data = bytes({0x01, 0x02, 0x03, 0x04, 0x05, 'h', 'i', 0x00});
    ByteReader r(data);
    CHECK(r.u16() == 0x0201);
    CHECK(r.u16() == 0x0403);
    CHECK(r.u8() == 0x05);
    CHECK(r.cstring() == "hi");
    CHECK(throwsAssetError([&] { (void)r.u8(); }));
}

void testLzoLiteralsOnly() {
    // initial literal run of 3 ("abc") then the M4 end marker 0x11 0x00 0x00
    const auto in = bytes({0x14, 'a', 'b', 'c', 0x11, 0x00, 0x00});
    const auto res = lzo1xDecompress(in, 16);
    CHECK(asString(res.output) == "abc");
    CHECK(res.consumed == in.size());
}

void testLzoM2Match() {
    // "abcd" + M2 (len 4, dist 4) + EOS -> "abcdabcd"
    const auto in = bytes({0x15, 'a', 'b', 'c', 'd', 0x6C, 0x00, 0x11, 0x00, 0x00});
    const auto res = lzo1xDecompress(in, 16);
    CHECK(asString(res.output) == "abcdabcd");
}

void testLzoOverlappingRun() {
    // "a" via short initial run (t<4 path), M2 len 7 dist 1 -> "aaaaaaaa"
    const auto in = bytes({0x12, 'a', 0xC0, 0x00, 0x11, 0x00, 0x00});
    const auto res = lzo1xDecompress(in, 16);
    CHECK(asString(res.output) == "aaaaaaaa");
}

void testLzoRejectsCorruption() {
    CHECK(throwsAssetError([] { (void)lzo1xDecompress(bytes({0x14, 'a'}), 16); }));
    // match distance before the start of output
    CHECK(throwsAssetError([] { (void)lzo1xDecompress(bytes({0x12, 'a', 0x6C, 0x10, 0x11, 0, 0}), 64); }));
    // output larger than allowed
    CHECK(throwsAssetError([] { (void)lzo1xDecompress(bytes({0x15, 'a', 'b', 'c', 'd', 0x11, 0, 0}), 2); }));
}

void testChunkedLzo() {
    // one block producing "abcdabcd" then three plain tail bytes "XYZ"
    Writer w;
    w.u16(10);
    for (const auto b : bytes({0x15, 'a', 'b', 'c', 'd', 0x6C, 0x00, 0x11, 0x00, 0x00})) w.buf.push_back(b);
    w.raw("XYZ");
    const auto res = decodeChunkedLzo(w.buf, 11);
    CHECK(asString(res.output) == "abcdabcdXYZ");
    CHECK(res.consumed == w.buf.size());
}

fs::path writeSyntheticBig(const fs::path& dir) {
    Writer w;
    w.raw("BIGB");
    w.u32(100);
    w.u32(0);  // footer offset, patched
    w.u32(0);  // footer size, patched
    const auto payloadA = w.buf.size();
    w.raw("hello");
    const auto payloadB = w.buf.size();
    w.raw("world!");

    const auto toc = w.buf.size();
    w.u32(1);       // stats count
    w.u32(0);       // type 0
    w.u32(2);       // two entries
    const auto entry = [&](std::uint32_t id, std::size_t off, std::uint32_t size, std::string_view name) {
        w.u32(42); w.u32(id); w.u32(0); w.u32(size); w.u32(static_cast<std::uint32_t>(off)); w.u32(0);
        w.lpstr(name);
        w.u32(7);           // timestamp
        w.u32(1);           // one dep
        w.lpstr("\\Dev\\src.tga");
        w.u32(2);           // info size
        w.u8(0xAA); w.u8(0xBB);
    };
    entry(1, payloadA, 5, "FIRST");
    entry(2, payloadB, 6, "SECOND");
    const auto tocSize = w.buf.size() - toc;

    const auto footer = w.buf.size();
    w.u32(1);
    w.raw("GBANK_TEST"); w.u8(0);
    w.u32(122); w.u32(2); w.u32(static_cast<std::uint32_t>(toc)); w.u32(static_cast<std::uint32_t>(tocSize)); w.u32(1);
    w.put32(8, static_cast<std::uint32_t>(footer));
    w.put32(12, static_cast<std::uint32_t>(w.buf.size() - footer));

    const auto path = dir / "synthetic.big";
    std::ofstream(path, std::ios::binary).write(reinterpret_cast<const char*>(w.buf.data()),
                                                 static_cast<std::streamsize>(w.buf.size()));
    return path;
}

void testBigArchive(const fs::path& tmp) {
    const auto big = BigArchive::open(writeSyntheticBig(tmp));
    CHECK(big.subBanks().size() == 1);
    CHECK(big.subBanks()[0].name == "GBANK_TEST");
    CHECK(big.subBanks()[0].typeStats.size() == 1);
    CHECK(big.entryCount() == 2);
    const auto* second = big.find("SECOND");
    CHECK(second != nullptr);
    if (second != nullptr) {
        CHECK(asString(big.readPayload(*second)) == "world!");
        CHECK(second->deps.size() == 1 && second->deps[0] == "\\Dev\\src.tga");
        CHECK(second->info.size() == 2);
        CHECK(&big.subBankOf(*second) == &big.subBanks()[0]);
    }
    CHECK(big.find("MISSING") == nullptr);

    const auto bogus = tmp / "bogus.big";
    std::ofstream(bogus, std::ios::binary) << "NOPE____________";
    CHECK(throwsAssetError([&] { (void)BigArchive::open(bogus); }));
}

void testTextureInfoAndDecode() {
    Writer w;
    w.u16(8); w.u16(4); w.u16(0); w.u16(6); w.u16(4); w.u16(1);
    w.u32(0x1F);
    w.u8(0); w.u8(1); w.u8(0); w.u8(0);
    w.u32(16); w.u32(0);
    for (const auto b : {3U, 4U, 0U, 0U, 0U, 0U}) w.u8(b);
    const auto info = TextureInfo::parse(w.buf);
    CHECK(info.allocWidth == 8 && info.allocHeight == 4);
    CHECK(info.frameWidth == 6 && info.frameHeight == 4);
    CHECK(info.format() == PixelFormat::Dxt1);
    CHECK(surfaceSize(PixelFormat::Dxt1, 8, 4) == 16);
    CHECK(surfaceSize(PixelFormat::Dxt3, 8, 4) == 32);
    CHECK(surfaceSize(PixelFormat::A8R8G8B8, 8, 4) == 128);

    // Two DXT1 blocks: pure red (all index 0), pure blue (all index 1).
    const auto surface = bytes({0x00, 0xF8, 0x00, 0x00, 0, 0, 0, 0,
                                0xFF, 0xFF, 0x1F, 0x00, 0x55, 0x55, 0x55, 0x55});
    const auto stored = decodeMip0(info, surface);  // MipSize0 == 0 -> raw
    const auto rgba = toRgba8(PixelFormat::Dxt1, 8, 4, stored);
    CHECK(rgba[0] == 255 && rgba[1] == 0 && rgba[2] == 0 && rgba[3] == 255);
    const std::size_t right = 4 * 4;  // texel (4,0)
    CHECK(rgba[right] == 0 && rgba[right + 2] == 255 && rgba[right + 3] == 255);

    const auto argb = toRgba8(PixelFormat::A8R8G8B8, 1, 1, bytes({0x10, 0x20, 0x30, 0x40}));
    CHECK(argb[0] == 0x30 && argb[1] == 0x20 && argb[2] == 0x10 && argb[3] == 0x40);

    CHECK(throwsAssetError([] { (void)TextureInfo::parse(bytes({1, 2, 3})); }));
}

void testTextString() {
    Writer w;
    for (const unsigned u : {0x48U, 0xE9U, 0x20ACU, 0xD83DU, 0xDE00U, 0U}) w.u16(u);  // "Hé€😀"
    w.lpstr("ScriptDialogue.lug");
    w.lpstr("FARMER");
    w.lpstr("TEXT_TEST");
    w.u32(1);
    w.u32(2); w.raw("ANIM:SCRIPT_CHEER_1"); w.u8(0);
    const auto s = TextBank::parseString(w.buf);
    CHECK(s.content == "H\xC3\xA9\xE2\x82\xAC\xF0\x9F\x98\x80");
    CHECK(s.speechBank == "ScriptDialogue.lug" && s.speaker == "FARMER" && s.name == "TEXT_TEST");
    CHECK(s.tags.size() == 1 && s.tags[0].position == 2 && s.tags[0].name == "ANIM:SCRIPT_CHEER_1");
    w.u8(0);  // trailing junk must be rejected
    CHECK(throwsAssetError([&] { (void)TextBank::parseString(w.buf); }));
}

void testCaseInsensitiveInstall(const fs::path& tmp) {
    const auto root = tmp / "FakeInstall";
    for (const auto rel : GameInstall::requiredFiles()) {
        auto p = root / fs::path(std::string(rel));
        fs::create_directories(p.parent_path());
        std::ofstream(p) << "x";
    }
    CHECK(!GameInstall::validate(root).ok);  // no language yet
    fs::create_directories(root / "data" / "lang" / "English");
    std::ofstream(root / "data" / "lang" / "English" / "text.big") << "x";

    const auto install = GameInstall::open(root);
    CHECK(install.has_value());
    if (install) {
        CHECK(install->resolve("DATA\\levels\\finalalbion.WAD").has_value());
        CHECK(!install->resolve("data/../outside").has_value());
        CHECK(!install->resolve("data/nope.big").has_value());
        CHECK(install->languages() == std::vector<std::string>{"English"});
        CHECK(!install->isKnownSteamBuild());
    }
    GameInstall::Validation report;
    CHECK(!GameInstall::open(tmp / "does-not-exist", &report).has_value());
    CHECK(!report.missing.empty());
}

void testRetailInstall(const fs::path& root) {
    std::cout << "retail checks against " << root << '\n';
    GameInstall::Validation report;
    const auto install = GameInstall::open(root, &report);
    CHECK(install.has_value());
    if (!install) {
        return;
    }
    const auto big = BigArchive::open(*install->resolve("data/graphics/pc/frontend.big"));
    CHECK(big.subBanks().size() == 1 && big.entryCount() == 394);
    const auto* backdrop = big.find("FRONTEND_BACKDROP_01");
    CHECK(backdrop != nullptr);
    if (backdrop != nullptr) {
        const auto info = TextureInfo::parse(backdrop->info);
        CHECK(info.frameWidth == 640 && info.frameHeight == 480);
        CHECK(decodeMip0(info, big.readPayload(*backdrop)).size() == info.frameDataSize);
    }
}

} // namespace

int main() {
    const auto tmp = fs::temp_directory_path() / "fable_assets_tests";
    fs::remove_all(tmp);
    fs::create_directories(tmp);

    testByteReader();
    testLzoLiteralsOnly();
    testLzoM2Match();
    testLzoOverlappingRun();
    testLzoRejectsCorruption();
    testChunkedLzo();
    testBigArchive(tmp);
    testTextureInfoAndDecode();
    testTextString();
    testCaseInsensitiveInstall(tmp);
    if (const char* dir = std::getenv("FABLE_INSTALL_DIR"); dir != nullptr && *dir != '\0') {
        testRetailInstall(dir);
    }

    fs::remove_all(tmp);
    if (g_failures != 0) {
        std::cerr << g_failures << " check(s) failed\n";
        return 1;
    }
    std::cout << "all asset tests passed\n";
    return 0;
}
