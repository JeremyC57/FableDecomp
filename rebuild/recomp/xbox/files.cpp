// I/O manager: Nt* file calls over host files. Object names are ANSI on the Xbox.
//   \Device\CdRom0                 -> the game folder (extracted disc), read-only
//   \Device\Harddisk0\PartitionN   -> <hdd>/PartitionN (1 = E: user data, 3-5 = cache)
//   \??\X:  (or \DosDevices\X:)    -> symbolic links created by XAPI / the kernel
// FATX names are case-insensitive: each path component is matched without case.
#include "settings.hpp"
#include "xhost.hpp"

#include <algorithm>
#include <dirent.h>
#include <fcntl.h>
#include <map>
#include <sys/stat.h>
#include <unistd.h>

namespace xb {

uint32_t ntToDos(uint32_t status);

struct HostFile {
    int fd = -1;
    bool dir = false, readOnly = false, deleteOnClose = false;
    int volume = -1;  // raw partition handle (\Device\Harddisk0\PartitionN): its number
    std::string host, object;
    uint64_t pos = 0;
    std::vector<std::string> list;  // directory enumeration
    size_t listPos = 0;
    bool listed = false;
    int refs = 1;
};

namespace {
std::string g_game, g_hdd;
std::map<std::string, std::string> g_links;  // lower-case link name -> target
std::mutex g_fsLock;

std::string lower(std::string s) {
    for (auto& ch : s) ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    return s;
}
bool startsWithI(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && strncasecmp(s.data(), p.data(), p.size()) == 0;
}

// Applies symbolic links (longest match on a component boundary), repeatedly.
std::string applyLinks(std::string p) {
    for (int depth = 0; depth < 8; ++depth) {
        if (startsWithI(p, "\\DosDevices\\")) p = "\\??\\" + p.substr(12);
        bool changed = false;
        std::lock_guard<std::mutex> l(g_fsLock);
        for (auto it = g_links.rbegin(); it != g_links.rend(); ++it) {
            const std::string& k = it->first;
            if (p.size() >= k.size() && lower(p.substr(0, k.size())) == k && (p.size() == k.size() || p[k.size()] == '\\')) {
                p = it->second + p.substr(k.size());
                changed = true;
                break;
            }
        }
        if (!changed) break;
    }
    return p;
}

// The disc's xuser.ini sets the draw distances (SetMaxAnimatedMeshDist(64),
// SetMaxStaticMeshDist(128), MaxThingDrawDist 128: meshes pop in and out at those ranges). With
// draw_distance > 1 the game reads a copy (in the cache folder) with the numbers multiplied.
std::string patchedUserIni(const std::string& orig) {
    const float k = settings().drawDistance;
    const char* cache = getenv("FABLE_CACHE_DIR");
    if (k <= 1.0f || !cache) return orig;
    FILE* in = fopen(orig.c_str(), "rb");
    if (!in) return orig;
    std::string text;
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, in)) > 0) text.append(buf, n);
    fclose(in);
    std::string out;
    size_t pos = 0;
    while (pos < text.size()) {
        size_t eol = text.find('\n', pos);
        if (eol == std::string::npos) eol = text.size(); else ++eol;
        std::string line = text.substr(pos, eol - pos);
        pos = eol;
        if (line.find("MaxAnimatedMeshDist") != std::string::npos || line.find("MaxStaticMeshDist") != std::string::npos ||
            line.find("MaxThingDrawDist") != std::string::npos) {
            const size_t d = line.find_first_of("0123456789");
            if (d != std::string::npos) {
                size_t e = d;
                while (e < line.size() && (isdigit(static_cast<unsigned char>(line[e])) || line[e] == '.')) ++e;
                const int v = static_cast<int>(atof(line.substr(d, e - d).c_str()) * k + 0.5f);
                line = line.substr(0, d) + std::to_string(v) + line.substr(e);
            }
        }
        out += line;
    }
    const std::string path = std::string(cache) + "/xuser.ini";
    if (FILE* f = fopen(path.c_str(), "wb")) {
        fwrite(out.data(), 1, out.size(), f);
        fclose(f);
        static bool logged = false;
        if (!logged) { logged = true; XLOG(1, "files: xuser.ini draw distances x%g", k); }
        return path;
    }
    return orig;
}

