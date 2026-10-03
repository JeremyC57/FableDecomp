// Microsoft C runtime extensions (declared in include/w32crt.h). File functions take Windows
// paths and go through the same mapping as CreateFile.
#include "w32.hpp"

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <map>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace w32;

namespace {
template <class C> void splitPath(const C* p, C* drive, C* dir, C* fname, C* ext) {
    auto len = [](const C* s) { size_t n = 0; while (s[n]) ++n; return n; };
    auto put = [](C* dst, const C* s, size_t n) { if (!dst) return; for (size_t i = 0; i < n; ++i) dst[i] = s[i]; dst[n] = 0; };
    size_t n = len(p), i = 0;
    if (n >= 2 && p[1] == ':') { put(drive, p, 2); i = 2; } else put(drive, p, 0);
    size_t lastSep = SIZE_MAX, lastDot = SIZE_MAX;
    for (size_t k = i; k < n; ++k) {
        if (p[k] == '\\' || p[k] == '/') lastSep = k, lastDot = SIZE_MAX;
        else if (p[k] == '.') lastDot = k;
    }
    const size_t nameStart = lastSep == SIZE_MAX ? i : lastSep + 1;
    put(dir, p + i, nameStart - i);
    const size_t nameEnd = lastDot == SIZE_MAX ? n : lastDot;
    put(fname, p + nameStart, nameEnd - nameStart);
    put(ext, p + nameEnd, n - nameEnd);
}
template <class C> void makePath(C* out, const C* drive, const C* dir, const C* fname, const C* ext) {
    size_t o = 0;
    auto app = [&](const C* s) { while (s && *s) out[o++] = *s++; };
    if (drive && *drive) { out[o++] = drive[0]; out[o++] = ':'; }
    if (dir && *dir) {
        app(dir);
        if (out[o - 1] != '\\' && out[o - 1] != '/') out[o++] = '\\';
    }
    app(fname);
    if (ext && *ext) {
        if (*ext != '.') out[o++] = '.';
        app(ext);
    }
    out[o] = 0;
}

struct FindState {
    std::vector<std::string> names;
    std::string dir;
    size_t next = 0;
};
std::mutex g_findLock;
std::map<intptr_t, FindState> g_finds;
intptr_t g_nextFind = 1;

bool wild(const char* p, const char* s) {
    if (!std::strcmp(p, "*.*")) return true;
    while (*p) {
        if (*p == '*') {
            ++p;
            if (!*p) return true;
            for (; *s; ++s)
                if (wild(p, s)) return true;
            return wild(p, s);
        }
        if (!*s) return false;
        if (*p != '?' && std::tolower(static_cast<unsigned char>(*p)) != std::tolower(static_cast<unsigned char>(*s))) return false;
        ++p, ++s;
    }
    return !*s;
}
template <class FD> bool nextFind(FindState& f, FD* fd) {
    while (f.next < f.names.size()) {
        const std::string n = f.names[f.next++];
        struct stat st;
        if (stat((f.dir + "/" + n).c_str(), &st) != 0) continue;
        fd->attrib = S_ISDIR(st.st_mode) ? _A_SUBDIR : _A_ARCH;
        if (!(st.st_mode & S_IWUSR)) fd->attrib |= _A_RDONLY;
        fd->time_create = static_cast<int32_t>(st.st_ctime);
        fd->time_access = static_cast<int32_t>(st.st_atime);
        fd->time_write = static_cast<int32_t>(st.st_mtime);
        fd->size = static_cast<uint32_t>(st.st_size);
        if constexpr (sizeof(fd->name[0]) == 1) {
            const std::string a = utf8ToAcp(n.c_str());
            std::strncpy(fd->name, a.c_str(), 259);
            fd->name[259] = 0;
        } else {
            const wstr w = fromUtf8(n.c_str());
            const size_t k = std::min<size_t>(w.size(), 259);
            for (size_t i = 0; i < k; ++i) fd->name[i] = w[i];
            fd->name[k] = 0;
        }
        return true;
    }
    return false;
}
template <class FD> intptr_t findFirst(const wstr& pattern, FD* fd) {
    const size_t cut = pattern.find_last_of(L"\\/");
    const wstr dirW = cut == wstr::npos ? L"." : pattern.substr(0, cut + 1);
    const std::string spec = toUtf8(pattern.c_str() + (cut == wstr::npos ? 0 : cut + 1));
    FindState f;
    f.dir = toPosixPath(dirW.c_str());
    DIR* d = opendir(f.dir.c_str());
    if (!d) { errno = ENOENT; return -1; }
    while (dirent* e = readdir(d))
        if (wild(spec.c_str(), e->d_name)) f.names.push_back(e->d_name);
    closedir(d);
    if (!nextFind(f, fd)) { errno = ENOENT; return -1; }
    std::lock_guard<std::mutex> l(g_findLock);
    const intptr_t h = g_nextFind++;
    g_finds[h] = std::move(f);
    return h;
}
template <class FD> int findNext(intptr_t h, FD* fd) {
    std::lock_guard<std::mutex> l(g_findLock);
    auto it = g_finds.find(h);
    if (it == g_finds.end() || !nextFind(it->second, fd)) { errno = ENOENT; return -1; }
    return 0;
}
char* itoaBase(unsigned long long v, bool neg, char* buf, int radix) {
    char tmp[72];
    int n = 0;
    do { const int d = static_cast<int>(v % static_cast<unsigned>(radix)); tmp[n++] = static_cast<char>(d < 10 ? '0' + d : 'a' + d - 10); v /= static_cast<unsigned>(radix); } while (v);
    int o = 0;
    if (neg) buf[o++] = '-';
    while (n) buf[o++] = tmp[--n];
    buf[o] = 0;
    return buf;
}
}  // namespace

