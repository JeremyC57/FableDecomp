// Registry: an in-memory tree persisted to <data dir>/registry.txt after every change.
// Keys and value names are case-insensitive; string data is stored as UTF-16 (what the W
// functions see) and converted for the A functions.
#include "w32.hpp"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>

namespace w32 {
namespace {

struct Key;
using KeyPtr = std::shared_ptr<Key>;
struct Value { DWORD type = REG_NONE; std::vector<uint8_t> data; std::string name; };
struct Key {
    std::string name;  // as created
    std::map<std::string, KeyPtr> sub;     // lower-case name -> key
    std::map<std::string, Value> values;   // lower-case name -> value
};

std::recursive_mutex g_lock;
KeyPtr g_roots[8];
std::string g_file;
bool g_loaded;

std::string lowerU8(const std::string& s) {
    std::string r = s;
    for (auto& c : r) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return r;
}

int rootIndex(HKEY h) {
    switch (static_cast<uint32_t>(reinterpret_cast<uintptr_t>(h))) {
        case 0x80000000u: return 0;  // HKEY_CLASSES_ROOT
        case 0x80000001u: return 1;  // HKEY_CURRENT_USER
        case 0x80000002u: return 2;  // HKEY_LOCAL_MACHINE
        case 0x80000003u: return 3;  // HKEY_USERS
        case 0x80000005u: return 4;  // HKEY_CURRENT_CONFIG
        default: return -1;
    }
}
const char* kRootNames[] = {"HKEY_CLASSES_ROOT", "HKEY_CURRENT_USER", "HKEY_LOCAL_MACHINE", "HKEY_USERS", "HKEY_CURRENT_CONFIG"};

struct OpenKey : Object {
    KeyPtr key;
    const char* kind() const override { return "key"; }
};

KeyPtr keyOf(HKEY h) {
    const int r = rootIndex(h);
    if (r >= 0) {
        if (!g_roots[r]) g_roots[r] = std::make_shared<Key>(), g_roots[r]->name = kRootNames[r];
        return g_roots[r];
    }
    auto o = objectAs<OpenKey>(reinterpret_cast<HANDLE>(h));
    return o ? o->key : nullptr;
}

// Walks a backslash path below k; creates missing keys if asked.
KeyPtr walk(KeyPtr k, const std::string& path, bool create) {
    size_t i = 0;
    while (k && i < path.size()) {
        size_t j = path.find('\\', i);
        if (j == std::string::npos) j = path.size();
        const std::string comp = path.substr(i, j - i);
        if (!comp.empty()) {
            const std::string lc = lowerU8(comp);
            auto it = k->sub.find(lc);
            if (it == k->sub.end()) {
                if (!create) return nullptr;
                auto n = std::make_shared<Key>();
                n->name = comp;
                it = k->sub.emplace(lc, n).first;
            }
            k = it->second;
        }
        i = j + 1;
    }
    return k;
}

std::string hex(const std::vector<uint8_t>& d) {
    static const char* x = "0123456789abcdef";
    std::string s;
    for (uint8_t b : d) { s += x[b >> 4]; s += x[b & 15]; }
    return s;
}

void saveKey(std::ostream& o, const Key& k, const std::string& path) {
    o << "[" << path << "]\n";
    for (const auto& [lc, v] : k.values) o << "\"" << v.name << "\"=" << v.type << ":" << hex(v.data) << "\n";
    for (const auto& [lc, s] : k.sub) saveKey(o, *s, path + "\\" + s->name);
}

void save() {
    if (g_file.empty()) return;
    std::ofstream o(g_file + ".tmp", std::ios::trunc);
    for (int r = 0; r < 5; ++r)
        if (g_roots[r]) saveKey(o, *g_roots[r], kRootNames[r]);
    o.close();
    std::rename((g_file + ".tmp").c_str(), g_file.c_str());
}

void load() {
    if (g_loaded) return;
    g_loaded = true;
    if (g_file.empty()) {
        const char* home = getenv("FABLE_RECOMP_DATA");
        g_file = std::string(home ? home : ".") + "/registry.txt";
    }
    std::ifstream in(g_file);
    std::string line;
    KeyPtr cur;
    while (std::getline(in, line)) {
        if (line.size() > 2 && line[0] == '[') {
            const std::string path = line.substr(1, line.size() - 2);
            const size_t cut = path.find('\\');
            const std::string root = path.substr(0, cut);
            cur = nullptr;
            for (int r = 0; r < 5; ++r)
                if (root == kRootNames[r]) {
                    cur = keyOf(reinterpret_cast<HKEY>(static_cast<uintptr_t>(r == 4 ? 0x80000005u : 0x80000000u + r)));
                    if (cut != std::string::npos) cur = walk(cur, path.substr(cut + 1), true);
                }
        } else if (cur && line.size() > 3 && line[0] == '"') {
            const size_t q = line.find("\"=", 1);
            if (q == std::string::npos) continue;
            Value v;
            v.name = line.substr(1, q - 1);
            const size_t colon = line.find(':', q);
            v.type = static_cast<DWORD>(std::stoul(line.substr(q + 2, colon - q - 2)));
            const std::string h = line.substr(colon + 1);
            for (size_t i = 0; i + 1 < h.size(); i += 2) v.data.push_back(static_cast<uint8_t>(std::stoul(h.substr(i, 2), nullptr, 16)));
            cur->values[lowerU8(v.name)] = v;
        }
    }
}

bool isString(DWORD t) { return t == REG_SZ || t == REG_EXPAND_SZ || t == REG_MULTI_SZ; }

LSTATUS openKey(HKEY parent, const std::string& sub, bool create, PHKEY out, LPDWORD disp) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    load();
    KeyPtr p = keyOf(parent);
    if (!p) return ERROR_INVALID_HANDLE;
    const bool existed = walk(p, sub, false) != nullptr;
    KeyPtr k = walk(p, sub, create);
    if (!k) return ERROR_FILE_NOT_FOUND;
    if (create && !existed) save();
    if (disp) *disp = existed ? REG_OPENED_EXISTING_KEY : REG_CREATED_NEW_KEY;
    auto o = std::make_shared<OpenKey>();
    o->key = k;
    *out = reinterpret_cast<HKEY>(newHandle(o));
    return ERROR_SUCCESS;
}

LSTATUS queryValue(HKEY h, const std::string& name, LPDWORD type, LPBYTE data, LPDWORD size, bool ansi) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    load();
    KeyPtr k = keyOf(h);
    if (!k) return ERROR_INVALID_HANDLE;
    auto it = k->values.find(lowerU8(name));
    if (it == k->values.end()) return ERROR_FILE_NOT_FOUND;
    const Value& v = it->second;
    std::vector<uint8_t> out = v.data;
    if (ansi && isString(v.type)) {
        const auto* w = reinterpret_cast<const wchar_t*>(v.data.data());
        const std::string a = utf8ToAcp(toUtf8(w, v.data.size() / 2).c_str());
        out.assign(a.begin(), a.end());
        if (out.empty() || out.back() != 0) out.push_back(0);
    }
    if (type) *type = v.type;
    if (!size) return data ? ERROR_INVALID_PARAMETER : ERROR_SUCCESS;
    const DWORD need = static_cast<DWORD>(out.size());
    if (data && *size < need) { *size = need; return ERROR_MORE_DATA; }
    if (data) std::memcpy(data, out.data(), out.size());
    *size = need;
    return ERROR_SUCCESS;
}

