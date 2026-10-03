#pragma once

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace recomp {

/// The retail PE image as the loader would map it (sections at their RVAs).
class Image {
public:
    explicit Image(const std::filesystem::path& exe) {
        std::ifstream in(exe, std::ios::binary);
        if (!in) throw std::runtime_error("cannot open " + exe.string());
        std::vector<uint8_t> file(std::filesystem::file_size(exe));
        in.read(reinterpret_cast<char*>(file.data()), static_cast<std::streamsize>(file.size()));
        const uint32_t pe = le32(file, 0x3C);
        if (le32(file, pe) != 0x00004550) throw std::runtime_error("not a PE");
        const uint16_t sections = le16(file, pe + 6);
        const uint16_t optSize = le16(file, pe + 20);
        const size_t opt = pe + 24;
        base_ = le32(file, opt + 28);
        entry_ = base_ + le32(file, opt + 16);
        const uint32_t sizeOfImage = le32(file, opt + 56);
        mem_.assign(sizeOfImage, 0);
        const uint32_t headers = le32(file, opt + 60);
        std::memcpy(mem_.data(), file.data(), std::min<size_t>(headers, file.size()));
        for (uint16_t i = 0; i < sections; ++i) {
            const size_t s = opt + optSize + size_t{i} * 40;
            Section sec;
            sec.name = std::string(reinterpret_cast<const char*>(&file[s]), strnlen(reinterpret_cast<const char*>(&file[s]), 8));
            sec.vsize = le32(file, s + 8);
            sec.va = le32(file, s + 12);
            const uint32_t raw = le32(file, s + 16), ptr = le32(file, s + 20);
            sec.flags = le32(file, s + 36);
            const uint32_t copy = std::min(raw, sec.vsize ? sec.vsize : raw);
            if (copy) std::memcpy(&mem_[sec.va], &file[ptr], copy);
            sections_.push_back(sec);
        }
        // Exports (DLLs) and base relocations: precise pointers into code.
        const uint32_t exportRva = le32(file, opt + 96), exportSize = le32(file, opt + 100);
        if (exportRva) {
            const uint32_t n = r32(base_ + exportRva + 20), funcs = r32(base_ + exportRva + 28);
            for (uint32_t i = 0; i < n; ++i) {
                const uint32_t rva = r32(base_ + funcs + 4 * i);
                if (rva && !(rva >= exportRva && rva < exportRva + exportSize)) exports_.push_back(base_ + rva);  // skip forwarders
            }
        }
        const uint32_t relocRva = le32(file, opt + 96 + 40), relocSize = le32(file, opt + 96 + 44);
        for (uint32_t b = relocRva; relocRva && b < relocRva + relocSize;) {
            const uint32_t page = r32(base_ + b), blockSize = r32(base_ + b + 4);
            if (blockSize < 8) break;
            for (uint32_t e = 8; e + 2 <= blockSize; e += 2) {
                uint16_t v;
                std::memcpy(&v, at(base_ + b + e), 2);
                if ((v >> 12) == 3) relocTargets_.push_back(r32(base_ + page + (v & 0xFFF)));  // IMAGE_REL_BASED_HIGHLOW
            }
            b += blockSize;
        }
        // Imports: IAT slot address -> "DLL!name".
        const uint32_t importRva = le32(file, opt + 96 + 8);
        for (uint32_t d = importRva; importRva; d += 20) {
            const uint32_t nameRva = r32(base_ + d + 12);
            if (!nameRva) break;
            const std::string dll = cstr(base_ + nameRva);
            const uint32_t lookup = r32(base_ + d) ? r32(base_ + d) : r32(base_ + d + 16);
            const uint32_t iat = r32(base_ + d + 16);
            for (uint32_t i = 0;; ++i) {
                const uint32_t e = r32(base_ + lookup + i * 4);
                if (!e) break;
                const std::string fn = (e & 0x80000000u) ? "#" + std::to_string(e & 0xFFFF) : cstr(base_ + e + 2);
                imports_[base_ + iat + i * 4] = dll + "!" + fn;
            }
        }
    }

    uint32_t base() const { return base_; }
    uint32_t entry() const { return entry_; }
    uint32_t end() const { return base_ + static_cast<uint32_t>(mem_.size()); }
    bool contains(uint32_t a, uint32_t n = 1) const { return a >= base_ && a - base_ + n <= mem_.size(); }
    const uint8_t* at(uint32_t a) const { return &mem_[a - base_]; }
    uint32_t r32(uint32_t a) const { uint32_t v; std::memcpy(&v, at(a), 4); return v; }
    uint8_t r8(uint32_t a) const { return *at(a); }
    std::string cstr(uint32_t a) const { return std::string(reinterpret_cast<const char*>(at(a))); }

    bool isCode(uint32_t a) const {
        for (const auto& s : sections_)
            if ((s.flags & 0x20000000u) && a >= base_ + s.va && a < base_ + s.va + s.vsize) return true;
        return false;
    }
    const std::map<uint32_t, std::string>& imports() const { return imports_; }
    const std::vector<uint32_t>& exports() const { return exports_; }
    const std::vector<uint32_t>& relocTargets() const { return relocTargets_; }
    struct Section { std::string name; uint32_t va = 0, vsize = 0, flags = 0; };  // va is an RVA
    const std::vector<Section>& sections() const { return sections_; }

private:
    static uint32_t le32(const std::vector<uint8_t>& f, size_t o) { uint32_t v; std::memcpy(&v, &f[o], 4); return v; }
    static uint16_t le16(const std::vector<uint8_t>& f, size_t o) { uint16_t v; std::memcpy(&v, &f[o], 2); return v; }

    uint32_t base_ = 0, entry_ = 0;
    std::vector<uint8_t> mem_;
    std::vector<Section> sections_;
    std::map<uint32_t, std::string> imports_;
    std::vector<uint32_t> exports_, relocTargets_;
};

} // namespace recomp
