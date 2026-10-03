// OLE32 / SHELL32 / BCRYPT: COM activation of host-implemented classes, task memory,
// command-line splitting, special folders, and SHA-256 hashing.
#include "w32sdl.hpp"

#include <cstring>
#include <map>
#include <propidl.h>
#include <shlobj.h>
#include <shellapi.h>
#include <bcrypt.h>
#include <sys/stat.h>

namespace w32 {
namespace {
struct GuidLess {
    bool operator()(const GUID& a, const GUID& b) const { return std::memcmp(&a, &b, sizeof a) < 0; }
};
std::mutex g_comLock;
// Filled from static initializers in other files, so constructed on first use.
std::map<GUID, std::function<HRESULT(REFIID, void**)>, GuidLess>& classes() {
    static auto* m = new std::map<GUID, std::function<HRESULT(REFIID, void**)>, GuidLess>;
    return *m;
}
#define g_classes classes()
std::string g_dataDir = ".";
}  // namespace

void registerComClass(REFCLSID clsid, std::function<HRESULT(REFIID, void**)> create) {
    std::lock_guard<std::mutex> l(g_comLock);
    g_classes[clsid] = std::move(create);
}
void setDataDir(const std::string& dir) { g_dataDir = dir; }
const std::string& dataDir() { return g_dataDir; }

}  // namespace w32

using namespace w32;