LSTATUS setValue(HKEY h, const std::string& name, DWORD type, const BYTE* data, DWORD size, bool ansi) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    load();
    KeyPtr k = keyOf(h);
    if (!k) return ERROR_INVALID_HANDLE;
    Value v;
    v.name = name;
    v.type = type;
    if (ansi && isString(type)) {
        const std::string u = acpToUtf8(reinterpret_cast<const char*>(data), size);
        const wstr w = fromUtf8(u.c_str(), u.size());
        v.data.assign(reinterpret_cast<const uint8_t*>(w.data()), reinterpret_cast<const uint8_t*>(w.data() + w.size()));
        if (w.empty() || w.back() != 0) v.data.push_back(0), v.data.push_back(0);
    } else {
        v.data.assign(data, data + size);
    }
    k->values[lowerU8(name)] = v;
    save();
    return ERROR_SUCCESS;
}

std::string nameW(LPCWSTR s) { return s ? toUtf8(s) : std::string(); }
std::string nameA(LPCSTR s) { return s ? acpToUtf8(s) : std::string(); }

}  // namespace

void setRegistryFile(const std::string& path) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    g_file = path;
    g_loaded = false;
    for (auto& r : g_roots) r = nullptr;
}

}  // namespace w32

using namespace w32;

extern "C" {
LSTATUS WINAPI RegOpenKeyExW(HKEY h, LPCWSTR sub, DWORD, REGSAM, PHKEY out) { return openKey(h, nameW(sub), false, out, nullptr); }
LSTATUS WINAPI RegOpenKeyExA(HKEY h, LPCSTR sub, DWORD, REGSAM, PHKEY out) { return openKey(h, nameA(sub), false, out, nullptr); }
LSTATUS WINAPI RegCreateKeyExW(HKEY h, LPCWSTR sub, DWORD, LPWSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY out, LPDWORD disp) {
    return openKey(h, nameW(sub), true, out, disp);
}
LSTATUS WINAPI RegCreateKeyExA(HKEY h, LPCSTR sub, DWORD, LPSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY out, LPDWORD disp) {
    return openKey(h, nameA(sub), true, out, disp);
}
LSTATUS WINAPI RegCloseKey(HKEY h) {
    if (rootIndex(h) >= 0) return ERROR_SUCCESS;
    return closeHandle(reinterpret_cast<HANDLE>(h)) ? ERROR_SUCCESS : ERROR_INVALID_HANDLE;
}
LSTATUS WINAPI RegQueryValueExW(HKEY h, LPCWSTR name, LPDWORD, LPDWORD type, LPBYTE data, LPDWORD size) {
    return queryValue(h, nameW(name), type, data, size, false);
}
LSTATUS WINAPI RegQueryValueExA(HKEY h, LPCSTR name, LPDWORD, LPDWORD type, LPBYTE data, LPDWORD size) {
    return queryValue(h, nameA(name), type, data, size, true);
}
LSTATUS WINAPI RegSetValueExW(HKEY h, LPCWSTR name, DWORD, DWORD type, const BYTE* data, DWORD size) {
    return setValue(h, nameW(name), type, data, size, false);
}
LSTATUS WINAPI RegSetValueExA(HKEY h, LPCSTR name, DWORD, DWORD type, const BYTE* data, DWORD size) {
    return setValue(h, nameA(name), type, data, size, true);
}
LSTATUS WINAPI RegDeleteValueW(HKEY h, LPCWSTR name) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    load();
    KeyPtr k = keyOf(h);
    if (!k) return ERROR_INVALID_HANDLE;
    if (!k->values.erase(lowerU8(nameW(name)))) return ERROR_FILE_NOT_FOUND;
    save();
    return ERROR_SUCCESS;
}
LSTATUS WINAPI RegDeleteValueA(HKEY h, LPCSTR name) {
    const wstr w = fromUtf8(nameA(name).c_str());
    return RegDeleteValueW(h, w.c_str());
}
LSTATUS WINAPI RegGetValueW(HKEY h, LPCWSTR sub, LPCWSTR name, DWORD flags, LPDWORD type, PVOID data, LPDWORD size) {
    HKEY k = h;
    if (sub && *sub) {
        const LSTATUS r = RegOpenKeyExW(h, sub, 0, KEY_READ, &k);
        if (r != ERROR_SUCCESS) return r;
    }
    DWORD t = 0;
    const LSTATUS r = queryValue(k, nameW(name), &t, static_cast<LPBYTE>(data), size, false);
    if (k != h) RegCloseKey(k);
    if (r != ERROR_SUCCESS) return r;
    if (type) *type = t;
    const DWORD want = flags & 0xFFFF;
    const bool ok = (t == REG_DWORD && (want & RRF_RT_REG_DWORD)) || (t == REG_SZ && (want & RRF_RT_REG_SZ)) ||
                    (t == REG_BINARY && (want & RRF_RT_REG_BINARY)) || (t == REG_QWORD && (want & RRF_RT_REG_QWORD)) ||
                    (t == REG_EXPAND_SZ && (want & RRF_RT_REG_EXPAND_SZ)) || (t == REG_MULTI_SZ && (want & RRF_RT_REG_MULTI_SZ)) || want == RRF_RT_ANY;
    return ok ? ERROR_SUCCESS : ERROR_UNSUPPORTED_TYPE;
}
LSTATUS WINAPI RegSetKeyValueW(HKEY h, LPCWSTR sub, LPCWSTR name, DWORD type, LPCVOID data, DWORD size) {
    HKEY k = h;
    if (sub && *sub) {
        const LSTATUS r = RegCreateKeyExW(h, sub, 0, nullptr, 0, KEY_WRITE, nullptr, &k, nullptr);
        if (r != ERROR_SUCCESS) return r;
    }
    const LSTATUS r = setValue(k, nameW(name), type, static_cast<const BYTE*>(data), size, false);
    if (k != h) RegCloseKey(k);
    return r;
}
}  // extern "C"
