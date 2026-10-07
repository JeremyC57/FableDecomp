#include "fable/assets/Lzo.hpp"

#include "fable/assets/ByteReader.hpp"

#include <cstdint>
#include <string>

namespace fable::assets {
namespace {

// Standard LZO1X grammar (miniLZO `lzo1x_decompress_safe`), which the engine
// ships at 0x00C08170. Written as an explicit state machine instead of gotos.
class LzoDecoder {
public:
    LzoDecoder(std::span<const std::byte> in, std::size_t maxOut) : in_(in), maxOut_(maxOut) {
        out_.reserve(maxOut);
    }

    LzoResult run() {
        State state = State::Loop;
        std::size_t t = 0;

        if (peek() > 17) {
            t = next() - 17U;
            if (t < 4) {
                state = State::MatchNext;
            } else {
                copyLiterals(t);
                state = State::FirstLiteralRun;
            }
        }

        for (;;) {
            switch (state) {
            case State::Loop:
                t = next();
                if (t >= 16) {
                    state = State::Match;
                    break;
                }
                if (t == 0) {
                    t = 15 + extendedLength();
                }
                copyLiterals(t + 3);
                state = State::FirstLiteralRun;
                break;

            case State::FirstLiteralRun:
                t = next();
                if (t >= 16) {
                    state = State::Match;
                    break;
                }
                // Short match directly after a literal run: length 3, distance >= 0x801.
                copyMatch(1 + 0x0800 + (t >> 2U) + (std::size_t{next()} << 2U), 3);
                state = trailingLiterals(t);
                break;

            case State::Match: {
                std::size_t dist = 0;
                std::size_t len = 0;
                if (t >= 64) {  // M2: length 3..8, 11-bit distance
                    dist = 1 + ((t >> 2U) & 7U) + (std::size_t{next()} << 3U);
                    len = (t >> 5U) + 1;
                } else if (t >= 32) {  // M3: 14-bit distance
                    len = t & 31U;
                    if (len == 0) {
                        len = 31 + extendedLength();
                    }
                    len += 2;
                    dist = 1 + readDistance14();
                } else if (t >= 16) {  // M4: 16..48 KiB distance, or end of stream
                    len = t & 7U;
                    if (len == 0) {
                        len = 7 + extendedLength();
                    }
                    len += 2;
                    dist = ((t & 8U) << 11U) + readDistance14();
                    if (dist == 0) {
                        return {std::move(out_), pos_};
                    }
                    dist += 0x4000;
                } else {  // M1: length 2 inside a trailing-literal chain
                    dist = 1 + (t >> 2U) + (std::size_t{next()} << 2U);
                    len = 2;
                }
                copyMatch(dist, len);
                state = trailingLiterals(t);
                break;
            }

            case State::MatchNext:
                copyLiterals(t);
                t = next();
                state = State::Match;
                break;
            }
        }
    }

private:
    enum class State { Loop, FirstLiteralRun, Match, MatchNext };

    // After any match, the low 2 bits of the byte two positions back give 0..3
    // trailing literals. Non-zero chains straight into another match.
    State trailingLiterals(std::size_t& t) {
        t = std::to_integer<std::size_t>(in_[pos_ - 2]) & 3U;
        return t == 0 ? State::Loop : State::MatchNext;
    }

    [[nodiscard]] std::uint8_t peek() const {
        if (pos_ >= in_.size()) {
            fail("input overrun");
        }
        return std::to_integer<std::uint8_t>(in_[pos_]);
    }

    std::uint8_t next() {
        const auto v = peek();
        ++pos_;
        return v;
    }

    std::size_t extendedLength() {
        std::size_t extra = 0;
        while (peek() == 0) {
            extra += 255;
            ++pos_;
            if (extra > maxOut_) {
                fail("length overflow");
            }
        }
        return extra + next();
    }

    std::size_t readDistance14() {
        const std::size_t lo = next();
        const std::size_t hi = next();
        return (lo >> 2U) + (hi << 6U);
    }

    void copyLiterals(std::size_t n) {
        if (n > in_.size() - pos_) {
            fail("literal run past end of input");
        }
        if (out_.size() + n > maxOut_) {
            fail("output overrun");
        }
        const auto first = in_.begin() + static_cast<std::ptrdiff_t>(pos_);
        out_.insert(out_.end(), first, first + static_cast<std::ptrdiff_t>(n));
        pos_ += n;
    }

    void copyMatch(std::size_t dist, std::size_t len) {
        if (dist > out_.size()) {
            fail("match distance before start of output");
        }
        if (out_.size() + len > maxOut_) {
            fail("output overrun");
        }
        auto from = out_.size() - dist;
        for (std::size_t i = 0; i < len; ++i) {
            out_.push_back(out_[from++]);  // byte-wise: overlapping copies are intended
        }
    }

    [[noreturn]] void fail(const char* why) const {
        throw AssetError(std::string("lzo1x: ") + why + " (input offset " + std::to_string(pos_) +
                         ", output " + std::to_string(out_.size()) + ")");
    }

    std::span<const std::byte> in_;
    std::size_t maxOut_;
    std::size_t pos_ = 0;
    std::vector<std::byte> out_;
};

} // namespace

LzoResult lzo1xDecompress(std::span<const std::byte> input, std::size_t maxOutput) {
    return LzoDecoder(input, maxOutput).run();
}

LzoResult decodeChunkedLzo(std::span<const std::byte> input, std::size_t rawSize) {
    LzoResult result;
    if (rawSize < 3) {
        throw AssetError("chunked lzo: raw size below the 3-byte plain tail");
    }
    const std::size_t compressedTarget = rawSize - 3;
    ByteReader r(input);
    result.output.reserve(rawSize);

    while (result.output.size() < compressedTarget) {
        std::size_t clen = r.u16();
        if (clen == 0xFFFF) {
            clen = r.u32();
        }
        const auto block = r.bytes(clen);
        const std::size_t room = compressedTarget - result.output.size();
        if (clen == 0) {  // stored chunk (docs/formats/TEXTURE.md): the rest of the mip, raw
            const auto raw = r.bytes(room);
            result.output.insert(result.output.end(), raw.begin(), raw.end());
            break;
        }
        auto decoded = lzo1xDecompress(block, room);
        if (decoded.consumed != clen) {
            throw AssetError("chunked lzo: block of " + std::to_string(clen) + " bytes ended after " +
                             std::to_string(decoded.consumed));
        }
        if (decoded.output.empty()) {
            throw AssetError("chunked lzo: block produced no output");
        }
        result.output.insert(result.output.end(), decoded.output.begin(), decoded.output.end());
    }

    const auto tail = r.bytes(3);
    result.output.insert(result.output.end(), tail.begin(), tail.end());
    result.consumed = r.position();
    return result;
}

} // namespace fable::assets