// Object path -> host path (or empty if no device matches). `ro` set for the disc.
std::string toHost(const std::string& object, bool& ro, int* volume = nullptr) {
    std::string p = applyLinks(object);
    if (volume) *volume = -1;
    std::string rest, root;
    ro = false;
    if (startsWithI(p, "\\Device\\CdRom0")) {
        root = g_game;
        rest = p.substr(14);
        ro = true;
    } else if (startsWithI(p, "\\Device\\Harddisk0\\Partition")) {
        size_t i = 27;
        std::string num;
        while (i < p.size() && isdigit(static_cast<unsigned char>(p[i]))) num += p[i++];
        if (num.empty()) return {};
        rest = p.substr(i);
        if (num == "0") {
            // The raw disk: XAPI keeps its cache-partition table in sector 4. A small image file.
            const std::string img = g_hdd + "/Partition0.img";
            struct stat st;
            if (stat(img.c_str(), &st) != 0) {
                const int fd = open(img.c_str(), O_CREAT | O_RDWR, 0644);
                if (fd >= 0) {
                    if (ftruncate(fd, 0x80000) != 0) {}
                    close(fd);
                }
            }
            return rest.empty() || rest == "\\" ? img : std::string();
        }
        root = g_hdd + "/Partition" + num;
        mkdir(root.c_str(), 0755);
        if (volume && rest.empty()) *volume = atoi(num.c_str());
    } else {
        return {};
    }
    // Resolve each component without case.
    std::string cur = root;
    size_t s = 0;
    while (s < rest.size()) {
        while (s < rest.size() && rest[s] == '\\') ++s;
        size_t e = rest.find('\\', s);
        if (e == std::string::npos) e = rest.size();
        if (e == s) break;
        std::string comp = rest.substr(s, e - s);
        s = e;
        if (comp == ".") continue;
        if (comp == "..") {
            const size_t slash = cur.rfind('/');
            if (slash != std::string::npos && cur.size() > root.size()) cur.resize(slash);
            continue;
        }
        std::string cand = cur + "/" + comp;
        struct stat st;
        if (stat(cand.c_str(), &st) != 0) {
            if (DIR* d = opendir(cur.c_str())) {
                while (dirent* de = readdir(d))
                    if (strcasecmp(de->d_name, comp.c_str()) == 0) {
                        cand = cur + "/" + de->d_name;
                        break;
                    }
                closedir(d);
            }
        }
        cur = cand;
    }
    if (ro) {
        const size_t slash = cur.rfind('/');
        if (strcasecmp(cur.c_str() + (slash == std::string::npos ? 0 : slash + 1), "xuser.ini") == 0) return patchedUserIni(cur);
    }
    return cur;
}

void fileTimes(const struct stat& st, uint32_t out) {  // Creation, LastAccess, LastWrite, Change
    auto ft = [](time_t t) { return static_cast<uint64_t>(t) * 10000000ull + 116444736000000000ull; };
    wr64(out + 0, ft(st.st_mtime));
    wr64(out + 8, ft(st.st_atime));
    wr64(out + 16, ft(st.st_mtime));
    wr64(out + 24, ft(st.st_mtime));
}
uint32_t attrsOf(const struct stat& st, bool ro) {
    if (S_ISDIR(st.st_mode)) return 0x10;
    return ro ? 0x01 : 0x80;
}
uint64_t allocSize(uint64_t n) { return (n + 0x3FFF) & ~0x3FFFull; }

bool wildMatch(const char* pat, const char* s) {
    if (!*pat) return !*s;
    if (*pat == '*') {
        for (const char* t = s;; ++t) {
            if (wildMatch(pat + 1, t)) return true;
            if (!*t) return false;
        }
    }
    if (!*s) return false;
    if (*pat == '?' || tolower(static_cast<unsigned char>(*pat)) == tolower(static_cast<unsigned char>(*s))) return wildMatch(pat + 1, s + 1);
    return false;
}

void ioStatus(uint32_t iosb, uint32_t status, uint32_t info) {
    if (!iosb) return;
    wr32(iosb, status);
    wr32(iosb + 4, info);
}

// Completion for calls that take Event/ApcRoutine/ApcContext (all I/O here is synchronous).
void complete(uint32_t event, uint32_t apc, uint32_t apcCtx, uint32_t iosb) {
    if (event) {
        Handle* h = handleGet(event);
        if (h && h->kind == Handle::Dispatcher) keSetEvent(h->object);
    }
    if (apc) curThread()->userApcs.push_back({apc, apcCtx, iosb, 0});
}

HostFile* fileOf(uint32_t h) {
    Handle* x = handleGet(h);
    return x && x->kind == Handle::File ? x->file : nullptr;
}

std::mutex g_wmaLock;
std::map<const void*, std::string> g_lastWma;  // guest thread -> host path of the last .wma it opened
}  // namespace

