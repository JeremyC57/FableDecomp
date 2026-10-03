#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

struct uc_struct;

namespace fable::oracle {

/// Thrown when a retail call leaves the pure-CPU boundary the oracle supports
/// (an unstubbed import, an unmapped access, the instruction budget...).
class OracleError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

enum class CallingConvention { Cdecl, Stdcall, Thiscall, Fastcall };

/// Result of one retail call. `st0` is valid when the function returned on the
/// x87 stack (float/double results under the VC7.1 ABI).
struct CallResult {
    std::uint32_t eax = 0;
    std::uint32_t edx = 0;
    double st0 = 0.0;
    bool st0Valid = false;
    std::vector<std::string> importsCalled;  ///< stubbed imports hit, in order
};

/// A behavioural oracle: executes functions of the user's retail Fable.exe in
/// Unicorn so portable reimplementations can be compared on identical inputs.
///
/// Developer tool only — never linked into the port. Loads the user's own
/// executable from disk; nothing retail is embedded or written out.
class RetailOracle {
public:
    static constexpr std::uint32_t kImageBase = 0x00400000;

    /// Handler for an imported function: read arguments with `arg(i)`, return EAX.
    /// `popBytes` is how many argument bytes the callee removes (stdcall); 0 for cdecl.
    struct ImportStub {
        std::function<std::uint32_t(RetailOracle&)> handler;
        std::uint32_t popBytes = 0;
    };

    struct Config {
        std::uint32_t stackSize = 0x00100000;  ///< mapped just below 0x0F100000
        std::uint32_t heapSize = 0x04000000;   ///< mapped at 0x10000000
        bool protectSections = true;           ///< read-only code/rodata, like Windows
    };

    explicit RetailOracle(const std::filesystem::path& exe) : RetailOracle(exe, Config{}) {}
    RetailOracle(const std::filesystem::path& exe, const Config& config);
    ~RetailOracle();
    RetailOracle(const RetailOracle&) = delete;
    RetailOracle& operator=(const RetailOracle&) = delete;

    /// Restore writable sections and the scratch heap to their freshly-loaded state.
    void reset();

    // ---- scratch memory -------------------------------------------------
    [[nodiscard]] std::uint32_t alloc(std::size_t size, std::uint32_t align = 16);
    void write(std::uint32_t address, const void* data, std::size_t size);
    void read(std::uint32_t address, void* out, std::size_t size) const;
    [[nodiscard]] std::uint32_t allocBytes(std::span<const std::byte> bytes);
    [[nodiscard]] std::uint32_t allocString(const std::string& s);

    template <typename T>
    void put(std::uint32_t address, const T& value) { write(address, &value, sizeof value); }
    template <typename T>
    [[nodiscard]] T get(std::uint32_t address) const {
        T v{};
        read(address, &v, sizeof v);
        return v;
    }

    // ---- calls ----------------------------------------------------------
    /// Call `function` with 32-bit stack slots. For Thiscall pass `this` in `ecx`;
    /// for Fastcall pass the first two arguments in `ecx`/`edx`.
    CallResult call(std::uint32_t function, CallingConvention cc, std::span<const std::uint32_t> stackArgs,
                    std::uint32_t ecx = 0, std::uint32_t edx = 0);

    /// Run retail code from `start` until execution reaches `stop` (a slice of a
    /// function, e.g. an initializer loop that precedes an unsupported call).
    void runUntil(std::uint32_t start, std::uint32_t stop);

    /// Registers an import stub by DLL-qualified name, e.g. "MSVCR71.dll!memcpy".
    void stubImport(const std::string& qualifiedName, ImportStub stub);
    /// Imports the executable references, as "DLL!name" (or "DLL!#ordinal").
    [[nodiscard]] std::vector<std::string> imports() const;

    /// Inside an import stub: the i-th 32-bit argument of the intercepted call.
    [[nodiscard]] std::uint32_t arg(std::size_t index) const;

    void setInstructionBudget(std::uint64_t budget) { budget_ = budget; }

    static std::uint32_t bits(float f) {
        std::uint32_t u;
        std::memcpy(&u, &f, 4);
        return u;
    }
    static float asFloat(std::uint32_t u) {
        float f;
        std::memcpy(&f, &u, 4);
        return f;
    }

private:
    struct Section {
        std::uint32_t va;
        std::uint32_t size;
        bool writable;
        std::vector<std::byte> pristine;
    };

    void mapImage(const std::vector<std::byte>& file);
    void bindImports(const std::vector<std::byte>& file);
    void setupSegments();
    void installDefaultStubs();
    void onCode(std::uint64_t address);
    static void codeHook(uc_struct* uc, std::uint64_t address, std::uint32_t size, void* self);
    static bool invalidHook(uc_struct* uc, int type, std::uint64_t address, int size, std::int64_t value, void* self);

    uc_struct* uc_ = nullptr;
    std::vector<Section> sections_;
    std::map<std::uint32_t, std::string> thunkNames_;  ///< trap address -> DLL!name
    std::map<std::string, ImportStub> stubs_;
    std::uint32_t heapNext_ = 0;
    Config config_{};
    std::uint64_t budget_ = 50'000'000;

    // per-call state
    CallResult* current_ = nullptr;
    std::string pendingError_;
    std::uint32_t stubEsp_ = 0;
};

} // namespace fable::oracle
