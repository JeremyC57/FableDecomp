// PE images in guest memory: Fable.exe and the recompiled helper DLLs from the
// game folder. Images are mapped as data at their preferred base (the code that
// runs is the recompiled C, which uses absolute guest addresses); imports are
// bound to host traps.
#include "host.hpp"

#include <map>
#include <mutex>
#include <vector>

extern "C" {
typedef struct RecompEntry {
    uint32_t addr;
    GuestFn fn;
} RecompEntry;
typedef struct GuestDllTable {
    const char* file;
    const RecompEntry* table;
    const uint32_t* size;
} GuestDllTable;
extern const GuestDllTable guest_dll_tables[];
extern const uint32_t guest_dll_count;
void recomp_register_table(const RecompEntry* table, uint32_t size);
}

namespace host {

namespace {
struct LoadedDll {
    std::string name;
    uint32_t base = 0, size = 0;
};
std::map<std::string, LoadedDll> g_loaded;  // lower-case file name
std::recursive_mutex g_loadLock;

std::string lower(std::string s) {
    for (auto& ch : s) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return s;
}
std::string baseName(const std::string& p) {
    const auto i = p.find_last_of("\\/");
    return i == std::string::npos ? p : p.substr(i + 1);
}
const GuestDllTable* findTable(const std::string& file) {
    for (uint32_t i = 0; i < guest_dll_count; ++i)
        if (lower(guest_dll_tables[i].file) == lower(file)) return &guest_dll_tables[i];
    return nullptr;
}
uint32_t tableBase(const GuestDllTable& t) { return t.table[0].addr & 0xFFFF0000u; }
uint32_t tableEnd(const GuestDllTable& t) { return (t.table[*t.size - 1].addr + 0x100000u) & 0xFFFF0000u; }
}  // namespace

void reserveGuestDllRanges() {
    for (uint32_t i = 0; i < guest_dll_count; ++i) {
        const GuestDllTable& t = guest_dll_tables[i];
        if (!VirtualAlloc(gp(tableBase(t)), tableEnd(t) - tableBase(t), MEM_RESERVE, PAGE_NOACCESS))
            log("warning: cannot reserve 0x%08X for %s", tableBase(t), t.file);
    }
}

// Maps headers and sections at `base` (already allocated) and binds imports.
void mapPeImage(const std::vector<uint8_t>& file, uint32_t base, const char* what) {
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(file.data());
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(file.data() + dos->e_lfanew);
    std::memcpy(gp(base), file.data(), nt->OptionalHeader.SizeOfHeaders);
    const auto* sec = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        const uint32_t va = base + sec[i].VirtualAddress;
        uint32_t raw = sec[i].SizeOfRawData;
        if (sec[i].Misc.VirtualSize && sec[i].Misc.VirtualSize < raw) raw = sec[i].Misc.VirtualSize;
        if (sec[i].PointerToRawData + raw <= file.size()) std::memcpy(gp(va), file.data() + sec[i].PointerToRawData, raw);
    }
    const auto& dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    int bound = 0, missing = 0;
    for (uint32_t d = base + dir.VirtualAddress; dir.VirtualAddress && rd32(d + 12); d += 20) {
        const char* dll = gp<char>(base + rd32(d + 12));
        const uint32_t lookup = base + (rd32(d) ? rd32(d) : rd32(d + 16));
        const uint32_t iat = base + rd32(d + 16);
        for (uint32_t i = 0;; ++i) {
            const uint32_t e = rd32(lookup + 4 * i);
            if (!e) break;
            std::string name = (e & 0x80000000u) ? "#" + std::to_string(e & 0xFFFF) : std::string(gp<char>(base + e + 2));
            const uint32_t known = resolveImport(dll, name.c_str());
            if (known) ++bound;
            else { ++missing; HLOG(1, "%s: import not implemented yet: %s!%s", what, dll, name.c_str()); }
            wr32(iat + 4 * i, known ? known : resolveImportOrStub(dll, name.c_str()));
        }
    }
    log("%s: imports: %d bound to host implementations, %d stubbed", what, bound, missing);
}

uint32_t loadGuestDll(const std::string& path) {
    const std::string file = baseName(path);
    std::lock_guard<std::recursive_mutex> l(g_loadLock);
    if (auto it = g_loaded.find(lower(file)); it != g_loaded.end()) return it->second.base;
    const GuestDllTable* t = findTable(file);
    if (!t) return 0;
    std::vector<uint8_t> data;
    {
        HANDLE h = CreateFileW((g_gameDir + widen(file.c_str())).c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        if (h == INVALID_HANDLE_VALUE) return 0;
        LARGE_INTEGER sz;
        GetFileSizeEx(h, &sz);
        data.resize(static_cast<size_t>(sz.QuadPart));
        DWORD got = 0;
        ReadFile(h, data.data(), static_cast<DWORD>(data.size()), &got, nullptr);
        CloseHandle(h);
    }
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(data.data() + reinterpret_cast<const IMAGE_DOS_HEADER*>(data.data())->e_lfanew);
    const uint32_t base = nt->OptionalHeader.ImageBase, size = nt->OptionalHeader.SizeOfImage;
    if (base != tableBase(*t)) die("%s: image base 0x%08X does not match the recompiled code (0x%08X)", file.c_str(), base, tableBase(*t));
    VirtualFree(gp(tableBase(*t)), 0, MEM_RELEASE);
    if (VirtualAlloc(gp(base), size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) != gp(base))
        die("%s: cannot map at its base address 0x%08X", file.c_str(), base);
    mapPeImage(data, base, file.c_str());
    recomp_register_table(t->table, *t->size);
    HMODULE res = LoadLibraryExW((g_gameDir + widen(file.c_str())).c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
    registerModule(base, res, file.c_str());
    g_loaded[lower(file)] = {file, base, size};
    log("loaded recompiled %s at 0x%08X", file.c_str(), base);
    if (const uint32_t entry = nt->OptionalHeader.AddressOfEntryPoint) {
        const uint32_t ok = guestCall(base + entry, {base, DLL_PROCESS_ATTACH, 0});
        log("%s: DllMain(PROCESS_ATTACH) -> %u", file.c_str(), ok);
    }
    return base;
}

bool isGuestDll(uint32_t handle) {
    std::lock_guard<std::recursive_mutex> l(g_loadLock);
    for (auto& [k, d] : g_loaded)
        if (d.base == handle) return true;
    return false;
}

uint32_t guestDllExport(uint32_t base, const std::string& name) {
    const uint32_t nt = base + rd32(base + 0x3C);
    const uint32_t exp = rd32(nt + 24 + 96);
    if (!exp) return 0;
    const uint32_t e = base + exp;
    const uint32_t ordBase = rd32(e + 16), nFuncs = rd32(e + 20), nNames = rd32(e + 24);
    const uint32_t funcs = base + rd32(e + 28), names = base + rd32(e + 32), ords = base + rd32(e + 36);
    uint32_t idx = 0xFFFFFFFFu;
    if (name[0] == '#') {
        idx = static_cast<uint32_t>(std::stoul(name.substr(1))) - ordBase;
    } else {
        for (uint32_t i = 0; i < nNames; ++i)
            if (name == gp<char>(base + rd32(names + 4 * i))) { idx = rd16(ords + 2 * i); break; }
    }
    if (idx >= nFuncs) return 0;
    const uint32_t rva = rd32(funcs + 4 * idx);
    return rva ? base + rva : 0;
}

}  // namespace host