std::string lastWmaOpened() {
    std::lock_guard<std::mutex> l(g_wmaLock);
    const auto it = g_lastWma.find(curThread());
    return it == g_lastWma.end() ? std::string() : it->second;
}

void hostFileClose(HostFile* f) {
    if (--f->refs > 0) return;
    if (f->fd >= 0) close(f->fd);
    if (f->deleteOnClose) (f->dir ? rmdir : unlink)(f->host.c_str());
    delete f;
}
void hostFileAddRef(HostFile* f) { ++f->refs; }

void filesInit(const std::string& game, const std::string& hdd) {
    g_game = game;
    g_hdd = hdd;
    mkdir(hdd.c_str(), 0755);
    for (int i = 1; i <= 5; ++i) mkdir((hdd + "/Partition" + std::to_string(i)).c_str(), 0755);
}

void symlinkCreate(const std::string& link, const std::string& target) {
    std::lock_guard<std::mutex> l(g_fsLock);
    g_links[lower(link)] = target;
    XLOG(1, "symlink %s -> %s", link.c_str(), target.c_str());
}
bool symlinkDelete(const std::string& link) {
    std::lock_guard<std::mutex> l(g_fsLock);
    return g_links.erase(lower(link)) != 0;
}
bool symlinkQuery(const std::string& link, std::string& target) {
    std::lock_guard<std::mutex> l(g_fsLock);
    auto it = g_links.find(lower(link));
    if (it == g_links.end()) return false;
    target = it->second;
    return true;
}

std::string resolveObjectName(uint32_t oa) {
    if (!oa) return {};
    std::string name = readAnsiString(rd32(oa + 4));
    const uint32_t root = rd32(oa);
    if (root == 0xFFFFFFFDu) return "\\??\\" + name;                 // ObDosDevicesDirectory()
    if (root == 0xFFFFFFFCu) return "\\Win32NamedObjects\\" + name;  // ObWin32NamedObjectsDirectory()
    if (root) {
        HostFile* f = fileOf(root);
        Handle* h = handleGet(root);
        std::string base = f ? f->object : (h ? h->link : std::string());
        if (!base.empty() && base.back() != '\\' && !name.empty() && name[0] != '\\') base += '\\';
        name = base + name;
    }
    return name;
}

// ============================================================================================
static uint32_t openCommon(Ctx* c, uint32_t pHandle, uint32_t access, uint32_t oa, uint32_t iosb, uint32_t disposition,
                           uint32_t options, uint32_t allocSizePtr) {
    (void)c;
    (void)allocSizePtr;
    const std::string object = resolveObjectName(oa);
    bool ro = false;
    int volume = -1;
    const std::string host = toHost(object, ro, &volume);
    if (host.empty()) {
        XLOG(1, "open %s: no such device", object.c_str());
        ioStatus(iosb, ST_OBJECT_PATH_NOT_FOUND, 0);
        return ST_OBJECT_PATH_NOT_FOUND;
    }
    struct stat st;
    const bool exists = stat(host.c_str(), &st) == 0;
    const bool isDir = exists && S_ISDIR(st.st_mode);
    const bool wantDir = options & 0x1, wantFile = options & 0x40;
    const bool write = access & (0x40000000u | 0x2 | 0x4 | 0x10000 | 0x100);  // GENERIC_WRITE, WRITE_DATA, APPEND, DELETE, WRITE_ATTRIBUTES
    uint32_t info = 1;  // FILE_OPENED
    if (exists) {
        if (disposition == 2) {  // FILE_CREATE
            ioStatus(iosb, ST_OBJECT_NAME_COLLISION, 0);
            return ST_OBJECT_NAME_COLLISION;
        }
        if (wantDir && !isDir) { ioStatus(iosb, ST_NOT_A_DIRECTORY, 0); return ST_NOT_A_DIRECTORY; }
        if (wantFile && isDir) { ioStatus(iosb, ST_FILE_IS_A_DIRECTORY, 0); return ST_FILE_IS_A_DIRECTORY; }
    } else {
        if (disposition == 1 || disposition == 4) {  // FILE_OPEN, FILE_OVERWRITE
            // Distinguish a missing file from a missing directory on the way.
            const std::string parent = host.substr(0, host.rfind('/'));
            struct stat ps;
            const uint32_t st2 = stat(parent.c_str(), &ps) == 0 ? ST_OBJECT_NAME_NOT_FOUND : ST_OBJECT_PATH_NOT_FOUND;
            XLOG(2, "open %s: not found", object.c_str());
            ioStatus(iosb, st2, 0);
            return st2;
        }
        if (ro) { ioStatus(iosb, ST_ACCESS_DENIED, 0); return ST_ACCESS_DENIED; }
    }
    auto* f = new HostFile;
    f->host = host;
    f->object = applyLinks(object);
    f->readOnly = ro;
    f->volume = volume;
    if ((exists && isDir) || (!exists && wantDir)) {
        if (!exists && mkdir(host.c_str(), 0755) != 0) {
            delete f;
            ioStatus(iosb, ST_OBJECT_PATH_NOT_FOUND, 0);
            return ST_OBJECT_PATH_NOT_FOUND;
        }
        f->dir = true;
        info = exists ? 1 : 2;
    } else {
        int flags = (write && !ro) ? O_RDWR : O_RDONLY;
        if (!exists) { flags |= O_CREAT; info = 2; }
        else if (!ro && (disposition == 0 || disposition == 4 || disposition == 5)) { flags |= O_TRUNC; info = disposition == 0 ? 0 : 3; }
        if (flags & (O_CREAT | O_TRUNC)) flags = (flags & ~O_RDONLY) | O_RDWR;
        f->fd = open(host.c_str(), flags, 0644);
        if (f->fd < 0) {
            delete f;
            const uint32_t s = errno == ENOENT ? ST_OBJECT_PATH_NOT_FOUND : ST_ACCESS_DENIED;
            ioStatus(iosb, s, 0);
            return s;
        }
    }
    if (options & 0x1000) f->deleteOnClose = true;  // FILE_DELETE_ON_CLOSE
    if (host.size() > 4 && strncasecmp(host.c_str() + host.size() - 4, ".wma", 4) == 0) {
        std::lock_guard<std::mutex> l(g_wmaLock);
        g_lastWma[curThread()] = host;
    }
    Handle h;
    h.kind = Handle::File;
    h.file = f;
    const uint32_t hv = handleNew(h);
    wr32(pHandle, hv);
    XLOG(2, "open %s -> %s handle 0x%X", object.c_str(), host.c_str(), hv);
    ioStatus(iosb, ST_SUCCESS, info);
    return ST_SUCCESS;
}

