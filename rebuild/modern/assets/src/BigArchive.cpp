#include "fable/assets/BigArchive.hpp"

#include "fable/assets/ByteReader.hpp"

#include <algorithm>

namespace fable::assets {
namespace {

std::vector<std::byte> readAt(std::ifstream& in, std::uint64_t offset, std::size_t size,
                              const std::filesystem::path& path) {
    std::vector<std::byte> buf(size);
    in.clear();
    in.seekg(static_cast<std::streamoff>(offset));
    if (size != 0) {
        in.read(reinterpret_cast<char*>(buf.data()), static_cast<std::streamsize>(size));
    }
    if (!in) {
        throw AssetError(path.string() + ": short read of " + std::to_string(size) +
                         " bytes at offset " + std::to_string(offset));
    }
    return buf;
}

BigEntry parseEntry(ByteReader& r) {
    BigEntry e;
    e.magic = r.u32();
    if (e.magic != BigArchive::kEntryMagic) {
        throw AssetError("BIGB TOC entry magic " + std::to_string(e.magic) + " != 42");
    }
    e.id = r.u32();
    e.type = r.i32();
    e.size = r.u32();
    e.offset = r.u32();
    e.crc = r.u32();
    e.name = r.lengthPrefixedString();
    e.timestamp = r.u32();
    const auto depCount = r.u32();
    if (depCount > r.remaining() / 4) {
        throw AssetError("BIGB entry '" + e.name + "' has implausible dep count");
    }
    e.deps.reserve(depCount);
    for (std::uint32_t i = 0; i < depCount; ++i) {
        e.deps.push_back(r.lengthPrefixedString());
    }
    const auto infoSize = r.u32();
    const auto info = r.bytes(infoSize);
    e.info.assign(info.begin(), info.end());
    return e;
}

} // namespace

BigArchive BigArchive::open(const std::filesystem::path& path) {
    BigArchive a;
    a.path_ = path;
    a.stream_ = std::make_unique<std::ifstream>(path, std::ios::binary);
    if (!*a.stream_) {
        throw AssetError("cannot open " + path.string());
    }
    a.fileSize_ = std::filesystem::file_size(path);
    auto& in = *a.stream_;

    const auto headerBytes = readAt(in, 0, 16, path);
    ByteReader header(headerBytes);
    const auto magic = header.bytes(4);
    if (std::string_view(reinterpret_cast<const char*>(magic.data()), 4) != "BIGB") {
        throw AssetError(path.string() + ": not a BIGB archive");
    }
    if (const auto version = header.u32(); version != kVersion) {
        throw AssetError(path.string() + ": unsupported BIGB version " + std::to_string(version));
    }
    const auto footerOffset = header.u32();
    const auto footerSize = header.u32();
    if (std::uint64_t{footerOffset} + footerSize > a.fileSize_) {
        throw AssetError(path.string() + ": footer lies outside the file");
    }

    const auto footerBytes = readAt(in, footerOffset, footerSize, path);
    ByteReader footer(footerBytes, footerOffset);
    const auto subBankCount = footer.u32();
    for (std::uint32_t i = 0; i < subBankCount; ++i) {
        BigSubBank sb;
        sb.name = footer.cstring();
        sb.version = footer.u32();
        sb.entryCount = footer.u32();
        sb.tocOffset = footer.u32();
        sb.tocSize = footer.u32();
        sb.align = footer.u32();
        a.subBanks_.push_back(std::move(sb));
    }

    for (auto& sb : a.subBanks_) {
        if (std::uint64_t{sb.tocOffset} + sb.tocSize > a.fileSize_) {
            throw AssetError(path.string() + ": TOC of " + sb.name + " lies outside the file");
        }
        const auto tocBytes = readAt(in, sb.tocOffset, sb.tocSize, path);
        ByteReader toc(tocBytes, sb.tocOffset);

        // Optional type-histogram header (EgoCore rule: count < 1000, else rewind).
        const auto statsCount = toc.u32();
        if (statsCount < 1000) {
            for (std::uint32_t s = 0; s < statsCount; ++s) {
                const auto type = toc.u32();
                const auto count = toc.u32();
                sb.typeStats.emplace_back(type, count);
            }
        } else {
            toc.seek(0);
        }

        sb.entries.reserve(sb.entryCount);
        for (std::uint32_t e = 0; e < sb.entryCount; ++e) {
            auto entry = parseEntry(toc);
            if (std::uint64_t{entry.offset} + entry.size > a.fileSize_) {
                throw AssetError(path.string() + ": payload of '" + entry.name +
                                 "' lies outside the file");
            }
            sb.entries.push_back(std::move(entry));
        }
    }
    return a;
}

std::size_t BigArchive::entryCount() const noexcept {
    std::size_t n = 0;
    for (const auto& sb : subBanks_) {
        n += sb.entries.size();
    }
    return n;
}

const BigEntry* BigArchive::find(std::string_view name) const noexcept {
    for (const auto& sb : subBanks_) {
        for (const auto& e : sb.entries) {
            if (e.name == name) {
                return &e;
            }
        }
    }
    return nullptr;
}

const BigSubBank& BigArchive::subBankOf(const BigEntry& entry) const {
    for (const auto& sb : subBanks_) {
        if (!sb.entries.empty() && &entry >= sb.entries.data() &&
            &entry < sb.entries.data() + sb.entries.size()) {
            return sb;
        }
    }
    throw AssetError("entry '" + entry.name + "' does not belong to " + path_.string());
}

std::vector<std::byte> BigArchive::readPayload(const BigEntry& entry) const {
    return readAt(*stream_, entry.offset, entry.size, path_);
}

} // namespace fable::assets
