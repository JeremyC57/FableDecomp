#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace fable::assets {

/// One TOC record of a BIGB sub-bank (docs/formats/BIG.md section 2).
struct BigEntry {
    std::uint32_t magic = 0;   ///< always 42 in retail
    std::uint32_t id = 0;
    std::int32_t type = 0;     ///< meaning depends on the bank kind
    std::uint32_t size = 0;    ///< payload bytes
    std::uint32_t offset = 0;  ///< absolute payload offset in the .big file
    std::uint32_t crc = 0;     ///< per-format class stamp, not a payload hash
    std::string name;
    std::uint32_t timestamp = 0;
    std::vector<std::string> deps;  ///< source build paths / speaker tags
    std::vector<std::byte> info;    ///< kind-specific descriptor (34 B for textures)
};

struct BigSubBank {
    std::string name;  ///< prefix selects the kind: GBANK_, MBANK_, TEXT_, LIPSYNC_, ...
    std::uint32_t version = 0;
    std::uint32_t entryCount = 0;
    std::uint32_t tocOffset = 0;
    std::uint32_t tocSize = 0;
    std::uint32_t align = 0;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> typeStats;  ///< (type, count)
    std::vector<BigEntry> entries;
};

/// Read-only BIGB ("BIGB", version 100) bank archive.
///
/// The directory is parsed eagerly; payloads are read on demand so the
/// 500 MB textures.big costs only its TOC in memory. Not thread-safe: use one
/// instance per thread when streaming in parallel.
class BigArchive {
public:
    static constexpr std::uint32_t kVersion = 100;
    static constexpr std::uint32_t kEntryMagic = 42;

    [[nodiscard]] static BigArchive open(const std::filesystem::path& path);

    [[nodiscard]] const std::filesystem::path& path() const noexcept { return path_; }
    [[nodiscard]] std::uint64_t fileSize() const noexcept { return fileSize_; }
    [[nodiscard]] const std::vector<BigSubBank>& subBanks() const noexcept { return subBanks_; }
    [[nodiscard]] std::size_t entryCount() const noexcept;

    /// First entry with this exact name across all sub-banks, or nullptr.
    [[nodiscard]] const BigEntry* find(std::string_view name) const noexcept;
    /// Sub-bank that owns `entry` (entry must come from this archive).
    [[nodiscard]] const BigSubBank& subBankOf(const BigEntry& entry) const;

    [[nodiscard]] std::vector<std::byte> readPayload(const BigEntry& entry) const;

private:
    BigArchive() = default;

    std::filesystem::path path_;
    std::uint64_t fileSize_ = 0;
    std::vector<BigSubBank> subBanks_;
    std::unique_ptr<std::ifstream> stream_;
};

} // namespace fable::assets