KFUNC(NtCreateFile, 9) {
    return openCommon(c, ARG(c, 0), ARG(c, 1), ARG(c, 2), ARG(c, 3), ARG(c, 7), ARG(c, 8), ARG(c, 4));
}
KFUNC(NtOpenFile, 6) { return openCommon(c, ARG(c, 0), ARG(c, 1), ARG(c, 2), ARG(c, 3), 1, ARG(c, 5), 0); }

KFUNC(NtClose, 1) {
    const uint32_t h = ARG(c, 0);
    return handleClose(h) ? ST_SUCCESS : ST_INVALID_HANDLE;
}

KFUNC(NtReadFile, 8) {
    const uint32_t h = ARG(c, 0), event = ARG(c, 1), apc = ARG(c, 2), apcCtx = ARG(c, 3), iosb = ARG(c, 4);
    const uint32_t buf = ARG(c, 5), len = ARG(c, 6), pOff = ARG(c, 7);
    HostFile* f = fileOf(h);
    if (f && f->volume >= 0) {  // raw partition: reads as blank sectors
        std::memset(gp(buf), 0, len);
        ioStatus(iosb, ST_SUCCESS, len);
        complete(event, apc, apcCtx, iosb);
        return ST_SUCCESS;
    }
    if (!f || f->fd < 0) return ST_INVALID_HANDLE;
    uint64_t off = f->pos;
    if (pOff) {
        const uint64_t v = rd64(pOff);
        if (v != 0xFFFFFFFFFFFFFFFEull) off = v;
    }
    ssize_t n;
    {
        GilRelease r;
        n = pread(f->fd, gp(buf), len, static_cast<off_t>(off));
    }
    if (n < 0) n = 0;
    f->pos = off + static_cast<uint64_t>(n);
    const uint32_t st = (n == 0 && len > 0) ? ST_END_OF_FILE : ST_SUCCESS;
    ioStatus(iosb, st, static_cast<uint32_t>(n));
    XLOG(3, "read %s @%llu len %u -> %zd", f->object.c_str(), static_cast<unsigned long long>(off), len, n);
    complete(event, apc, apcCtx, iosb);
    return st;
}

