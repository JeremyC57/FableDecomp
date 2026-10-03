#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>

namespace fable::assets {

/// Thrown for any malformed or truncated retail data. Messages name the
/// structure and offset so a failing file can be located quickly.
class AssetError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/// Bounds-checked little-endian cursor over an immutable byte span.
/// Fable's PC data is little-endian x86 layout throughout.
class ByteReader {
public:
    explicit ByteReader(std::span<const std::byte> data, std::size_t base = 0) noexcept
        : data_(data), base_(base) {}

    [[nodiscard]] std::size_t position() const noexcept { return pos_; }
    [[nodiscard]] std::size_t remaining() const noexcept { return data_.size() - pos_; }
    [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }

    void seek(std::size_t pos) {
        if (pos > data_.size()) {
            fail("seek", pos - pos_);
        }
        pos_ = pos;
    }

    void skip(std::size_t n) {
        require(n, "skip");
        pos_ += n;
    }

    [[nodiscard]] std::uint8_t u8() {
        require(1, "u8");
        return std::to_integer<std::uint8_t>(data_[pos_++]);
    }

    [[nodiscard]] std::uint16_t u16() {
        require(2, "u16");
        const auto v = static_cast<std::uint16_t>(at(0) | (at(1) << 8U));
        pos_ += 2;
        return v;
    }

    [[nodiscard]] std::uint32_t u32() {
        require(4, "u32");
        const std::uint32_t v = at(0) | (at(1) << 8U) | (at(2) << 16U) | (at(3) << 24U);
        pos_ += 4;
        return v;
    }

    [[nodiscard]] std::int32_t i32() { return static_cast<std::int32_t>(u32()); }

    [[nodiscard]] std::span<const std::byte> bytes(std::size_t n) {
        require(n, "bytes");
        auto s = data_.subspan(pos_, n);
        pos_ += n;
        return s;
    }

    /// NUL-terminated string (footer sub-bank names).
    [[nodiscard]] std::string cstring() {
        std::string out;
        for (;;) {
            const auto c = u8();
            if (c == 0) {
                return out;
            }
            out.push_back(static_cast<char>(c));
        }
    }

    /// `u32 length` + bytes. Retail lengths usually include a trailing NUL,
    /// engine-accepted rebuilt records may not; trailing NULs are stripped.
    [[nodiscard]] std::string lengthPrefixedString() {
        const auto len = u32();
        auto raw = bytes(len);
        std::string out(reinterpret_cast<const char*>(raw.data()), raw.size());
        while (!out.empty() && out.back() == '\0') {
            out.pop_back();
        }
        return out;
    }

private:
    [[nodiscard]] std::uint32_t at(std::size_t i) const {
        return std::to_integer<std::uint32_t>(data_[pos_ + i]);
    }

    void require(std::size_t n, const char* what) const {
        if (n > remaining()) {
            fail(what, n);
        }
    }

    [[noreturn]] void fail(const char* what, std::size_t n) const {
        throw AssetError(std::string("truncated read (") + what + ", " + std::to_string(n) +
                         " bytes) at offset 0x" + toHex(base_ + pos_));
    }

    static std::string toHex(std::size_t v) {
        static constexpr char digits[] = "0123456789abcdef";
        std::string s;
        do {
            s.insert(s.begin(), digits[v & 0xFU]);
            v >>= 4U;
        } while (v != 0);
        return s;
    }

    std::span<const std::byte> data_;
    std::size_t base_ = 0;
    std::size_t pos_ = 0;
};

} // namespace fable::assets
