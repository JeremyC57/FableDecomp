// Files, directories and paths.
#include "w32.hpp"

#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <map>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <unistd.h>

namespace w32 {

// ============================================================================
// paths
// ============================================================================
namespace {
std::mutex g_pathLock;
std::string g_cDrive = "/tmp/fable-c";
wstr g_cwd;  // current directory, Windows form with trailing backslash

bool ieq(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
        if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i]))) return false;
    return true;
}

// Resolves one component under dir case-insensitively; returns the on-disk name or "".
std::string matchComponent(const std::string& dir, const std::string& name) {
    struct stat st;
    const std::string direct = dir + (dir.empty() || dir.back() == '/' ? "" : "/") + name;
    if (lstat(direct.c_str(), &st) == 0) return name;
    DIR* d = opendir(dir.empty() ? "." : dir.c_str());
    if (!d) return {};
    std::string found;
    while (dirent* e = readdir(d))
        if (ieq(e->d_name, name)) { found = e->d_name; break; }
    closedir(d);
    return found;
}

wstr currentDir() {
    if (g_cwd.empty()) {
        char buf[4096];
        g_cwd = toWindowsPath(getcwd(buf, sizeof buf) ? buf : "/");
        if (g_cwd.back() != L'\\') g_cwd += L'\\';
    }
    return g_cwd;
}

// Windows path -> absolute Windows path with backslashes, "." and ".." resolved.
wstr fullWindowsPath(const wchar_t* in) {
    wstr p = in ? in : L"";
    for (auto& ch : p) if (ch == L'/') ch = L'\\';
    if (p.rfind(L"\\\\?\\", 0) == 0 || p.rfind(L"\\\\.\\", 0) == 0) p = p.substr(4);
    std::lock_guard<std::mutex> l(g_pathLock);
    const wstr cwd = currentDir();
    if (p.size() >= 2 && p[1] == L':') {
        if (p.size() == 2 || p[2] != L'\\') p = p.substr(0, 2) + L"\\" + p.substr(2);  // "C:foo" -> "C:\foo"
    } else if (!p.empty() && p[0] == L'\\') {
        p = cwd.substr(0, 2) + p;
    } else {
        p = cwd + p;
    }
    // normalise components
    std::vector<wstr> parts;
    wstr cur;
    for (size_t i = 3; i <= p.size(); ++i) {
        if (i == p.size() || p[i] == L'\\') {
            if (cur == L"..") { if (!parts.empty()) parts.pop_back(); }
            else if (!cur.empty() && cur != L".") parts.push_back(cur);
            cur.clear();
        } else {
            cur += p[i];
        }
    }
    wstr out = p.substr(0, 3);
    out[0] = upper(out[0]);
    for (size_t i = 0; i < parts.size(); ++i) out += (i ? L"\\" : L"") + parts[i];
    return out;
}
}  // namespace

void setCDrive(const std::string& dir) {
    std::lock_guard<std::mutex> l(g_pathLock);
    g_cDrive = dir;
}

wstr toWindowsPath(const std::string& posix) {
    wstr w = L"Z:";
    if (posix.empty() || posix[0] != '/') w += L"\\";
    w += fromUtf8(posix.c_str());
    for (auto& ch : w) if (ch == L'/') ch = L'\\';
    return w;
}

std::string toPosixPath(const wchar_t* winPath, bool forCreate) {
    (void)forCreate;
    const wstr full = fullWindowsPath(winPath);
    std::string root;
    if (full[0] == L'Z') root = "/";
    else {
        std::lock_guard<std::mutex> l(g_pathLock);
        root = g_cDrive + "/";
    }
    // components after "X:\"
    std::string rest = toUtf8(full.c_str() + 3);
    std::string out = root;
    size_t i = 0;
    while (i <= rest.size()) {
        size_t j = rest.find('\\', i);
        if (j == std::string::npos) j = rest.size();
        const std::string comp = rest.substr(i, j - i);
        if (!comp.empty()) {
            std::string m = matchComponent(out, comp);
            if (m.empty()) {
                // Not found: keep this and the remaining components as given.
                out += comp;
                for (size_t k = j; k < rest.size(); ++k) out += rest[k] == '\\' ? '/' : rest[k];
                return out;
            }
            out += m;
            if (j < rest.size()) out += '/';
        }
        i = j + 1;
    }
    if (out.size() > 1 && out.back() == '/') out.pop_back();
    return out;
}