KFUNC(NtWriteFile, 8) {
    const uint32_t h = ARG(c, 0), event = ARG(c, 1), apc = ARG(c, 2), apcCtx = ARG(c, 3), iosb = ARG(c, 4);
    const uint32_t buf = ARG(c, 5), len = ARG(c, 6), pOff = ARG(c, 7);
    HostFile* f = fileOf(h);
    if (f && f->volume >= 0 && !f->readOnly) {
        // Raw writes to a partition are XAPI formatting it (the cache drive): start empty.
        if (pOff && rd64(pOff) == 0) {
            XLOG(1, "partition %d formatted: clearing %s", f->volume, f->host.c_str());
            const std::string cmd = "rm -rf '" + f->host + "'/* 2>/dev/null";
            if (system(cmd.c_str()) != 0) {}
        }
        ioStatus(iosb, ST_SUCCESS, len);
        complete(event, apc, apcCtx, iosb);
        return ST_SUCCESS;
    }
    if (!f || f->fd < 0) return ST_INVALID_HANDLE;
    if (f->readOnly) return ST_ACCESS_DENIED;
    uint64_t off = f->pos;
    if (pOff) {
        const uint64_t v = rd64(pOff);
        if (v == 0xFFFFFFFFFFFFFFFFull) off = static_cast<uint64_t>(lseek(f->fd, 0, SEEK_END));
        else if (v != 0xFFFFFFFFFFFFFFFEull) off = v;
    }
    ssize_t n;
    {
        GilRelease r;
        n = pwrite(f->fd, gp(buf), len, static_cast<off_t>(off));
    }
    if (n < 0) {
        ioStatus(iosb, ST_ACCESS_DENIED, 0);
        return ST_ACCESS_DENIED;
    }
    f->pos = off + static_cast<uint64_t>(n);
    ioStatus(iosb, ST_SUCCESS, static_cast<uint32_t>(n));
    complete(event, apc, apcCtx, iosb);
    return ST_SUCCESS;
}

KFUNC(NtFlushBuffersFile, 2) {
    ioStatus(ARG(c, 1), ST_SUCCESS, 0);
    return ST_SUCCESS;
}

KFUNC(NtQueryInformationFile, 5) {
    const uint32_t h = ARG(c, 0), iosb = ARG(c, 1), out = ARG(c, 2), len = ARG(c, 3), cls = ARG(c, 4);
    HostFile* f = fileOf(h);
    if (!f) return ST_INVALID_HANDLE;
    struct stat st;
    if ((f->fd >= 0 ? fstat(f->fd, &st) : stat(f->host.c_str(), &st)) != 0) return ST_UNSUCCESSFUL;
    uint32_t used = 0;
    switch (cls) {
    case 4:  // FileBasicInformation
        if (len < 0x28) return ST_BUFFER_TOO_SMALL;
        fileTimes(st, out);
        wr32(out + 32, attrsOf(st, f->readOnly));
        wr32(out + 36, 0);
        used = 0x28;
        break;
    case 5:  // FileStandardInformation
        if (len < 0x18) return ST_BUFFER_TOO_SMALL;
        wr64(out, allocSize(static_cast<uint64_t>(st.st_size)));
        wr64(out + 8, static_cast<uint64_t>(st.st_size));
        wr32(out + 16, 1);
        wr8(out + 20, 0);
        wr8(out + 21, S_ISDIR(st.st_mode) ? 1 : 0);
        wr16(out + 22, 0);
        used = 0x18;
        break;
    case 14:  // FilePositionInformation
        wr64(out, f->pos);
        used = 8;
        break;
    case 34:  // FileNetworkOpenInformation
        if (len < 0x38) return ST_BUFFER_TOO_SMALL;
        fileTimes(st, out);
        wr64(out + 32, allocSize(static_cast<uint64_t>(st.st_size)));
        wr64(out + 40, static_cast<uint64_t>(st.st_size));
        wr32(out + 48, attrsOf(st, f->readOnly));
        wr32(out + 52, 0);
        used = 0x38;
        break;
    case 6:  // FileInternalInformation
        wr64(out, static_cast<uint64_t>(st.st_ino));
        used = 8;
        break;
    case 16:  // FileModeInformation
        wr32(out, 0);
        used = 4;
        break;
    default:
        XLOG(0, "NtQueryInformationFile: class %u not implemented", cls);
        return ST_INVALID_PARAMETER;
    }
    ioStatus(iosb, ST_SUCCESS, used);
    return ST_SUCCESS;
}