extern "C" {
HRESULT WINAPI CoInitialize(LPVOID) { return S_OK; }
HRESULT WINAPI CoInitializeEx(LPVOID, DWORD) { return S_OK; }
void WINAPI CoUninitialize(void) {}
void WINAPI CoFreeUnusedLibraries(void) {}
HRESULT WINAPI CoCreateInstance(REFCLSID clsid, LPUNKNOWN, DWORD, REFIID iid, LPVOID* out) {
    std::function<HRESULT(REFIID, void**)> f;
    {
        std::lock_guard<std::mutex> l(g_comLock);
        auto it = g_classes.find(clsid);
        if (it != g_classes.end()) f = it->second;
    }
    *out = nullptr;
    return f ? f(iid, out) : REGDB_E_CLASSNOTREG;
}
LPVOID WINAPI CoTaskMemAlloc(SIZE_T n) { return std::malloc(n ? n : 1); }
void WINAPI CoTaskMemFree(LPVOID p) { std::free(p); }
HRESULT WINAPI PropVariantClear(PROPVARIANT* v) {
    std::memset(v, 0, sizeof *v);
    return S_OK;
}

// Splits like the Windows shell: whitespace separates, quotes group, \" escapes.
LPWSTR* WINAPI CommandLineToArgvW(LPCWSTR cmd, int* count) {
    std::vector<wstr> args;
    wstr cur;
    bool inQuote = false, any = false;
    for (const wchar_t* p = cmd; p && *p; ++p) {
        if (*p == L'\\') {
            size_t n = 0;
            while (*p == L'\\') ++n, ++p;
            if (*p == L'"') {
                cur.append(n / 2, L'\\');
                if (n % 2) { cur += L'"'; any = true; continue; }
                --p;
                continue;
            }
            cur.append(n, L'\\');
            any = true;
            --p;
        } else if (*p == L'"') {
            inQuote = !inQuote;
            any = true;
        } else if ((*p == L' ' || *p == L'\t') && !inQuote) {
            if (any) args.push_back(cur), cur.clear(), any = false;
        } else {
            cur += *p;
            any = true;
        }
    }
    if (any) args.push_back(cur);
    // One allocation: pointer array followed by the strings (freed with LocalFree).
    size_t bytes = (args.size() + 1) * sizeof(LPWSTR);
    for (auto& a : args) bytes += (a.size() + 1) * sizeof(wchar_t);
    auto* block = static_cast<uint8_t*>(LocalAlloc(LMEM_FIXED, bytes));
    auto** ptrs = reinterpret_cast<LPWSTR*>(block);
    auto* s = reinterpret_cast<wchar_t*>(block + (args.size() + 1) * sizeof(LPWSTR));
    for (size_t i = 0; i < args.size(); ++i) {
        ptrs[i] = s;
        std::memcpy(s, args[i].c_str(), (args[i].size() + 1) * sizeof(wchar_t));
        s += args[i].size() + 1;
    }
    ptrs[args.size()] = nullptr;
    *count = static_cast<int>(args.size());
    return ptrs;
}

// Special folders live under the data directory (saves go to "My Documents").
HRESULT WINAPI SHGetFolderPathW(HWND, int csidl, HANDLE, DWORD, LPWSTR out) {
    std::string sub;
    switch (csidl & 0xFF) {
        case CSIDL_PERSONAL: sub = "Documents"; break;
        case CSIDL_APPDATA: sub = "AppData/Roaming"; break;
        case CSIDL_LOCAL_APPDATA: sub = "AppData/Local"; break;
        case CSIDL_COMMON_APPDATA: sub = "ProgramData"; break;
        default: sub = "Folders/" + std::to_string(csidl & 0xFF); break;
    }
    std::string path = dataDir();
    for (size_t i = 0; i <= sub.size(); ++i) {
        if (i == sub.size() || sub[i] == '/') mkdir((path + "/" + sub.substr(0, i)).c_str(), 0755);
    }
    mkdir(dataDir().c_str(), 0755);
    path += "/" + sub;
    mkdir(path.c_str(), 0755);
    const wstr w = toWindowsPath(path);
    const size_t n = std::min<size_t>(w.size(), MAX_PATH - 1);
    for (size_t i = 0; i < n; ++i) out[i] = w[i];
    out[n] = 0;
    return S_OK;
}
HINSTANCE WINAPI ShellExecuteA(HWND, LPCSTR op, LPCSTR file, LPCSTR, LPCSTR, INT) {
    std::fprintf(stderr, "ShellExecute(%s, %s) ignored\n", op ? op : "", file ? file : "");
    return reinterpret_cast<HINSTANCE>(static_cast<uintptr_t>(SE_ERR_NOASSOC));
}

// ---- SHA-256 (BCRYPT_SHA256_ALGORITHM only) ---------------------------------------------
struct Sha256 {
    uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    uint8_t buf[64];
    size_t used = 0;
    uint64_t total = 0;
    static uint32_t ror(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
    void block(const uint8_t* p) {
        static const uint32_t k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be,
            0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa,
            0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85,
            0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
            0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f,
            0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) w[i] = (uint32_t(p[4 * i]) << 24) | (uint32_t(p[4 * i + 1]) << 16) | (uint32_t(p[4 * i + 2]) << 8) | p[4 * i + 3];
        for (int i = 16; i < 64; ++i) {
            const uint32_t s0 = ror(w[i - 15], 7) ^ ror(w[i - 15], 18) ^ (w[i - 15] >> 3), s1 = ror(w[i - 2], 17) ^ ror(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], hh = h[7];
        for (int i = 0; i < 64; ++i) {
            const uint32_t t1 = hh + (ror(e, 6) ^ ror(e, 11) ^ ror(e, 25)) + ((e & f) ^ (~e & g)) + k[i] + w[i];
            const uint32_t t2 = (ror(a, 2) ^ ror(a, 13) ^ ror(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
            hh = g, g = f, f = e, e = d + t1, d = c, c = b, b = a, a = t1 + t2;
        }
        h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e, h[5] += f, h[6] += g, h[7] += hh;
    }
    void update(const uint8_t* p, size_t n) {
        total += n;
        while (n) {
            const size_t take = std::min(n, 64 - used);
            std::memcpy(buf + used, p, take);
            used += take, p += take, n -= take;
            if (used == 64) block(buf), used = 0;
        }
    }
    void finish(uint8_t* out) {
        const uint64_t bits = total * 8;
        const uint8_t one = 0x80, zero = 0;
        update(&one, 1);
        while (used != 56) update(&zero, 1);
        uint8_t len[8];
        for (int i = 0; i < 8; ++i) len[i] = static_cast<uint8_t>(bits >> (56 - 8 * i));
        update(len, 8);
        for (int i = 0; i < 8; ++i) out[4 * i] = h[i] >> 24, out[4 * i + 1] = h[i] >> 16, out[4 * i + 2] = h[i] >> 8, out[4 * i + 3] = h[i];
    }
};
NTSTATUS WINAPI BCryptOpenAlgorithmProvider(BCRYPT_ALG_HANDLE* alg, LPCWSTR, LPCWSTR, ULONG) {
    *alg = reinterpret_cast<BCRYPT_ALG_HANDLE>(static_cast<uintptr_t>(0x5A256));
    return 0;
}
NTSTATUS WINAPI BCryptCloseAlgorithmProvider(BCRYPT_ALG_HANDLE, ULONG) { return 0; }
NTSTATUS WINAPI BCryptCreateHash(BCRYPT_ALG_HANDLE, BCRYPT_HASH_HANDLE* h, PUCHAR, ULONG, PUCHAR, ULONG, ULONG) {
    *h = new Sha256;
    return 0;
}
NTSTATUS WINAPI BCryptHashData(BCRYPT_HASH_HANDLE h, PUCHAR p, ULONG n, ULONG) {
    static_cast<Sha256*>(h)->update(p, n);
    return 0;
}
NTSTATUS WINAPI BCryptFinishHash(BCRYPT_HASH_HANDLE h, PUCHAR out, ULONG, ULONG) {
    static_cast<Sha256*>(h)->finish(out);
    return 0;
}
NTSTATUS WINAPI BCryptDestroyHash(BCRYPT_HASH_HANDLE h) {
    delete static_cast<Sha256*>(h);
    return 0;
}
}  // extern "C"