std::string toPosixPathA(const char* winPath, bool forCreate) {
    const wstr w = fromUtf8(acpToUtf8(winPath).c_str());
    return toPosixPath(w.c_str(), forCreate);
}

uint64_t unixToFileTime(const timespec& t) {
    return (static_cast<uint64_t>(t.tv_sec) + 11644473600ull) * 10000000ull + static_cast<uint64_t>(t.tv_nsec) / 100;
}
timespec fileTimeToUnix(uint64_t ft) {
    timespec t;
    t.tv_sec = static_cast<time_t>(ft / 10000000ull) - 11644473600ll;
    t.tv_nsec = static_cast<long>((ft % 10000000ull) * 100);
    return t;
}
FILETIME toFT(uint64_t v) { return FILETIME{static_cast<DWORD>(v), static_cast<DWORD>(v >> 32)}; }
uint64_t fromFT(const FILETIME& f) { return f.dwLowDateTime | (static_cast<uint64_t>(f.dwHighDateTime) << 32); }

DWORD attributesOf(const struct stat& st, const std::string& path) {
    DWORD a = S_ISDIR(st.st_mode) ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_ARCHIVE;
    if (!(st.st_mode & S_IWUSR)) a |= FILE_ATTRIBUTE_READONLY;
    const auto slash = path.find_last_of('/');
    if (path.size() > slash + 1 && path[slash + 1] == '.') a |= FILE_ATTRIBUTE_HIDDEN;
    return a;
}

// ============================================================================
// file objects
// ============================================================================
struct File : Object {
    int fd = -1;
    std::string path;
    bool overlapped = false;
    bool deleteOnClose = false;
    bool console = false;
    ~File() override {
        if (fd > 2) close(fd);
        if (deleteOnClose) unlink(path.c_str());
    }
    const char* kind() const override { return "file"; }
    bool waitable() const override { return true; }
    bool signaled(DWORD) const override { return true; }  // I/O always completes synchronously
};

namespace {
std::shared_ptr<File> fileOf(HANDLE h) {
    auto f = objectAs<File>(h);
    if (!f) setError(ERROR_INVALID_HANDLE);
    return f;
}

HANDLE g_std[3];
HANDLE stdHandle(int i) {
    static std::once_flag once;
    std::call_once(once, [] {
        for (int k = 0; k < 3; ++k) {
            auto f = std::make_shared<File>();
            f->fd = k;
            f->console = true;
            g_std[k] = newHandle(f);
        }
    });
    return g_std[i];
}

HANDLE createFile(const std::string& path, DWORD access, DWORD disposition, DWORD flags) {
    int o = 0;
    const bool r = access & (GENERIC_READ | GENERIC_ALL | FILE_READ_DATA | GENERIC_EXECUTE);
    const bool w = access & (GENERIC_WRITE | GENERIC_ALL | FILE_WRITE_DATA | FILE_APPEND_DATA);
    o = r && w ? O_RDWR : w ? O_WRONLY : O_RDONLY;
    switch (disposition) {
        case CREATE_NEW: o |= O_CREAT | O_EXCL; break;
        case CREATE_ALWAYS: o |= O_CREAT | O_TRUNC; break;
        case OPEN_EXISTING: break;
        case OPEN_ALWAYS: o |= O_CREAT; break;
        case TRUNCATE_EXISTING: o |= O_TRUNC; break;
        default: setError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE;
    }
    struct stat st;
    const bool existed = stat(path.c_str(), &st) == 0;
    if (existed && S_ISDIR(st.st_mode)) {
        if (!(flags & FILE_FLAG_BACKUP_SEMANTICS)) { setError(ERROR_ACCESS_DENIED); return INVALID_HANDLE_VALUE; }
        o = O_RDONLY | O_DIRECTORY;
    }
    const int fd = open(path.c_str(), o | O_CLOEXEC, 0644);
    if (fd < 0) {
        setError(errno == ENOENT && disposition != OPEN_EXISTING ? ERROR_PATH_NOT_FOUND : errnoToWin(errno));
        if (errno == EEXIST) setError(ERROR_FILE_EXISTS);
        return INVALID_HANDLE_VALUE;
    }
    auto f = std::make_shared<File>();
    f->fd = fd;
    f->path = path;
    f->overlapped = flags & FILE_FLAG_OVERLAPPED;
    f->deleteOnClose = flags & FILE_FLAG_DELETE_ON_CLOSE;
    setError(existed && (disposition == CREATE_ALWAYS || disposition == OPEN_ALWAYS) ? ERROR_ALREADY_EXISTS : ERROR_SUCCESS);
    return newHandle(f);
}

BOOL doIo(HANDLE h, void* buf, DWORD n, DWORD* done, OVERLAPPED* ov, bool write) {
    if (done) *done = 0;
    auto f = fileOf(h);
    if (!f) return FALSE;
    ssize_t r;
    if (ov) {
        const off_t at = static_cast<off_t>(ov->Offset | (static_cast<uint64_t>(ov->OffsetHigh) << 32));
        r = write ? pwrite(f->fd, buf, n, at) : pread(f->fd, buf, n, at);
    } else {
        r = write ? ::write(f->fd, buf, n) : ::read(f->fd, buf, n);
    }
    if (r < 0) {
        const DWORD e = errnoToWin(errno);
        if (ov) ov->Internal = e;
        setError(e);
        return FALSE;
    }
    if (done) *done = static_cast<DWORD>(r);
    if (ov) {
        ov->Internal = (!write && r == 0 && n) ? ERROR_HANDLE_EOF : 0;
        ov->InternalHigh = static_cast<ULONG_PTR>(r);
        if (ov->hEvent) SetEvent(ov->hEvent);
        if (!write && r == 0 && n) { setError(ERROR_HANDLE_EOF); return FALSE; }
    }
    return TRUE;
}
}  // namespace