extern "C" {
char* _strdup(const char* s) { return strdup(s); }
int _stricmp(const char* a, const char* b) { return strcasecmp(a, b); }
int _strnicmp(const char* a, const char* b, size_t n) { return strncasecmp(a, b, n); }
char* _strlwr(char* s) { for (char* p = s; *p; ++p) *p = static_cast<char>(std::tolower(static_cast<unsigned char>(*p))); return s; }
char* _strupr(char* s) { for (char* p = s; *p; ++p) *p = static_cast<char>(std::toupper(static_cast<unsigned char>(*p))); return s; }
char* _strrev(char* s) {
    const size_t n = std::strlen(s);
    for (size_t i = 0; i < n / 2; ++i) std::swap(s[i], s[n - 1 - i]);
    return s;
}
int _wcsicmp(const wchar_t* a, const wchar_t* b) {
    for (;; ++a, ++b) {
        const wchar_t x = lower(*a), y = lower(*b);
        if (x != y) return static_cast<uint16_t>(x) < static_cast<uint16_t>(y) ? -1 : 1;
        if (!x) return 0;
    }
}
int _wcsnicmp(const wchar_t* a, const wchar_t* b, size_t n) {
    for (; n; --n, ++a, ++b) {
        const wchar_t x = lower(*a), y = lower(*b);
        if (x != y) return static_cast<uint16_t>(x) < static_cast<uint16_t>(y) ? -1 : 1;
        if (!x) return 0;
    }
    return 0;
}
wchar_t* _wcsupr(wchar_t* s) { for (wchar_t* p = s; *p; ++p) *p = upper(*p); return s; }
wchar_t* _wcslwr(wchar_t* s) { for (wchar_t* p = s; *p; ++p) *p = lower(*p); return s; }
wchar_t* _wcsdup(const wchar_t* s) {
    const size_t n = wlen(s) + 1;
    auto* d = static_cast<wchar_t*>(std::malloc(n * sizeof(wchar_t)));
    std::memcpy(d, s, n * sizeof(wchar_t));
    return d;
}
long _wtol(const wchar_t* s) { return std::strtol(toUtf8(s).c_str(), nullptr, 10); }
int _wtoi(const wchar_t* s) { return static_cast<int>(_wtol(s)); }
double _wtof(const wchar_t* s) { return std::strtod(toUtf8(s).c_str(), nullptr); }

FILE* _wfopen(const wchar_t* name, const wchar_t* mode) {
    const std::string m = toUtf8(mode);
    const bool create = m.find('w') != std::string::npos || m.find('a') != std::string::npos;
    std::string md;
    for (char ch : m) if (ch != 't' && ch != 'c' && ch != 'n' && ch != 'S' && ch != 'R' && ch != 'T' && ch != 'D') md += ch;
    return std::fopen(toPosixPath(name, create).c_str(), md.c_str());
}
int _wremove(const wchar_t* p) { return std::remove(toPosixPath(p).c_str()); }
int _wrename(const wchar_t* a, const wchar_t* b) { return std::rename(toPosixPath(a).c_str(), toPosixPath(b, true).c_str()); }
int _wmkdir(const wchar_t* p) { return mkdir(toPosixPath(p, true).c_str(), 0755); }
int _wrmdir(const wchar_t* p) { return rmdir(toPosixPath(p).c_str()); }
int _mkdir(const char* p) { return mkdir(toPosixPathA(p, true).c_str(), 0755); }
int _wchdir(const wchar_t* p) { return SetCurrentDirectoryW(p) ? 0 : -1; }
int _chdir(const char* p) { return SetCurrentDirectoryA(p) ? 0 : -1; }
wchar_t* _wgetcwd(wchar_t* buf, int n) {
    if (!buf) { n = MAX_PATH; buf = static_cast<wchar_t*>(std::malloc(MAX_PATH * sizeof(wchar_t))); }
    return GetCurrentDirectoryW(static_cast<DWORD>(n), buf) ? buf : nullptr;
}
char* _getcwd(char* buf, int n) {
    if (!buf) { n = MAX_PATH; buf = static_cast<char*>(std::malloc(MAX_PATH)); }
    return GetCurrentDirectoryA(static_cast<DWORD>(n), buf) ? buf : nullptr;
}
int _getdrive(void) {
    wchar_t cwd[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, cwd);
    return upper(cwd[0]) - L'A' + 1;
}
int _chdrive(int d) {
    wchar_t root[4] = {static_cast<wchar_t>(L'A' + d - 1), L':', L'\\', 0};
    return SetCurrentDirectoryW(root) ? 0 : -1;
}
int _access(const char* p, int mode) { return access(toPosixPathA(p).c_str(), mode & 6); }
int _waccess(const wchar_t* p, int mode) { return access(toPosixPath(p).c_str(), mode & 6); }

intptr_t _wfindfirst32(const wchar_t* pattern, struct _wfinddata32_t* fd) { return findFirst(pattern, fd); }
int _wfindnext32(intptr_t h, struct _wfinddata32_t* fd) { return findNext(h, fd); }
intptr_t _findfirst32(const char* pattern, struct _finddata32_t* fd) { return findFirst(fromUtf8(acpToUtf8(pattern).c_str()), fd); }
int _findnext32(intptr_t h, struct _finddata32_t* fd) { return findNext(h, fd); }
int _findclose(intptr_t h) {
    std::lock_guard<std::mutex> l(g_findLock);
    return g_finds.erase(h) ? 0 : -1;
}

void _wsplitpath(const wchar_t* p, wchar_t* d, wchar_t* dir, wchar_t* f, wchar_t* e) { splitPath(p, d, dir, f, e); }
void _splitpath(const char* p, char* d, char* dir, char* f, char* e) { splitPath(p, d, dir, f, e); }
void _wmakepath(wchar_t* o, const wchar_t* d, const wchar_t* dir, const wchar_t* f, const wchar_t* e) { makePath(o, d, dir, f, e); }
void _makepath(char* o, const char* d, const char* dir, const char* f, const char* e) { makePath(o, d, dir, f, e); }

int _vsnprintf(char* buf, size_t n, const char* fmt, va_list ap) {
    const int r = std::vsnprintf(buf, n, fmt, ap);
    return r >= 0 && static_cast<size_t>(r) >= n ? -1 : r;  // MS: -1 when truncated
}
int _snprintf(char* buf, size_t n, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    const int r = _vsnprintf(buf, n, fmt, ap);
    va_end(ap);
    return r;
}
// Wide printf: the narrow format runs through vsnprintf (%s takes UTF-8 here), then widened.
int _vsnwprintf(wchar_t* buf, size_t n, const wchar_t* fmt, va_list ap) {
    std::string f = toUtf8(fmt);
    char tmp[4096];
    const int r = std::vsnprintf(tmp, sizeof tmp, f.c_str(), ap);
    if (r < 0) return -1;
    const wstr w = fromUtf8(tmp);
    if (w.size() >= n) {
        for (size_t i = 0; i < n; ++i) buf[i] = w[i];
        return -1;
    }
    for (size_t i = 0; i <= w.size(); ++i) buf[i] = i < w.size() ? w[i] : 0;
    return static_cast<int>(w.size());
}
int _snwprintf(wchar_t* buf, size_t n, const wchar_t* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    const int r = _vsnwprintf(buf, n, fmt, ap);
    va_end(ap);
    return r;
}
char* _itoa(int v, char* buf, int radix) { return itoaBase(v < 0 && radix == 10 ? 0ull - static_cast<unsigned long long>(static_cast<long long>(v)) : static_cast<unsigned>(v), v < 0 && radix == 10, buf, radix); }
char* _ltoa(long v, char* buf, int radix) { return _itoa(static_cast<int>(v), buf, radix); }
char* _ultoa(unsigned long v, char* buf, int radix) { return itoaBase(v, false, buf, radix); }
wchar_t* _itow(int v, wchar_t* buf, int radix) {
    char tmp[72];
    _itoa(v, tmp, radix);
    size_t i = 0;
    for (; tmp[i]; ++i) buf[i] = static_cast<unsigned char>(tmp[i]);
    buf[i] = 0;
    return buf;
}
int64_t _atoi64(const char* s) { return std::strtoll(s, nullptr, 10); }

// Single-byte code page (1252): the multibyte string functions are the byte functions.
unsigned char* _mbspbrk(const unsigned char* a, const unsigned char* b) { return (unsigned char*)(std::strpbrk(reinterpret_cast<const char*>(a), reinterpret_cast<const char*>(b))); }
unsigned char* _mbsrchr(const unsigned char* a, unsigned int c) { return (unsigned char*)(std::strrchr(reinterpret_cast<const char*>(a), static_cast<int>(c))); }
unsigned char* _mbschr(const unsigned char* a, unsigned int c) { return (unsigned char*)(std::strchr(reinterpret_cast<const char*>(a), static_cast<int>(c))); }
unsigned char* _mbsstr(const unsigned char* a, const unsigned char* b) { return (unsigned char*)(std::strstr(reinterpret_cast<const char*>(a), reinterpret_cast<const char*>(b))); }
size_t _mbsspn(const unsigned char* a, const unsigned char* b) { return std::strspn(reinterpret_cast<const char*>(a), reinterpret_cast<const char*>(b)); }
size_t _mbscspn(const unsigned char* a, const unsigned char* b) { return std::strcspn(reinterpret_cast<const char*>(a), reinterpret_cast<const char*>(b)); }
int _mbsnbcmp(const unsigned char* a, const unsigned char* b, size_t n) { return std::strncmp(reinterpret_cast<const char*>(a), reinterpret_cast<const char*>(b), n); }
int _mbsicmp(const unsigned char* a, const unsigned char* b) { return strcasecmp(reinterpret_cast<const char*>(a), reinterpret_cast<const char*>(b)); }
unsigned char* _mbsinc(const unsigned char* p) { return const_cast<unsigned char*>(p + 1); }
size_t _mbclen(const unsigned char*) { return 1; }
int _ismbcdigit(unsigned int c) { return c >= '0' && c <= '9'; }
int _ismbcspace(unsigned int c) { return c == ' ' || (c >= 9 && c <= 13); }
int _ismbblead(unsigned int) { return 0; }
}  // extern "C"
