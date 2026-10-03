#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace fable::assets {

/// The user's own retail Fable: The Lost Chapters installation.
///
/// The port ships no game data: every asset is read from here at runtime.
/// Path lookups are case-insensitive so the same relative paths work on
/// Windows, Linux and Android storage (retail names mix case, e.g.
/// `data/Sound/GUILD_NIGHT.OGG`, `data/lang/English/Dialogue.lut`).
class GameInstall {
public:
    /// Steam build that FableDecomp targets (16,666,624 bytes, image base 0x400000).
    static constexpr std::uintmax_t kSteamExeSize = 16'666'624;

    struct Validation {
        bool ok = false;
        std::vector<std::string> missing;  ///< required relative paths not found
    };

    /// Files the engine cannot start without.
    [[nodiscard]] static const std::vector<std::string_view>& requiredFiles();

    [[nodiscard]] static Validation validate(const std::filesystem::path& root);

    /// Returns an install when validation passes; otherwise nullopt and, if
    /// `report` is given, the reason.
    [[nodiscard]] static std::optional<GameInstall> open(const std::filesystem::path& root,
                                                        Validation* report = nullptr);

    [[nodiscard]] const std::filesystem::path& root() const noexcept { return root_; }

    /// Case-insensitive lookup of a relative path ('/' or '\\' separators).
    [[nodiscard]] std::optional<std::filesystem::path> resolve(std::string_view relative) const;

    /// Size of Fable.exe, if present (the port does not run it; this only
    /// identifies the build for bug reports).
    [[nodiscard]] std::optional<std::uintmax_t> exeSize() const;
    [[nodiscard]] bool isKnownSteamBuild() const { return exeSize() == kSteamExeSize; }

    /// Language folders under data/lang (e.g. "English").
    [[nodiscard]] std::vector<std::string> languages() const;

private:
    explicit GameInstall(std::filesystem::path root) : root_(std::move(root)) {}

    std::filesystem::path root_;
};

/// Case-insensitive resolution relative to any base directory.
[[nodiscard]] std::optional<std::filesystem::path> resolveCaseInsensitive(
    const std::filesystem::path& base, std::string_view relative);

} // namespace fable::assets