KFUNC(NtSetInformationFile, 5) {
    const uint32_t h = ARG(c, 0), iosb = ARG(c, 1), in = ARG(c, 2), cls = ARG(c, 4);
    HostFile* f = fileOf(h);
    if (!f) return ST_INVALID_HANDLE;
    switch (cls) {
    case 14: f->pos = rd64(in); break;  // FilePositionInformation
    case 20:                             // FileEndOfFileInformation
        if (f->readOnly || f->fd < 0 || ftruncate(f->fd, static_cast<off_t>(rd64(in))) != 0) return ST_ACCESS_DENIED;
        break;
    case 19: break;                      // FileAllocationInformation
    case 4: break;                       // FileBasicInformation (times, attributes)
    case 13: f->deleteOnClose = rd8(in) != 0; break;  // FileDispositionInformation
    case 10: {                           // FileRenameInformation: ReplaceIfExists, RootDirectory, OBJECT_STRING
        const uint32_t oaFake = poolAllocZero(12);
        wr32(oaFake, rd32(in + 4));
        wr32(oaFake + 4, in + 8);
        const std::string object = resolveObjectName(oaFake);
        poolFree(oaFake);
        bool ro;
        const std::string target = toHost(object, ro);
        if (target.empty() || ro || f->readOnly) return ST_ACCESS_DENIED;
        if (rename(f->host.c_str(), target.c_str()) != 0) return ST_ACCESS_DENIED;
        f->host = target;
        f->object = object;
        break;
    }
    default:
        XLOG(0, "NtSetInformationFile: class %u not implemented", cls);
        return ST_INVALID_PARAMETER;
    }
    ioStatus(iosb, ST_SUCCESS, 0);
    return ST_SUCCESS;
}

KFUNC(NtQueryVolumeInformationFile, 5) {
    const uint32_t h = ARG(c, 0), iosb = ARG(c, 1), out = ARG(c, 2), len = ARG(c, 3), cls = ARG(c, 4);
    HostFile* f = fileOf(h);
    if (!f) return ST_INVALID_HANDLE;
    uint32_t used = 0;
    const uint64_t clusters = 0x40000, freeClusters = 0x30000;  // 4 GB volume with 3 GB free, 16 KB clusters
    switch (cls) {
    case 3:  // FileFsSizeInformation
        wr64(out, clusters);
        wr64(out + 8, freeClusters);
        wr32(out + 16, 32);
        wr32(out + 20, 512);
        used = 24;
        break;
    case 7:  // FileFsFullSizeInformation
        wr64(out, clusters);
        wr64(out + 8, freeClusters);
        wr64(out + 16, freeClusters);
        wr32(out + 24, 32);
        wr32(out + 28, 512);
        used = 32;
        break;
    case 1:  // FileFsVolumeInformation
        if (len < 0x18) return ST_BUFFER_TOO_SMALL;
        std::memset(gp(out), 0, 0x18);
        wr32(out + 8, 0x12345678);
        used = 0x18;
        break;
    case 4:  // FileFsDeviceInformation
        wr32(out, f->readOnly ? 2 : 7);  // FILE_DEVICE_CD_ROM / FILE_DEVICE_DISK
        wr32(out + 4, 0);
        used = 8;
        break;
    case 5: {  // FileFsAttributeInformation
        const char* fs = f->readOnly ? "CDFS" : "FATX";
        wr32(out, 0);
        wr32(out + 4, 42);
        wr32(out + 8, 4);
        std::memcpy(gp(out + 12), fs, 4);
        used = 16;
        break;
    }
    default:
        XLOG(0, "NtQueryVolumeInformationFile: class %u not implemented", cls);
        return ST_INVALID_PARAMETER;
    }
    ioStatus(iosb, ST_SUCCESS, used);
    return ST_SUCCESS;
}