// ============================================================================
// find
// ============================================================================
struct Find : Object {
    std::vector<std::string> names;
    std::string dir;
    size_t next = 0;
    const char* kind() const override { return "find"; }
};

namespace {
bool wildMatch(const char* p, const char* s) {
    // Windows-style: '*' any run, '?' one character; case-insensitive; "*.*" matches all.
    if (!std::strcmp(p, "*.*")) return true;
    while (*p) {
        if (*p == '*') {
            ++p;
            if (!*p) return true;
            for (; *s; ++s)
                if (wildMatch(p, s)) return true;
            return wildMatch(p, s);
        }
        if (!*s) return false;
        if (*p != '?' && std::tolower(static_cast<unsigned char>(*p)) != std::tolower(static_cast<unsigned char>(*s))) return false;
        ++p, ++s;
    }
    return !*s;
}

template <class FD> bool fillFind(Find& f, FD* out) {
    while (f.next < f.names.size()) {
        const std::string& n = f.names[f.next++];
        struct stat st;
        if (stat((f.dir + "/" + n).c_str(), &st) != 0) continue;
        std::memset(out, 0, sizeof *out);
        out->dwFileAttributes = attributesOf(st, f.dir + "/" + n);
        out->ftCreationTime = toFT(unixToFileTime(st.st_ctim));
        out->ftLastAccessTime = toFT(unixToFileTime(st.st_atim));
        out->ftLastWriteTime = toFT(unixToFileTime(st.st_mtim));
        out->nFileSizeHigh = static_cast<DWORD>(static_cast<uint64_t>(st.st_size) >> 32);
        out->nFileSizeLow = static_cast<DWORD>(st.st_size);
        if constexpr (sizeof(out->cFileName[0]) == 1) {
            const std::string a = utf8ToAcp(n.c_str());
            std::strncpy(out->cFileName, a.c_str(), MAX_PATH - 1);
        } else {
            const wstr w = fromUtf8(n.c_str());
            for (size_t i = 0; i < w.size() && i < MAX_PATH - 1; ++i) out->cFileName[i] = w[i];
        }
        return true;
    }
    return false;
}

HANDLE findFirst(const wstr& pattern, void* out, bool wide) {
    const size_t cut = pattern.find_last_of(L"\\/");
    const wstr dirW = cut == wstr::npos ? L"." : pattern.substr(0, cut + 1);
    const std::string spec = toUtf8(pattern.c_str() + (cut == wstr::npos ? 0 : cut + 1));
    auto f = std::make_shared<Find>();
    f->dir = toPosixPath(dirW.c_str());
    DIR* d = opendir(f->dir.c_str());
    if (!d) { setError(ERROR_PATH_NOT_FOUND); return INVALID_HANDLE_VALUE; }
    while (dirent* e = readdir(d))
        if (wildMatch(spec.c_str(), e->d_name)) f->names.push_back(e->d_name);
    closedir(d);
    const bool ok = wide ? fillFind(*f, static_cast<WIN32_FIND_DATAW*>(out)) : fillFind(*f, static_cast<WIN32_FIND_DATAA*>(out));
    if (!ok) { setError(ERROR_FILE_NOT_FOUND); return INVALID_HANDLE_VALUE; }
    return newHandle(f);
}
}  // namespace

}  // namespace w32

