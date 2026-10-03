#include "fable/assets/GameInstall.hpp"

#include <algorithm>
#include <cctype>
#include <system_error>

namespace fable::assets {
namespace {

std::string lower(std::string_view s) {
    std::string out(s);
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return out;
}

std::vector<std::string> splitPath(std::string_view relative) {
    std::vector<std::string> parts;
    std::string cur;
    for (const char c : relative) {
        if (c == '/' || c == '\\') {
            if (!cur.empty()) {
                parts.push_back(std::move(cur));
                cur.clear();
            }
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) {
        parts.push_back(std::move(cur));
    }
    return parts;
}

} // namespace

std::optional<std::filesystem::path> resolveCaseInsensitive(const std::filesystem::path& base,
                                                            std::string_view relative) {
    namespace fs = std::filesystem;
    fs::path cur = base;
    std::error_code ec;
    for (const auto& part : splitPath(relative)) {
        if (part == "..") {
            return std::nullopt;  // stay inside the install
        }
        if (part == ".") {
            continue;
        }
        fs::path exact = cur / part;
        if (fs::exists(exact, ec)) {
            cur = std::move(exact);
            continue;
        }
        const auto want = lower(part);
        bool found = false;
        for (fs::directory_iterator it(cur, ec), end; !ec && it != end; it.increment(ec)) {
            if (lower(it->path().filename().string()) == want) {
                cur = it->path();
                found = true;
                break;
            }
        }
        if (!found) {
            return std::nullopt;
        }
    }
    return cur;
}

const std::vector<std::string_view>& GameInstall::requiredFiles() {
    static const std::vector<std::string_view> files{
        "data/graphics/graphics.big",
        "data/graphics/pc/textures.big",
        "data/graphics/pc/frontend.big",
        "data/shaders/pc/shaders.big",
        "data/Misc/pc/effects.big",
        "data/Levels/FinalAlbion.wld",
        "data/Levels/FinalAlbion.wad",
        "data/CompiledDefs/game.bin",
        "data/CompiledDefs/frontend.bin",
        "data/CompiledDefs/script.bin",
        "data/CompiledDefs/names.bin",
        "data/Sound/Ingame.lug",
    };
    return files;
}

GameInstall::Validation GameInstall::validate(const std::filesystem::path& root) {
    Validation v;
    std::error_code ec;
    if (!std::filesystem::is_directory(root, ec)) {
        v.missing.emplace_back(root.string() + " (not a directory)");
        return v;
    }
    for (const auto rel : requiredFiles()) {
        if (!resolveCaseInsensitive(root, rel)) {
            v.missing.emplace_back(rel);
        }
    }
    if (GameInstall(root).languages().empty()) {
        v.missing.emplace_back("data/lang/<language>/text.big");
    }
    v.ok = v.missing.empty();
    return v;
}

std::optional<GameInstall> GameInstall::open(const std::filesystem::path& root, Validation* report) {
    auto v = validate(root);
    const bool ok = v.ok;
    if (report != nullptr) {
        *report = std::move(v);
    }
    if (!ok) {
        return std::nullopt;
    }
    return GameInstall(root);
}

std::optional<std::filesystem::path> GameInstall::resolve(std::string_view relative) const {
    return resolveCaseInsensitive(root_, relative);
}

std::optional<std::uintmax_t> GameInstall::exeSize() const {
    const auto exe = resolve("Fable.exe");
    if (!exe) {
        return std::nullopt;
    }
    std::error_code ec;
    const auto size = std::filesystem::file_size(*exe, ec);
    if (ec) {
        return std::nullopt;
    }
    return size;
}

std::vector<std::string> GameInstall::languages() const {
    std::vector<std::string> out;
    const auto langDir = resolve("data/lang");
    if (!langDir) {
        return out;
    }
    std::error_code ec;
    for (std::filesystem::directory_iterator it(*langDir, ec), end; !ec && it != end;
         it.increment(ec)) {
        if (it->is_directory(ec) && resolveCaseInsensitive(it->path(), "text.big")) {
            out.push_back(it->path().filename().string());
        }
    }
    std::sort(out.begin(), out.end());
    return out;
}

} // namespace fable::assets
