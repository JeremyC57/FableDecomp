#include "fable/assets/TextBank.hpp"

#include "fable/assets/BigArchive.hpp"
#include "fable/assets/ByteReader.hpp"

namespace fable::assets {
namespace {

void appendUtf8(std::string& out, std::uint32_t cp) {
    if (cp < 0x80) {
        out.push_back(static_cast<char>(cp));
    } else if (cp < 0x800) {
        out.push_back(static_cast<char>(0xC0U | (cp >> 6U)));
        out.push_back(static_cast<char>(0x80U | (cp & 0x3FU)));
    } else if (cp < 0x10000) {
        out.push_back(static_cast<char>(0xE0U | (cp >> 12U)));
        out.push_back(static_cast<char>(0x80U | ((cp >> 6U) & 0x3FU)));
        out.push_back(static_cast<char>(0x80U | (cp & 0x3FU)));
    } else {
        out.push_back(static_cast<char>(0xF0U | (cp >> 18U)));
        out.push_back(static_cast<char>(0x80U | ((cp >> 12U) & 0x3FU)));
        out.push_back(static_cast<char>(0x80U | ((cp >> 6U) & 0x3FU)));
        out.push_back(static_cast<char>(0x80U | (cp & 0x3FU)));
    }
}

constexpr std::int32_t kStringType = 0;
constexpr std::int32_t kGroupType = 1;
constexpr std::int32_t kNarratorType = 2;

} // namespace

std::string utf16ToUtf8(std::span<const std::uint16_t> units) {
    std::string out;
    out.reserve(units.size());
    for (std::size_t i = 0; i < units.size(); ++i) {
        std::uint32_t cp = units[i];
        if (cp >= 0xD800 && cp <= 0xDBFF && i + 1 < units.size() && units[i + 1] >= 0xDC00 &&
            units[i + 1] <= 0xDFFF) {
            cp = 0x10000 + ((cp - 0xD800) << 10U) + (units[i + 1] - 0xDC00U);
            ++i;
        } else if (cp >= 0xD800 && cp <= 0xDFFF) {
            cp = 0xFFFD;  // unpaired surrogate
        }
        appendUtf8(out, cp);
    }
    return out;
}

TextString TextBank::parseString(std::span<const std::byte> payload) {
    ByteReader r(payload);
    std::vector<std::uint16_t> units;
    for (std::uint16_t u = r.u16(); u != 0; u = r.u16()) {
        units.push_back(u);
    }
    TextString s;
    s.content = utf16ToUtf8(units);
    s.speechBank = r.lengthPrefixedString();
    s.speaker = r.lengthPrefixedString();
    s.name = r.lengthPrefixedString();  // Identifier == entry name in retail
    const auto tagCount = r.u32();
    if (tagCount > r.remaining() / 5) {
        throw AssetError("text entry '" + s.name + "' has implausible tag count");
    }
    s.tags.reserve(tagCount);
    for (std::uint32_t i = 0; i < tagCount; ++i) {
        TextTag tag;
        tag.position = r.i32();
        tag.name = r.cstring();
        s.tags.push_back(std::move(tag));
    }
    if (r.remaining() != 0) {
        throw AssetError("text entry '" + s.name + "' has " + std::to_string(r.remaining()) +
                         " unexplained trailing bytes");
    }
    return s;
}

TextBank TextBank::load(const BigArchive& archive) {
    TextBank bank;
    bool sawTextBank = false;
    for (const auto& sb : archive.subBanks()) {
        if (sb.name.rfind("TEXT_", 0) != 0) {
            continue;
        }
        sawTextBank = true;
        for (const auto& e : sb.entries) {
            switch (e.type) {
            case kStringType: {
                auto s = parseString(archive.readPayload(e));
                s.id = e.id;
                s.name = e.name;
                bank.idsByName_.emplace(s.name, s.id);
                bank.strings_.emplace(e.id, std::move(s));
                break;
            }
            case kGroupType: {
                const auto payload = archive.readPayload(e);
                ByteReader r(payload);
                TextGroup g;
                g.id = e.id;
                g.name = e.name;
                const auto count = r.u32();
                if (count > r.remaining() / 4) {
                    throw AssetError("text group '" + e.name + "' has implausible size");
                }
                for (std::uint32_t i = 0; i < count; ++i) {
                    g.members.push_back(r.u32());
                }
                bank.idsByName_.emplace(g.name, g.id);
                bank.groups_.emplace(e.id, std::move(g));
                break;
            }
            case kNarratorType: {
                ByteReader r(e.info);  // narrator data lives in the TOC Info blob
                const auto count = r.u32();
                for (std::uint32_t i = 0; i < count; ++i) {
                    bank.narrators_.push_back(r.cstring());
                }
                break;
            }
            default:
                throw AssetError("unknown text entry type " + std::to_string(e.type) + " for '" +
                                 e.name + "'");
            }
        }
    }
    if (!sawTextBank) {
        throw AssetError(archive.path().string() + " has no TEXT_* sub-bank");
    }
    return bank;
}

const TextString* TextBank::string(std::uint32_t id) const noexcept {
    const auto it = strings_.find(id);
    return it == strings_.end() ? nullptr : &it->second;
}

const TextString* TextBank::string(std::string_view name) const noexcept {
    const auto it = idsByName_.find(std::string(name));
    return it == idsByName_.end() ? nullptr : string(it->second);
}

const TextGroup* TextBank::group(std::uint32_t id) const noexcept {
    const auto it = groups_.find(id);
    return it == groups_.end() ? nullptr : &it->second;
}

const TextGroup* TextBank::group(std::string_view name) const noexcept {
    const auto it = idsByName_.find(std::string(name));
    return it == idsByName_.end() ? nullptr : group(it->second);
}

} // namespace fable::assets