using namespace w32;

extern "C" {

HANDLE WINAPI CreateFileW(LPCWSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES, DWORD disposition, DWORD flags, HANDLE) {
    (void)share;
    if (!name) { setError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE; }
    const std::string p = toPosixPath(name, disposition != OPEN_EXISTING && disposition != TRUNCATE_EXISTING);
    return createFile(p, access, disposition, flags);
}
HANDLE WINAPI CreateFileA(LPCSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES sa, DWORD disposition, DWORD flags, HANDLE t) {
    const wstr w = fromUtf8(acpToUtf8(name).c_str());
    return CreateFileW(w.c_str(), access, share, sa, disposition, flags, t);
}

BOOL WINAPI ReadFile(HANDLE h, LPVOID buf, DWORD n, LPDWORD done, LPOVERLAPPED ov) { return doIo(h, buf, n, done, ov, false); }
BOOL WINAPI WriteFile(HANDLE h, LPCVOID buf, DWORD n, LPDWORD done, LPOVERLAPPED ov) {
    return doIo(h, const_cast<void*>(buf), n, done, ov, true);
}

// The transfer happens now; the completion routine runs at the caller's next alertable wait.
static BOOL ioEx(HANDLE h, void* buf, DWORD n, LPOVERLAPPED ov, LPOVERLAPPED_COMPLETION_ROUTINE fn, bool write) {
    DWORD done = 0;
    const BOOL ok = doIo(h, buf, n, &done, ov, write);
    if (!ok && GetLastError() != ERROR_HANDLE_EOF) return FALSE;
    const DWORD err = ok ? 0 : ERROR_HANDLE_EOF;
    if (fn) queueApc(currentTid(), [fn, err, done, ov] { fn(err, done, ov); });
    setError(ERROR_SUCCESS);
    return TRUE;
}
BOOL WINAPI ReadFileEx(HANDLE h, LPVOID buf, DWORD n, LPOVERLAPPED ov, LPOVERLAPPED_COMPLETION_ROUTINE fn) { return ioEx(h, buf, n, ov, fn, false); }
BOOL WINAPI WriteFileEx(HANDLE h, LPCVOID buf, DWORD n, LPOVERLAPPED ov, LPOVERLAPPED_COMPLETION_ROUTINE fn) {
    return ioEx(h, const_cast<void*>(buf), n, ov, fn, true);
}
BOOL WINAPI GetOverlappedResult(HANDLE, LPOVERLAPPED ov, LPDWORD done, BOOL) {
    if (done) *done = static_cast<DWORD>(ov->InternalHigh);
    if (ov->Internal) { setError(static_cast<DWORD>(ov->Internal)); return FALSE; }
    return TRUE;
}

DWORD WINAPI SetFilePointer(HANDLE h, LONG dist, PLONG high, DWORD method) {
    auto f = fileOf(h);
    if (!f) return INVALID_SET_FILE_POINTER;
    int64_t off = high ? (static_cast<int64_t>(*high) << 32) | static_cast<uint32_t>(dist) : dist;
    const int whence = method == FILE_BEGIN ? SEEK_SET : method == FILE_CURRENT ? SEEK_CUR : SEEK_END;
    const off_t r = lseek(f->fd, off, whence);
    if (r < 0) { setError(ERROR_NEGATIVE_SEEK); return INVALID_SET_FILE_POINTER; }
    if (high) *high = static_cast<LONG>(static_cast<uint64_t>(r) >> 32);
    setError(ERROR_SUCCESS);
    return static_cast<DWORD>(r);
}
BOOL WINAPI SetFilePointerEx(HANDLE h, LARGE_INTEGER dist, PLARGE_INTEGER out, DWORD method) {
    auto f = fileOf(h);
    if (!f) return FALSE;
    const int whence = method == FILE_BEGIN ? SEEK_SET : method == FILE_CURRENT ? SEEK_CUR : SEEK_END;
    const off_t r = lseek(f->fd, dist.QuadPart, whence);
    if (r < 0) return fail(ERROR_NEGATIVE_SEEK);
    if (out) out->QuadPart = r;
    return TRUE;
}
BOOL WINAPI SetEndOfFile(HANDLE h) {
    auto f = fileOf(h);
    if (!f) return FALSE;
    const off_t at = lseek(f->fd, 0, SEEK_CUR);
    return ftruncate(f->fd, at) == 0 ? TRUE : fail(errnoToWin(errno));
}
DWORD WINAPI GetFileSize(HANDLE h, LPDWORD high) {
    auto f = fileOf(h);
    struct stat st;
    if (!f || fstat(f->fd, &st) != 0) return INVALID_FILE_SIZE;
    if (high) *high = static_cast<DWORD>(static_cast<uint64_t>(st.st_size) >> 32);
    setError(ERROR_SUCCESS);
    return static_cast<DWORD>(st.st_size);
}
BOOL WINAPI GetFileSizeEx(HANDLE h, PLARGE_INTEGER out) {
    auto f = fileOf(h);
    struct stat st;
    if (!f || fstat(f->fd, &st) != 0) return FALSE;
    out->QuadPart = st.st_size;
    return TRUE;
}
DWORD WINAPI GetFileType(HANDLE h) {
    auto f = objectAs<File>(h);
    if (!f) { setError(ERROR_INVALID_HANDLE); return FILE_TYPE_UNKNOWN; }
    return f->console ? FILE_TYPE_CHAR : FILE_TYPE_DISK;
}
BOOL WINAPI GetFileTime(HANDLE h, LPFILETIME c, LPFILETIME a, LPFILETIME w) {
    auto f = fileOf(h);
    struct stat st;
    if (!f || fstat(f->fd, &st) != 0) return FALSE;
    if (c) *c = toFT(unixToFileTime(st.st_ctim));
    if (a) *a = toFT(unixToFileTime(st.st_atim));
    if (w) *w = toFT(unixToFileTime(st.st_mtim));
    return TRUE;
}
BOOL WINAPI SetFileTime(HANDLE h, const FILETIME*, const FILETIME* a, const FILETIME* w) {
    auto f = fileOf(h);
    if (!f) return FALSE;
    timespec ts[2];
    ts[0].tv_nsec = ts[1].tv_nsec = UTIME_OMIT;
    if (a) ts[0] = fileTimeToUnix(fromFT(*a));
    if (w) ts[1] = fileTimeToUnix(fromFT(*w));
    return futimens(f->fd, ts) == 0 ? TRUE : fail(errnoToWin(errno));
}
BOOL WINAPI GetFileInformationByHandle(HANDLE h, LPBY_HANDLE_FILE_INFORMATION out) {
    auto f = fileOf(h);
    struct stat st;
    if (!f || fstat(f->fd, &st) != 0) return FALSE;
    std::memset(out, 0, sizeof *out);
    out->dwFileAttributes = attributesOf(st, f->path);
    out->ftCreationTime = toFT(unixToFileTime(st.st_ctim));
    out->ftLastAccessTime = toFT(unixToFileTime(st.st_atim));
    out->ftLastWriteTime = toFT(unixToFileTime(st.st_mtim));
    out->dwVolumeSerialNumber = static_cast<DWORD>(st.st_dev);
    out->nFileSizeHigh = static_cast<DWORD>(static_cast<uint64_t>(st.st_size) >> 32);
    out->nFileSizeLow = static_cast<DWORD>(st.st_size);
    out->nNumberOfLinks = static_cast<DWORD>(st.st_nlink);
    out->nFileIndexHigh = static_cast<DWORD>(static_cast<uint64_t>(st.st_ino) >> 32);
    out->nFileIndexLow = static_cast<DWORD>(st.st_ino);
    return TRUE;
}
BOOL WINAPI FlushFileBuffers(HANDLE h) {
    auto f = fileOf(h);
    if (!f) return FALSE;
    if (!f->console) fsync(f->fd);
    return TRUE;
}

DWORD WINAPI GetFileAttributesW(LPCWSTR name) {
    struct stat st;
    const std::string p = toPosixPath(name);
    if (stat(p.c_str(), &st) != 0) { setError(errno == ENOTDIR ? ERROR_PATH_NOT_FOUND : ERROR_FILE_NOT_FOUND); return INVALID_FILE_ATTRIBUTES; }
    return attributesOf(st, p);
}
DWORD WINAPI GetFileAttributesA(LPCSTR name) {
    const wstr w = fromUtf8(acpToUtf8(name).c_str());
    return GetFileAttributesW(w.c_str());
}
BOOL WINAPI GetFileAttributesExW(LPCWSTR name, GET_FILEEX_INFO_LEVELS, LPVOID out) {
    struct stat st;
    const std::string p = toPosixPath(name);
    if (stat(p.c_str(), &st) != 0) return fail(errno == ENOTDIR ? ERROR_PATH_NOT_FOUND : ERROR_FILE_NOT_FOUND);
    auto* d = static_cast<WIN32_FILE_ATTRIBUTE_DATA*>(out);
    d->dwFileAttributes = attributesOf(st, p);
    d->ftCreationTime = toFT(unixToFileTime(st.st_ctim));
    d->ftLastAccessTime = toFT(unixToFileTime(st.st_atim));
    d->ftLastWriteTime = toFT(unixToFileTime(st.st_mtim));
    d->nFileSizeHigh = static_cast<DWORD>(static_cast<uint64_t>(st.st_size) >> 32);
    d->nFileSizeLow = static_cast<DWORD>(st.st_size);
    return TRUE;
}
BOOL WINAPI SetFileAttributesW(LPCWSTR name, DWORD attr) {
    const std::string p = toPosixPath(name);
    struct stat st;
    if (stat(p.c_str(), &st) != 0) return fail(ERROR_FILE_NOT_FOUND);
    mode_t m = st.st_mode & 07777;
    m = (attr & FILE_ATTRIBUTE_READONLY) ? (m & ~0222) : (m | S_IWUSR);
    return chmod(p.c_str(), m) == 0 ? TRUE : fail(errnoToWin(errno));
}
BOOL WINAPI DeleteFileW(LPCWSTR name) {
    return unlink(toPosixPath(name).c_str()) == 0 ? TRUE : fail(errnoToWin(errno));
}
BOOL WINAPI DeleteFileA(LPCSTR name) { return unlink(toPosixPathA(name).c_str()) == 0 ? TRUE : fail(errnoToWin(errno)); }
BOOL WINAPI RemoveDirectoryW(LPCWSTR name) {
    return rmdir(toPosixPath(name).c_str()) == 0 ? TRUE : fail(errnoToWin(errno));
}
BOOL WINAPI CreateDirectoryW(LPCWSTR name, LPSECURITY_ATTRIBUTES) {
    return mkdir(toPosixPath(name, true).c_str(), 0755) == 0 ? TRUE : fail(errnoToWin(errno));
}
BOOL WINAPI CreateDirectoryA(LPCSTR name, LPSECURITY_ATTRIBUTES) {
    return mkdir(toPosixPathA(name, true).c_str(), 0755) == 0 ? TRUE : fail(errnoToWin(errno));
}
BOOL WINAPI MoveFileW(LPCWSTR from, LPCWSTR to) {
    return rename(toPosixPath(from).c_str(), toPosixPath(to, true).c_str()) == 0 ? TRUE : fail(errnoToWin(errno));
}

HANDLE WINAPI FindFirstFileA(LPCSTR pattern, LPWIN32_FIND_DATAA out) {
    return findFirst(fromUtf8(acpToUtf8(pattern).c_str()), out, false);
}
HANDLE WINAPI FindFirstFileW(LPCWSTR pattern, LPWIN32_FIND_DATAW out) { return findFirst(pattern, out, true); }
BOOL WINAPI FindNextFileA(HANDLE h, LPWIN32_FIND_DATAA out) {
    auto f = objectAs<Find>(h);
    if (!f) return fail(ERROR_INVALID_HANDLE);
    return fillFind(*f, out) ? TRUE : fail(ERROR_NO_MORE_FILES);
}
BOOL WINAPI FindNextFileW(HANDLE h, LPWIN32_FIND_DATAW out) {
    auto f = objectAs<Find>(h);
    if (!f) return fail(ERROR_INVALID_HANDLE);
    return fillFind(*f, out) ? TRUE : fail(ERROR_NO_MORE_FILES);
}
BOOL WINAPI FindClose(HANDLE h) { return closeHandle(h) ? TRUE : fail(ERROR_INVALID_HANDLE); }

static BOOL diskFree(const std::string& p, PULARGE_INTEGER avail, PULARGE_INTEGER total, PULARGE_INTEGER free) {
    struct statvfs s;
    if (statvfs(p.c_str(), &s) != 0) return fail(errnoToWin(errno));
    if (avail) avail->QuadPart = static_cast<uint64_t>(s.f_bavail) * s.f_frsize;
    if (total) total->QuadPart = static_cast<uint64_t>(s.f_blocks) * s.f_frsize;
    if (free) free->QuadPart = static_cast<uint64_t>(s.f_bfree) * s.f_frsize;
    return TRUE;
}
BOOL WINAPI GetDiskFreeSpaceExW(LPCWSTR dir, PULARGE_INTEGER a, PULARGE_INTEGER t, PULARGE_INTEGER f) {
    // No directory: the (emulated) current directory's disk, not the process's cwd ("/" on Android).
    return diskFree(toPosixPath((dir ? wstr(dir) : fullWindowsPath(L".")).c_str()), a, t, f);
}
BOOL WINAPI GetDiskFreeSpaceExA(LPCSTR dir, PULARGE_INTEGER a, PULARGE_INTEGER t, PULARGE_INTEGER f) {
    return dir ? diskFree(toPosixPathA(dir), a, t, f) : GetDiskFreeSpaceExW(nullptr, a, t, f);
}

DWORD WINAPI GetFullPathNameW(LPCWSTR name, DWORD n, LPWSTR buf, LPWSTR* filePart) {
    const wstr full = fullWindowsPath(name);
    if (full.size() + 1 > n) return static_cast<DWORD>(full.size() + 1);
    for (size_t i = 0; i <= full.size(); ++i) buf[i] = i < full.size() ? full[i] : 0;
    if (filePart) {
        const size_t cut = full.find_last_of(L'\\');
        *filePart = cut == wstr::npos || cut + 1 == full.size() ? nullptr : buf + cut + 1;
    }
    return static_cast<DWORD>(full.size());
}
BOOL WINAPI SetCurrentDirectoryW(LPCWSTR dir) {
    wstr full = fullWindowsPath(dir);
    struct stat st;
    if (stat(toPosixPath(full.c_str()).c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) return fail(ERROR_PATH_NOT_FOUND);
    if (full.back() != L'\\') full += L'\\';
    std::lock_guard<std::mutex> l(g_pathLock);
    g_cwd = full;
    return TRUE;
}
BOOL WINAPI SetCurrentDirectoryA(LPCSTR dir) {
    const wstr w = fromUtf8(acpToUtf8(dir).c_str());
    return SetCurrentDirectoryW(w.c_str());
}
DWORD WINAPI GetCurrentDirectoryW(DWORD n, LPWSTR buf) {
    wstr cwd;
    {
        std::lock_guard<std::mutex> l(g_pathLock);
        cwd = currentDir();
    }
    if (cwd.size() > 3) cwd.pop_back();  // no trailing backslash except at the root
    if (cwd.size() + 1 > n) return static_cast<DWORD>(cwd.size() + 1);
    for (size_t i = 0; i <= cwd.size(); ++i) buf[i] = i < cwd.size() ? cwd[i] : 0;
    return static_cast<DWORD>(cwd.size());
}
DWORD WINAPI GetCurrentDirectoryA(DWORD n, LPSTR buf) {
    wchar_t w[MAX_PATH * 4];
    const DWORD len = GetCurrentDirectoryW(MAX_PATH * 4, w);
    const std::string a = utf8ToAcp(toUtf8(w, len).c_str());
    if (a.size() + 1 > n) return static_cast<DWORD>(a.size() + 1);
    std::memcpy(buf, a.c_str(), a.size() + 1);
    return static_cast<DWORD>(a.size());
}

HANDLE WINAPI GetStdHandle(DWORD which) {
    switch (which) {
        case STD_INPUT_HANDLE: return stdHandle(0);
        case STD_OUTPUT_HANDLE: return stdHandle(1);
        case STD_ERROR_HANDLE: return stdHandle(2);
        default: setError(ERROR_INVALID_PARAMETER); return INVALID_HANDLE_VALUE;
    }
}
BOOL WINAPI SetStdHandle(DWORD, HANDLE) { return TRUE; }

BOOL WINAPI DeviceIoControl(HANDLE, DWORD code, LPVOID, DWORD, LPVOID, DWORD, LPDWORD ret, LPOVERLAPPED) {
    (void)code;
    if (ret) *ret = 0;
    return fail(ERROR_NOT_SUPPORTED);
}

}  // extern "C"