KFUNC(NtQueryDirectoryFile, 10) {
    const uint32_t h = ARG(c, 0), event = ARG(c, 1), apc = ARG(c, 2), apcCtx = ARG(c, 3), iosb = ARG(c, 4);
    const uint32_t out = ARG(c, 5), len = ARG(c, 6), cls = ARG(c, 7), mask = ARG(c, 8), restart = ARG(c, 9) & 0xFF;
    HostFile* f = fileOf(h);
    if (!f || !f->dir) return ST_INVALID_HANDLE;
    if (cls != 1) {
        XLOG(0, "NtQueryDirectoryFile: class %u not implemented", cls);
        return ST_INVALID_PARAMETER;
    }
    const bool first = !f->listed;
    if (!f->listed || restart || mask) {
        std::string pat = mask ? readAnsiString(mask) : std::string("*");
        if (pat.empty()) pat = "*";
        if (!f->listed || restart || mask) {
            f->list.clear();
            if (DIR* d = opendir(f->host.c_str())) {
                while (dirent* de = readdir(d)) {
                    if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;
                    if (wildMatch(pat.c_str(), de->d_name)) f->list.push_back(de->d_name);
                }
                closedir(d);
            }
            std::sort(f->list.begin(), f->list.end());
            f->listPos = 0;
            f->listed = true;
        }
    }
    if (f->listPos >= f->list.size()) {
        const uint32_t st = first && f->list.empty() ? ST_NO_SUCH_FILE : ST_NO_MORE_FILES;
        ioStatus(iosb, st, 0);
        return st;
    }
    const std::string& name = f->list[f->listPos];
    if (len < 0x40 + name.size()) return ST_BUFFER_OVERFLOW;
    ++f->listPos;
    struct stat st;
    stat((f->host + "/" + name).c_str(), &st);
    std::memset(gp(out), 0, 0x40);
    fileTimes(st, out + 8);
    wr64(out + 0x28, S_ISDIR(st.st_mode) ? 0 : static_cast<uint64_t>(st.st_size));
    wr64(out + 0x30, S_ISDIR(st.st_mode) ? 0 : allocSize(static_cast<uint64_t>(st.st_size)));
    wr32(out + 0x38, attrsOf(st, f->readOnly));
    wr32(out + 0x3C, static_cast<uint32_t>(name.size()));
    std::memcpy(gp(out + 0x40), name.data(), name.size());
    ioStatus(iosb, ST_SUCCESS, 0x40 + static_cast<uint32_t>(name.size()));
    complete(event, apc, apcCtx, iosb);
    return ST_SUCCESS;
}

KFUNC(NtQueryFullAttributesFile, 2) {
    const std::string object = resolveObjectName(ARG(c, 0));
    bool ro;
    const std::string host = toHost(object, ro);
    struct stat st;
    if (host.empty() || stat(host.c_str(), &st) != 0) {
        XLOG(2, "attributes %s: not found", object.c_str());
        return ST_OBJECT_NAME_NOT_FOUND;
    }
    const uint32_t out = ARG(c, 1);
    fileTimes(st, out);
    wr64(out + 32, allocSize(static_cast<uint64_t>(st.st_size)));
    wr64(out + 40, static_cast<uint64_t>(st.st_size));
    wr32(out + 48, attrsOf(st, ro));
    wr32(out + 52, 0);
    return ST_SUCCESS;
}

KFUNC(NtDeleteFile, 1) {
    const std::string object = resolveObjectName(ARG(c, 0));
    bool ro;
    const std::string host = toHost(object, ro);
    if (host.empty() || ro) return ST_ACCESS_DENIED;
    struct stat st;
    if (stat(host.c_str(), &st) != 0) return ST_OBJECT_NAME_NOT_FOUND;
    if ((S_ISDIR(st.st_mode) ? rmdir(host.c_str()) : unlink(host.c_str())) != 0)
        return S_ISDIR(st.st_mode) ? ST_DIRECTORY_NOT_EMPTY : ST_ACCESS_DENIED;
    return ST_SUCCESS;
}

static uint32_t ioctl(Ctx* c, const char* what) {
    const uint32_t iosb = ARG(c, 4), code = ARG(c, 5), out = ARG(c, 8), outLen = ARG(c, 9);
    HostFile* f = fileOf(ARG(c, 0));
    // Partitions: 750 MB cache partitions (3..5), larger data partitions.
    const uint64_t partSize = f && f->volume >= 3 ? 0x2EE00000ull : 0x1312D6000ull;
    if (code == 0x70000 && outLen >= 0x18) {  // IOCTL_DISK_GET_DRIVE_GEOMETRY
        wr64(out, partSize / 512);  // Cylinders
        wr32(out + 8, 12);          // FixedMedia
        wr32(out + 12, 1);          // TracksPerCylinder
        wr32(out + 16, 1);          // SectorsPerTrack
        wr32(out + 20, 512);        // BytesPerSector
        ioStatus(iosb, ST_SUCCESS, 0x18);
        return ST_SUCCESS;
    }
    if (code == 0x74004 && outLen >= 0x20) {  // IOCTL_DISK_GET_PARTITION_INFO
        std::memset(gp(out), 0, 0x20);
        wr64(out, 0x80000ull * static_cast<uint64_t>(f ? std::max(f->volume, 1) : 1));  // StartingOffset
        wr64(out + 8, partSize);                                                       // PartitionLength
        wr32(out + 20, f ? static_cast<uint32_t>(f->volume) : 0);                      // PartitionNumber
        wr8(out + 24, 0x0E);
        wr8(out + 26, 1);                                                               // RecognizedPartition
        ioStatus(iosb, ST_SUCCESS, 0x20);
        return ST_SUCCESS;
    }
    XLOG(1, "%s(handle 0x%X, code 0x%X): not implemented, reporting success", what, ARG(c, 0), code);
    if (out && outLen) std::memset(gp(out), 0, outLen);
    ioStatus(iosb, ST_SUCCESS, 0);
    return ST_SUCCESS;
}
KFUNC(NtFsControlFile, 10) { return ioctl(c, "NtFsControlFile"); }
KFUNC(NtDeviceIoControlFile, 10) { return ioctl(c, "NtDeviceIoControlFile"); }

