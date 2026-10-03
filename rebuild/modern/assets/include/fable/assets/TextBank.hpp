#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace fable::assets {

class BigArchive;

struct TextTag {
    std::int32_t position = 0;  ///< character index into the content where it fires
    std::string name;           ///< ANIM:..., CAM:..., CONVERSATION_ATTITUDE_..., custom
};

/// Type-0 entry of a TEXT_* sub-bank (docs/formats/TEXT.md section 4).
struct TextString {
    std::uint32_t id = 0;
    std::string name;
    std::string content;     ///< UTF-8 (stored as UTF-16LE on disk)
    std::string speechBank;  ///< voice bank, e.g. "ScriptDialogue.lug", often empty
    std::string speaker;
    std::vector<TextTag> tags;
};

/// Type-1 entry: a set of string IDs (random variants of one line).
struct TextGroup {
    std::uint32_t id = 0;
    std::string name;
    std::vector<std::uint32_t> members;
};

/// The localized string table of `data/lang/<language>/text.big`.
/// Runtime lookup is by numeric ID, exactly as `NGameText::CDataBank`.
class TextBank {
public:
    [[nodiscard]] static TextBank load(const BigArchive& archive);

    /// Decode one type-0 payload (exposed for tests and tools).
    [[nodiscard]] static TextString parseString(std::span<const std::byte> payload);

    [[nodiscard]] const TextString* string(std::uint32_t id) const noexcept;
    [[nodiscard]] const TextString* string(std::string_view name) const noexcept;
    [[nodiscard]] const TextGroup* group(std::uint32_t id) const noexcept;
    [[nodiscard]] const TextGroup* group(std::string_view name) const noexcept;
    [[nodiscard]] const std::vector<std::string>& narrators() const noexcept { return narrators_; }

    [[nodiscard]] std::size_t stringCount() const noexcept { return strings_.size(); }
    [[nodiscard]] std::size_t groupCount() const noexcept { return groups_.size(); }

private:
    std::unordered_map<std::uint32_t, TextString> strings_;
    std::unordered_map<std::uint32_t, TextGroup> groups_;
    std::unordered_map<std::string, std::uint32_t> idsByName_;
    std::vector<std::string> narrators_;
};

/// UTF-16LE code units (surrogate pairs allowed) to UTF-8.
[[nodiscard]] std::string utf16ToUtf8(std::span<const std::uint16_t> units);

} // namespace fable::assets
