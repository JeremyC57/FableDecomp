#include "fable/oracle/RetailOracle.hpp"

#include <unicorn/unicorn.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>

namespace fable::oracle {
namespace {

// Address-space layout of the emulated process (all outside the image).
constexpr std::uint32_t kStackBase = 0x0F000000;
constexpr std::uint32_t kStackSize = 0x00100000;
constexpr std::uint32_t kHeapBase = 0x10000000;
constexpr std::uint32_t kHeapSize = 0x04000000;
constexpr std::uint32_t kTrapBase = 0x7E000000;   // one 4-byte slot per import
constexpr std::uint32_t kTrapSize = 0x00010000;
constexpr std::uint32_t kPrologue = kTrapBase + 0xF000;  // fninit; fldcw; jmp [target]
constexpr std::uint32_t kPrologueData = kTrapBase + 0xF100;
constexpr std::uint32_t kSentinel = kTrapBase + 0xFFF0;  // return address of every call
constexpr std::uint32_t kGdtBase = 0x7FFC0000;
constexpr std::uint32_t kTebBase = 0x7FFDE000;
constexpr std::uint16_t kMsvc71ControlWord = 0x027F;  // 53-bit precision, all exceptions masked

std::uint32_t le32(const std::vector<std::byte>& f, std::size_t at) {
    if (at + 4 > f.size()) {
        throw OracleError("PE truncated");
    }
    std::uint32_t v;
    std::memcpy(&v, f.data() + at, 4);
    return v;
}

std::uint16_t le16(const std::vector<std::byte>& f, std::size_t at) {
    if (at + 2 > f.size()) {
        throw OracleError("PE truncated");
    }
    std::uint16_t v;
    std::memcpy(&v, f.data() + at, 2);
    return v;
}

void check(uc_err err, const char* what) {
    if (err != UC_ERR_OK) {
        throw OracleError(std::string(what) + ": " + uc_strerror(err));
    }
}

std::uint64_t gdtEntry(std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::uint8_t flags) {
    std::uint64_t e = limit & 0xFFFFU;
    e |= std::uint64_t{base & 0xFFFFFFU} << 16U;
    e |= std::uint64_t{access} << 40U;
    e |= std::uint64_t{(limit >> 16U) & 0xFU} << 48U;
    e |= std::uint64_t{flags & 0xFU} << 52U;
    e |= std::uint64_t{(base >> 24U) & 0xFFU} << 56U;
    return e;
}

double x87ToDouble(const std::uint8_t raw[10]) {
    std::uint64_t mantissa;
    std::uint16_t se;
    std::memcpy(&mantissa, raw, 8);
    std::memcpy(&se, raw + 8, 2);
    const bool negative = (se & 0x8000U) != 0;
    const int exponent = se & 0x7FFF;
    double v;
    if (exponent == 0 && mantissa == 0) {
        v = 0.0;
    } else if (exponent == 0x7FFF) {
        v = (mantissa << 1U) == 0 ? INFINITY : NAN;
    } else {
        v = std::ldexp(static_cast<double>(mantissa), exponent - 16383 - 63);
    }
    return negative ? -v : v;
}

} // namespace

RetailOracle::RetailOracle(const std::filesystem::path& exe) {
    std::ifstream in(exe, std::ios::binary);
    if (!in) {
        throw OracleError("cannot open " + exe.string());
    }
    std::vector<std::byte> file(std::filesystem::file_size(exe));
    in.read(reinterpret_cast<char*>(file.data()), static_cast<std::streamsize>(file.size()));
    if (file.size() < 0x40 || le16(file, 0) != 0x5A4D) {
        throw OracleError(exe.string() + " is not a PE executable");
    }
    check(uc_open(UC_ARCH_X86, UC_MODE_32, &uc_), "uc_open");
    mapImage(file);
    bindImports(file);
    setupSegments();

    check(uc_mem_map(uc_, kStackBase, kStackSize, UC_PROT_READ | UC_PROT_WRITE), "map stack");
    check(uc_mem_map(uc_, kHeapBase, kHeapSize, UC_PROT_READ | UC_PROT_WRITE), "map heap");
    heapNext_ = kHeapBase;

    // Prologue: fninit; fldcw [kPrologueData]; jmp dword [kPrologueData + 4]
    const std::uint8_t prologue[] = {0xDB, 0xE3, 0xD9, 0x2D, 0, 0, 0, 0, 0xFF, 0x25, 0, 0, 0, 0};
    std::uint8_t code[sizeof prologue];
    std::memcpy(code, prologue, sizeof prologue);
    const std::uint32_t cwAddr = kPrologueData;
    const std::uint32_t targetAddr = kPrologueData + 4;
    std::memcpy(code + 4, &cwAddr, 4);
    std::memcpy(code + 10, &targetAddr, 4);
    write(kPrologue, code, sizeof code);
    write(kPrologueData, &kMsvc71ControlWord, 2);

    uc_hook hook;
    check(uc_hook_add(uc_, &hook, UC_HOOK_CODE, reinterpret_cast<void*>(&RetailOracle::codeHook), this,
                      kTrapBase, kTrapBase + kTrapSize - 1),
          "hook traps");
    check(uc_hook_add(uc_, &hook, UC_HOOK_MEM_INVALID, reinterpret_cast<void*>(&RetailOracle::invalidHook),
                      this, 1, 0),
          "hook invalid");

    for (auto& s : sections_) {
        if (s.writable) {
            s.pristine.resize(s.size);
            read(s.va, s.pristine.data(), s.size);
        }
    }
    installDefaultStubs();
}

RetailOracle::~RetailOracle() {
    if (uc_ != nullptr) {
        uc_close(uc_);
    }
}

void RetailOracle::mapImage(const std::vector<std::byte>& file) {
    const std::uint32_t pe = le32(file, 0x3C);
    if (le32(file, pe) != 0x00004550 || le16(file, pe + 4) != 0x014C) {
        throw OracleError("not a PE32 x86 image");
    }
    const std::uint16_t sectionCount = le16(file, pe + 6);
    const std::uint16_t optSize = le16(file, pe + 20);
    const std::size_t opt = pe + 24;
    if (le32(file, opt + 28) != kImageBase) {
        throw OracleError("unexpected image base (the oracle expects 0x00400000)");
    }
    const std::uint32_t sizeOfImage = le32(file, opt + 56);
    const std::uint32_t sizeOfHeaders = le32(file, opt + 60);
    const std::uint32_t mapped = (sizeOfImage + 0xFFFU) & ~0xFFFU;
    check(uc_mem_map(uc_, kImageBase, mapped, UC_PROT_ALL), "map image");
    write(kImageBase, file.data(), std::min<std::size_t>(sizeOfHeaders, file.size()));

    const std::size_t secTable = opt + optSize;
    for (std::uint16_t i = 0; i < sectionCount; ++i) {
        const std::size_t s = secTable + std::size_t{i} * 40;
        const std::uint32_t vsize = le32(file, s + 8);
        const std::uint32_t va = le32(file, s + 12);
        const std::uint32_t rawSize = le32(file, s + 16);
        const std::uint32_t rawPtr = le32(file, s + 20);
        const std::uint32_t flags = le32(file, s + 36);
        const std::uint32_t copy = std::min(vsize == 0 ? rawSize : vsize, rawSize);
        if (rawPtr + std::size_t{copy} > file.size()) {
            throw OracleError("section data outside the file");
        }
        if (copy != 0) {
            write(kImageBase + va, file.data() + rawPtr, copy);
        }
        sections_.push_back({kImageBase + va, std::max(vsize, rawSize), (flags & 0x80000000U) != 0, {}});
    }
}

void RetailOracle::bindImports(const std::vector<std::byte>& file) {
    check(uc_mem_map(uc_, kTrapBase, kTrapSize, UC_PROT_ALL), "map traps");
    const std::uint32_t pe = le32(file, 0x3C);
    const std::size_t opt = pe + 24;
    const std::uint32_t importRva = le32(file, opt + 96 + 8);
    if (importRva == 0) {
        return;
    }
    auto readMem32 = [&](std::uint32_t rva) { return get<std::uint32_t>(kImageBase + rva); };
    auto readName = [&](std::uint32_t rva) {
        std::string s;
        for (std::uint32_t a = kImageBase + rva;; ++a) {
            const auto c = get<char>(a);
            if (c == '\0') {
                return s;
            }
            s.push_back(c);
        }
    };
    std::uint32_t slot = kTrapBase;
    for (std::uint32_t desc = importRva;; desc += 20) {
        const std::uint32_t nameRva = readMem32(desc + 12);
        if (nameRva == 0) {
            break;
        }
        const std::string dll = readName(nameRva);
        const std::uint32_t lookup = readMem32(desc) != 0 ? readMem32(desc) : readMem32(desc + 16);
        const std::uint32_t iat = readMem32(desc + 16);
        for (std::uint32_t i = 0;; ++i) {
            const std::uint32_t entry = readMem32(lookup + i * 4);
            if (entry == 0) {
                break;
            }
            std::string name = (entry & 0x80000000U) != 0 ? "#" + std::to_string(entry & 0xFFFFU)
                                                          : readName(entry + 2);
            if (slot >= kPrologue) {
                throw OracleError("too many imports for the trap region");
            }
            thunkNames_[slot] = dll + "!" + name;
            put<std::uint32_t>(kImageBase + iat + i * 4, slot);
            const std::uint8_t ret = 0xC3;
            write(slot, &ret, 1);
            slot += 4;
        }
    }
}

void RetailOracle::setupSegments() {
    check(uc_mem_map(uc_, kGdtBase, 0x1000, UC_PROT_READ | UC_PROT_WRITE), "map gdt");
    check(uc_mem_map(uc_, kTebBase, 0x2000, UC_PROT_READ | UC_PROT_WRITE), "map teb");
    constexpr std::uint8_t kPresent = 0x80;
    constexpr std::uint8_t kCodeData = 0x10;
    constexpr std::uint8_t kCode = 0x08;
    constexpr std::uint8_t kRw = 0x02;
    constexpr std::uint8_t kFlags32 = 0x0C;  // 4 KiB granularity, 32-bit
    const std::uint64_t gdt[5] = {
        0,
        gdtEntry(0, 0xFFFFF, kPresent | kCodeData | kCode | kRw, kFlags32),  // 0x08 code
        gdtEntry(0, 0xFFFFF, kPresent | kCodeData | kRw, kFlags32),          // 0x10 data/stack
        gdtEntry(kTebBase, 0xFFF, kPresent | kCodeData | kRw, 0x04),         // 0x18 fs -> TEB
        0,
    };
    write(kGdtBase, gdt, sizeof gdt);
    uc_x86_mmr gdtr{};
    gdtr.base = kGdtBase;
    gdtr.limit = sizeof gdt - 1;
    check(uc_reg_write(uc_, UC_X86_REG_GDTR, &gdtr), "gdtr");
    const int segs[][2] = {{UC_X86_REG_SS, 0x10}, {UC_X86_REG_DS, 0x10}, {UC_X86_REG_ES, 0x10},
                           {UC_X86_REG_GS, 0x10}, {UC_X86_REG_FS, 0x18}, {UC_X86_REG_CS, 0x08}};
    for (const auto& [reg, sel] : segs) {
        check(uc_reg_write(uc_, reg, &sel), "segment");
    }
    // Minimal TEB: SEH chain end, stack bounds, self pointer.
    put<std::uint32_t>(kTebBase + 0x00, 0xFFFFFFFFU);
    put<std::uint32_t>(kTebBase + 0x04, kStackBase + kStackSize);
    put<std::uint32_t>(kTebBase + 0x08, kStackBase);
    put<std::uint32_t>(kTebBase + 0x18, kTebBase);
}

void RetailOracle::reset() {
    for (const auto& s : sections_) {
        if (s.writable) {
            write(s.va, s.pristine.data(), s.pristine.size());
        }
    }
    std::vector<std::byte> zero(heapNext_ - kHeapBase);
    if (!zero.empty()) {
        write(kHeapBase, zero.data(), zero.size());
    }
    heapNext_ = kHeapBase;
}

std::uint32_t RetailOracle::alloc(std::size_t size, std::uint32_t align) {
    const std::uint32_t at = (heapNext_ + align - 1) & ~(align - 1);
    if (at + size > kHeapBase + kHeapSize) {
        throw OracleError("oracle scratch heap exhausted");
    }
    heapNext_ = at + static_cast<std::uint32_t>(size);
    return at;
}

void RetailOracle::write(std::uint32_t address, const void* data, std::size_t size) {
    check(uc_mem_write(uc_, address, data, size), "mem write");
}

void RetailOracle::read(std::uint32_t address, void* out, std::size_t size) const {
    check(uc_mem_read(uc_, address, out, size), "mem read");
}

std::uint32_t RetailOracle::allocBytes(std::span<const std::byte> bytes) {
    const auto at = alloc(bytes.size() + 1);
    write(at, bytes.data(), bytes.size());
    return at;
}

std::uint32_t RetailOracle::allocString(const std::string& s) {
    const auto at = alloc(s.size() + 1);
    write(at, s.c_str(), s.size() + 1);
    return at;
}

std::vector<std::string> RetailOracle::imports() const {
    std::vector<std::string> out;
    for (const auto& [addr, name] : thunkNames_) {
        out.push_back(name);
    }
    return out;
}

void RetailOracle::stubImport(const std::string& qualifiedName, ImportStub stub) {
    stubs_[qualifiedName] = std::move(stub);
}

std::uint32_t RetailOracle::arg(std::size_t index) const {
    return get<std::uint32_t>(stubEsp_ + 4 + static_cast<std::uint32_t>(index) * 4);
}

CallResult RetailOracle::call(std::uint32_t function, CallingConvention cc,
                              std::span<const std::uint32_t> stackArgs, std::uint32_t ecx, std::uint32_t edx) {
    CallResult result;
    current_ = &result;
    pendingError_.clear();

    std::uint32_t esp = kStackBase + kStackSize - 0x1000;
    for (auto it = stackArgs.rbegin(); it != stackArgs.rend(); ++it) {
        esp -= 4;
        put(esp, *it);
    }
    esp -= 4;
    put(esp, kSentinel);
    const std::uint32_t expectedEsp =
        esp + 4 + ((cc == CallingConvention::Cdecl) ? 0U : static_cast<std::uint32_t>(stackArgs.size() * 4));

    const std::uint32_t zero = 0;
    for (const int reg : {UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP}) {
        check(uc_reg_write(uc_, reg, &zero), "reg");
    }
    check(uc_reg_write(uc_, UC_X86_REG_ECX, &ecx), "ecx");
    check(uc_reg_write(uc_, UC_X86_REG_EDX, &edx), "edx");
    check(uc_reg_write(uc_, UC_X86_REG_ESP, &esp), "esp");
    const std::uint32_t eflags = 0x202;
    check(uc_reg_write(uc_, UC_X86_REG_EFLAGS, &eflags), "eflags");
    put(kPrologueData + 4, function);

    const uc_err err = uc_emu_start(uc_, kPrologue, kSentinel, 0, budget_);
    current_ = nullptr;
    if (!pendingError_.empty()) {
        throw OracleError(pendingError_);
    }
    check(err, "emulation");
    std::uint32_t eip = 0;
    uc_reg_read(uc_, UC_X86_REG_EIP, &eip);
    if (eip != kSentinel) {
        throw OracleError("instruction budget exhausted or emulation stopped early at 0x" +
                          std::to_string(eip));
    }
    std::uint32_t finalEsp = 0;
    uc_reg_read(uc_, UC_X86_REG_ESP, &finalEsp);
    if (finalEsp != expectedEsp) {
        throw OracleError("stack imbalance after call: wrong calling convention or argument count");
    }
    uc_reg_read(uc_, UC_X86_REG_EAX, &result.eax);
    uc_reg_read(uc_, UC_X86_REG_EDX, &result.edx);

    std::uint32_t fpsw = 0;
    uc_reg_read(uc_, UC_X86_REG_FPSW, &fpsw);
    const unsigned top = (fpsw >> 11U) & 7U;
    if (top == 7) {  // exactly one value pushed since fninit
        std::uint8_t raw[16] = {};
        uc_reg_read(uc_, UC_X86_REG_FP0 + 7, raw);
        result.st0 = x87ToDouble(raw);
        result.st0Valid = true;
    }
    return result;
}

void RetailOracle::runUntil(std::uint32_t start, std::uint32_t stop) {
    pendingError_.clear();
    std::uint32_t esp = kStackBase + kStackSize - 0x1000;
    check(uc_reg_write(uc_, UC_X86_REG_ESP, &esp), "esp");
    put(kPrologueData + 4, start);
    const uc_err err = uc_emu_start(uc_, kPrologue, stop, 0, budget_);
    if (!pendingError_.empty()) {
        throw OracleError(pendingError_);
    }
    check(err, "emulation");
    std::uint32_t eip = 0;
    uc_reg_read(uc_, UC_X86_REG_EIP, &eip);
    if (eip != stop) {
        throw OracleError("runUntil stopped before reaching the stop address");
    }
}

void RetailOracle::codeHook(uc_struct*, std::uint64_t address, std::uint32_t, void* self) {
    static_cast<RetailOracle*>(self)->onCode(address);
}

bool RetailOracle::invalidHook(uc_struct* uc, int type, std::uint64_t address, int size, std::int64_t,
                               void* self) {
    auto* o = static_cast<RetailOracle*>(self);
    std::uint32_t eip = 0;
    uc_reg_read(uc, UC_X86_REG_EIP, &eip);
    const char* kind = type == UC_MEM_FETCH_UNMAPPED ? "fetch" : type == UC_MEM_WRITE_UNMAPPED ? "write" : "read";
    char buf[160];
    std::snprintf(buf, sizeof buf, "unmapped %s of %d bytes at 0x%08llx (eip 0x%08x)", kind, size,
                  static_cast<unsigned long long>(address), eip);
    o->pendingError_ = buf;
    return false;
}

void RetailOracle::onCode(std::uint64_t address) {
    const auto a = static_cast<std::uint32_t>(address);
    if (a == kSentinel || a < kTrapBase || a >= kPrologue) {
        return;  // sentinel stop is handled by uc_emu_start's `until`; prologue runs normally
    }
    const auto name = thunkNames_.find(a);
    if (name == thunkNames_.end()) {
        pendingError_ = "jump into the trap region at an unknown slot";
        uc_emu_stop(uc_);
        return;
    }
    const auto stub = stubs_.find(name->second);
    if (stub == stubs_.end()) {
        pendingError_ = "call to unstubbed import " + name->second;
        uc_emu_stop(uc_);
        return;
    }
    uc_reg_read(uc_, UC_X86_REG_ESP, &stubEsp_);
    if (current_ != nullptr) {
        current_->importsCalled.push_back(name->second);
    }
    const std::uint32_t eax = stub->second.handler(*this);
    const std::uint32_t ret = get<std::uint32_t>(stubEsp_);
    const std::uint32_t esp = stubEsp_ + 4 + stub->second.popBytes;
    uc_reg_write(uc_, UC_X86_REG_EAX, &eax);
    uc_reg_write(uc_, UC_X86_REG_ESP, &esp);
    uc_reg_write(uc_, UC_X86_REG_EIP, &ret);
}

void RetailOracle::installDefaultStubs() {
    // C runtime helpers commonly reached from leaf utilities (all cdecl).
    auto copy = [](RetailOracle& o, std::uint32_t dst, std::uint32_t src, std::uint32_t n) {
        std::vector<std::byte> tmp(n);
        o.read(src, tmp.data(), n);
        o.write(dst, tmp.data(), n);
    };
    stubImport("MSVCR71.dll!memcpy", {[copy](RetailOracle& o) { copy(o, o.arg(0), o.arg(1), o.arg(2)); return o.arg(0); }});
    stubImport("MSVCR71.dll!memmove", {[copy](RetailOracle& o) { copy(o, o.arg(0), o.arg(1), o.arg(2)); return o.arg(0); }});
    stubImport("MSVCR71.dll!memset", {[](RetailOracle& o) {
        std::vector<std::uint8_t> tmp(o.arg(2), static_cast<std::uint8_t>(o.arg(1)));
        o.write(o.arg(0), tmp.data(), tmp.size());
        return o.arg(0);
    }});
    stubImport("MSVCR71.dll!strlen", {[](RetailOracle& o) {
        std::uint32_t n = 0;
        while (o.get<char>(o.arg(0) + n) != '\0') {
            ++n;
        }
        return n;
    }});
    auto alloc = [](RetailOracle& o) { return o.alloc(std::max<std::uint32_t>(o.arg(0), 1)); };
    stubImport("MSVCR71.dll!malloc", {alloc});
    stubImport("MSVCR71.dll!??2@YAPAXI@Z", {alloc});  // operator new
    stubImport("MSVCR71.dll!free", {[](RetailOracle&) { return 0U; }});
    stubImport("MSVCR71.dll!??3@YAXPAX@Z", {[](RetailOracle&) { return 0U; }});  // operator delete
}

} // namespace fable::oracle