KFUNC(IoCreateSymbolicLink, 2) {
    symlinkCreate(readAnsiString(ARG(c, 0)), readAnsiString(ARG(c, 1)));
    return ST_SUCCESS;
}
KFUNC(IoDeleteSymbolicLink, 1) {
    return symlinkDelete(readAnsiString(ARG(c, 0))) ? ST_SUCCESS : ST_OBJECT_NAME_NOT_FOUND;
}
KFUNC(NtOpenSymbolicLinkObject, 2) {
    const std::string name = resolveObjectName(ARG(c, 1));
    std::string target;
    if (!symlinkQuery(name, target)) return ST_OBJECT_NAME_NOT_FOUND;
    Handle h;
    h.kind = Handle::SymLink;
    h.link = target;
    wr32(ARG(c, 0), handleNew(h));
    return ST_SUCCESS;
}
KFUNC(NtQuerySymbolicLinkObject, 3) {
    Handle* h = handleGet(ARG(c, 0));
    if (!h || h->kind != Handle::SymLink) return ST_INVALID_HANDLE;
    const uint32_t s = ARG(c, 1);
    const uint32_t n = static_cast<uint32_t>(h->link.size());
    if (ARG(c, 2)) wr32(ARG(c, 2), n);
    if (rd16(s + 2) < n) return ST_BUFFER_TOO_SMALL;
    std::memcpy(gp(rd32(s + 4)), h->link.data(), n);
    wr16(s, static_cast<uint16_t>(n));
    return ST_SUCCESS;
}

// A guest FILE_OBJECT for a file handle (ObReferenceObjectByHandle). XAPI's physical sort
// key reads FileObject->FsContext (+8)[0], the start sector on a GDFX disc: a stable value
// per file keeps the game's load ordering meaningful.
uint32_t fileObjectFor(Handle* h) {
    if (h->object) return h->object;
    const uint32_t fo = poolAllocZero(0x40), fcb = poolAllocZero(0x20);
    uint32_t key = 2166136261u;
    for (char ch : h->file ? h->file->host : std::string()) key = (key ^ static_cast<uint8_t>(ch)) * 16777619u;
    wr32(fcb, key & 0x7FFFFFFF);
    wr32(fo + 8, fcb);
    return h->object = fo;
}

// IoQueryVolumeInformation(FileObject, FsInformationClass, Length, FsInformation, ReturnedLength)
KFUNC(IoQueryVolumeInformation, 5) {
    const uint32_t cls = ARG(c, 1), len = ARG(c, 2), out = ARG(c, 3);
    if (cls != 5 || len < 16) return ST_NOT_IMPLEMENTED;  // FileFsAttributeInformation only
    wr32(out, 0);        // FileSystemAttributes
    wr32(out + 4, 255);  // MaximumComponentNameLength
    wr32(out + 8, 4);    // FileSystemNameLength
    std::memcpy(gp(out + 12), "GDFX", 4);
    if (ARG(c, 4)) wr32(ARG(c, 4), 16);
    return ST_SUCCESS;
}

// XAPI passes this as the ApcRoutine of ReadFileEx/WriteFileEx: ApcContext is the caller's
// completion routine, the IO_STATUS_BLOCK is the start of its OVERLAPPED.
KFUNC(NtUserIoApcDispatcher, 3) {
    const uint32_t routine = ARG(c, 0), iosb = ARG(c, 1);
    const uint32_t status = rd32(iosb);
    const uint32_t err = (status & 0x80000000u) ? ntToDos(status) : 0;
    guestCall(routine, {err, rd32(iosb + 4), iosb});
    return 0;
}

// Device-driver plumbing (memory units, USB): reported as unavailable.
KFUNC(IoInvalidDeviceRequest, 2) { return ST_INVALID_DEVICE_REQUEST; }
KFUNC(IoSynchronousDeviceIoControlRequest, 8) { return ST_INVALID_DEVICE_REQUEST; }
KFUNC(IoStartPacket, 3) { return 0; }
KFUNC(IoStartNextPacket, 1) { return 0; }
KFUNC(IoMarkIrpMustComplete, 1) { return 0; }

} // namespace xb
