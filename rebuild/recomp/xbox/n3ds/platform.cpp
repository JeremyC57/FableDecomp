// New 3DS host: entry point, screens, input, the xiso on the SD card as the game disc, crash
// reports. Boot test build: no rendering yet (the NV2A's commands are consumed, not drawn); the
// top screen shows status, the bottom screen the log, and sdmc:/3ds/fable/xbox_recomp.log has it all.
#include "../disc.hpp"
#include "../gpu.hpp"
#include "../input.hpp"
#include "../settings.hpp"
#include "../xhost.hpp"

#include <3ds.h>

#include <dirent.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/iosupport.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cctype>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// libctru heaps: the guest window (recomp.h RECOMP_WINDOW_*) takes the whole heap area, so the
// host's own malloc heap lives in linear FCRAM (__system_allocateHeaps below).
extern "C" {
u32 __ctru_heap_size = 0;
u32 __ctru_linear_heap_size = 0;
u32 __stacksize__ = 256 * 1024;  // the main thread runs the boot sequence
}

namespace xb {
void bootGame(std::vector<uint8_t> xbe, const std::string& game, const std::string& hdd);  // boot.cpp
const char* g_dataDir = "sdmc:/3ds/fable";

namespace gpu {
Renderer& softRenderer();
Renderer* g_renderer;
Renderer& renderer() { return g_renderer ? *g_renderer : softRenderer(); }
std::atomic<uint32_t> g_vblanks{0};
extern uint64_t g_frameCount;
}  // namespace gpu

namespace {
PrintConsole g_top, g_bottom;
std::atomic<bool> g_fatal{false};
char g_fatalMsg[1024];
std::mutex g_screenLock;
}  // namespace

// ---- crash reports ---------------------------------------------------------------------------
[[noreturn]] void platformFatal(const char* msg) {
    if (!g_fatal.exchange(true)) {
        std::snprintf(g_fatalMsg, sizeof g_fatalMsg, "%s", msg);
        std::lock_guard<std::mutex> l(g_screenLock);
        consoleSelect(&g_top);
        std::printf("\x1b[2J\x1b[1;1H\x1b[31mFATAL\x1b[0m\n%s\n\nLog: %s/xbox_recomp.log\nPress START to exit.\n", g_fatalMsg, g_dataDir);
    }
    // The main thread shows the message and exits on START; other threads stop here.
    for (;;) svcSleepThread(1000000000LL);
}

namespace {
// One exception stack per thread that can fault (guest threads, the pusher, the main thread).
constexpr size_t kExcStack = 0x2000;
struct ExcState {
    alignas(8) uint8_t stack[kExcStack];
    ERRF_ExceptionData data;
};
thread_local ExcState* t_exc;

[[noreturn]] void onException(ERRF_ExceptionInfo* info, CpuRegisters* regs) {
    char m[512];
    const uint32_t far = info->far, pc = regs->pc;
    const char* kind = info->type == ERRF_EXCEPTION_DATA_ABORT ? "data abort" : info->type == ERRF_EXCEPTION_PREFETCH_ABORT ? "prefetch abort"
                     : info->type == ERRF_EXCEPTION_UNDEFINED ? "undefined instruction" : "VFP exception";
    int n = std::snprintf(m, sizeof m, "%s at pc %08lX lr %08lX\nfault address %08lX", kind, static_cast<unsigned long>(pc),
                          static_cast<unsigned long>(regs->lr), static_cast<unsigned long>(far));
    if (far >= RECOMP_WINDOW_BASE && far < RECOMP_WINDOW_BASE + RECOMP_WINDOW_MASK + 1)
        n += std::snprintf(m + n, sizeof m - n, " = guest %08lX", static_cast<unsigned long>(far - RECOMP_WINDOW_BASE));
    n += std::snprintf(m + n, sizeof m - n, "\nsp %08lX fsr %08lX\nr0 %08lX r1 %08lX r2 %08lX r3 %08lX\nr4 %08lX r5 %08lX r6 %08lX r7 %08lX",
                       static_cast<unsigned long>(regs->sp), static_cast<unsigned long>(info->fsr), static_cast<unsigned long>(regs->r[0]),
                       static_cast<unsigned long>(regs->r[1]), static_cast<unsigned long>(regs->r[2]), static_cast<unsigned long>(regs->r[3]),
                       static_cast<unsigned long>(regs->r[4]), static_cast<unsigned long>(regs->r[5]), static_cast<unsigned long>(regs->r[6]),
                       static_cast<unsigned long>(regs->r[7]));
    if (const XThread* t = curThread())
        std::snprintf(m + n, sizeof m - n, "\nguest thread %u: esp %08X eax %08X ecx %08X", t->id, t->ctx.esp, t->ctx.eax, t->ctx.ecx);
    logf(0, "FATAL: %s", m);
    platformFatal(m);
}
}  // namespace

void platformThreadInit() {
    if (!t_exc) t_exc = new ExcState;
    threadOnException(onException, t_exc->stack + kExcStack, &t_exc->data);
}
void platformInstallFaultHandler() { platformThreadInit(); }

// ---- host threads ----------------------------------------------------------------------------
namespace {
int g_gameCore = -2;  // core 2 on the New 3DS (set in main), else the process default
s32 g_mainPrio = 0x30;
}

void startHostThread(std::function<void()> fn, size_t stackBytes, ThreadRole role) {
    auto* f = new std::function<void()>(std::move(fn));
    const s32 prio = role == ThreadRole::Tick ? g_mainPrio - 1 : role == ThreadRole::System ? g_mainPrio + 1 : g_mainPrio;
    const int core = role == ThreadRole::Game ? g_gameCore : -2;
    auto entry = [](void* p) {
        platformThreadInit();
        auto* fn = static_cast<std::function<void()>*>(p);
        (*fn)();
        delete fn;
    };
    Thread t = threadCreate(entry, f, stackBytes, prio, core, true);
    if (!t && core != -2) t = threadCreate(entry, f, stackBytes, prio, -2, true);  // core 2 refused
    if (!t) die("cannot create a host thread (%zu KB stack)", stackBytes / 1024);
}

// ---- small host services the desktop files provide elsewhere ---------------------------------
void setThreadName(const char*) {}
void profilerStart() {}
bool profilerCapture(double, double, const std::string&) { return false; }
[[noreturn]] void hostRelaunch() {
    // Loading a save from the pause menu reboots the title with launch data (kernel.cpp).
    die("loading a save from the pause menu restarts the game, which this build cannot do yet: load it from the main menu");
}

// ---- input: the 3DS buttons as an Xbox pad on port 1 ----------------------------------------
namespace input {
namespace {
std::mutex g_padLock;
Pad g_pad;
}
void init() {}
void update() {
    hidScanInput();
    const u32 k = hidKeysHeld();
    circlePosition cp, cs;
    hidCircleRead(&cp);
    irrstCstickRead(&cs);
    Pad p;
    p.buttons = (k & KEY_DUP ? 0x01 : 0) | (k & KEY_DDOWN ? 0x02 : 0) | (k & KEY_DLEFT ? 0x04 : 0) | (k & KEY_DRIGHT ? 0x08 : 0) |
                (k & KEY_START ? 0x10 : 0) | (k & KEY_SELECT ? 0x20 : 0);
    p.analog[0] = k & KEY_A ? 255 : 0;
    p.analog[1] = k & KEY_B ? 255 : 0;
    p.analog[2] = k & KEY_X ? 255 : 0;
    p.analog[3] = k & KEY_Y ? 255 : 0;
    p.analog[4] = k & KEY_ZR ? 255 : 0;  // black
    p.analog[5] = k & KEY_ZL ? 255 : 0;  // white
    p.analog[6] = k & KEY_L ? 255 : 0;   // left trigger
    p.analog[7] = k & KEY_R ? 255 : 0;   // right trigger
    auto axis = [](int v) { return static_cast<int16_t>(std::max(-32767, std::min(32767, v * 32767 / 150))); };
    p.lx = axis(cp.dx);
    p.ly = axis(cp.dy);
    p.rx = axis(cs.dx);
    p.ry = axis(cs.dy);
    std::lock_guard<std::mutex> l(g_padLock);
    p.packet = g_pad.packet + (std::memcmp(&p.buttons, &g_pad.buttons, sizeof(Pad) - sizeof(uint32_t)) != 0);
    g_pad = p;
}
uint32_t connectedMask() { return 1; }
Pad pad(int port) {
    std::lock_guard<std::mutex> l(g_padLock);
    return port == 0 ? g_pad : Pad{};
}
void rumble(int, uint16_t, uint16_t) {}
void setRecordFile(const char*) {}
void setVirtualPad(bool, uint32_t, float, float, float, float, float, float) {}
void touchLook(int, int) {}
}  // namespace input

// ---- the xiso as a read-only device, "iso:/..." ----------------------------------------------
// files.cpp opens game files with POSIX calls under the game folder; newlib routes "iso:" paths
// here, and each read is one seek + read of the image on the SD card.
namespace {
struct IsoFile {
    disc::Entry e;
    uint64_t pos;
};
struct IsoDir {
    std::vector<disc::Entry> entries;
    size_t next;
};

std::string isoPath(const char* path) {
    const char* p = std::strchr(path, ':');
    std::string s = p ? p + 1 : path;
    for (char& ch : s)
        if (ch == '/') ch = '\\';
    while (!s.empty() && s.back() == '\\') s.pop_back();
    return s;
}

void fillStat(const disc::Entry& e, struct stat* st) {
    std::memset(st, 0, sizeof *st);
    st->st_mode = e.dir ? (S_IFDIR | 0555) : (S_IFREG | 0444);
    st->st_size = static_cast<off_t>(e.size);
    st->st_nlink = 1;
    st->st_blksize = 2048;
    st->st_mtime = st->st_atime = st->st_ctime = disc::timestamp();
}

int isoOpen(struct _reent* r, void* fileStruct, const char* path, int flags, int) {
    if ((flags & O_ACCMODE) != O_RDONLY) { r->_errno = EROFS; return -1; }
    auto* f = static_cast<IsoFile*>(fileStruct);
    new (f) IsoFile{};
    if (!disc::find(isoPath(path), f->e) || f->e.dir) { r->_errno = ENOENT; return -1; }
    return 0;
}
int isoClose(struct _reent*, void* fd) { static_cast<IsoFile*>(fd)->~IsoFile(); return 0; }
ssize_t isoRead(struct _reent* r, void* fd, char* ptr, size_t len) {
    auto* f = static_cast<IsoFile*>(fd);
    if (f->pos >= f->e.size) return 0;
    len = static_cast<size_t>(std::min<uint64_t>(len, f->e.size - f->pos));
    const int64_t n = disc::read(f->e, ptr, len, f->pos);
    if (n < 0) { r->_errno = EIO; return -1; }
    f->pos += static_cast<uint64_t>(n);
    return static_cast<ssize_t>(n);
}
off_t isoSeek(struct _reent* r, void* fd, off_t pos, int dir) {
    auto* f = static_cast<IsoFile*>(fd);
    const int64_t base = dir == SEEK_SET ? 0 : dir == SEEK_CUR ? static_cast<int64_t>(f->pos) : static_cast<int64_t>(f->e.size);
    if (base + pos < 0) { r->_errno = EINVAL; return -1; }
    f->pos = static_cast<uint64_t>(base + pos);
    return static_cast<off_t>(f->pos);
}
int isoFstat(struct _reent*, void* fd, struct stat* st) { fillStat(static_cast<IsoFile*>(fd)->e, st); return 0; }
int isoStat(struct _reent* r, const char* path, struct stat* st) {
    disc::Entry e;
    if (!disc::find(isoPath(path), e)) { r->_errno = ENOENT; return -1; }
    fillStat(e, st);
    return 0;
}
DIR_ITER* isoDirOpen(struct _reent* r, DIR_ITER* it, const char* path) {
    disc::Entry e;
    if (!disc::find(isoPath(path), e) || !e.dir) { r->_errno = ENOENT; return nullptr; }
    auto* d = static_cast<IsoDir*>(it->dirStruct);
    new (d) IsoDir{disc::list(e), 0};
    return it;
}
int isoDirReset(struct _reent*, DIR_ITER* it) { static_cast<IsoDir*>(it->dirStruct)->next = 0; return 0; }
int isoDirNext(struct _reent* r, DIR_ITER* it, char* name, struct stat* st) {
    auto* d = static_cast<IsoDir*>(it->dirStruct);
    if (d->next >= d->entries.size()) { r->_errno = ENOENT; return -1; }
    const disc::Entry& e = d->entries[d->next++];
    std::snprintf(name, NAME_MAX, "%s", e.name.c_str());
    if (st) fillStat(e, st);
    return 0;
}
int isoDirClose(struct _reent*, DIR_ITER* it) { static_cast<IsoDir*>(it->dirStruct)->~IsoDir(); return 0; }

const devoptab_t g_isoDevice = [] {
    devoptab_t d{};
    d.name = "iso";
    d.structSize = sizeof(IsoFile);
    d.open_r = isoOpen;
    d.close_r = isoClose;
    d.read_r = isoRead;
    d.seek_r = isoSeek;
    d.fstat_r = isoFstat;
    d.stat_r = isoStat;
    d.dirStateSize = sizeof(IsoDir);
    d.diropen_r = isoDirOpen;
    d.dirreset_r = isoDirReset;
    d.dirnext_r = isoDirNext;
    d.dirclose_r = isoDirClose;
    return d;
}();

// The Fable xiso in the SD card root: the first .iso / .xiso whose name contains "fable",
// else the first that is an Xbox disc image.
std::string findDisc() {
    std::vector<std::string> cands;
    if (DIR* d = opendir("sdmc:/")) {
        while (dirent* e = readdir(d)) {
            const char* dot = std::strrchr(e->d_name, '.');
            if (dot && (!strcasecmp(dot, ".iso") || !strcasecmp(dot, ".xiso"))) cands.push_back(e->d_name);
        }
        closedir(d);
    }
    auto hasFable = [](std::string n) {
        for (char& ch : n) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        return n.find("fable") != std::string::npos;
    };
    std::stable_partition(cands.begin(), cands.end(), hasFable);
    for (const std::string& n : cands)
        if (disc::open("sdmc:/" + n)) return "sdmc:/" + n;
    return {};
}

void mkdirs(const std::string& path) {
    for (size_t i = 1; i <= path.size(); ++i)
        if (i == path.size() || path[i] == '/') mkdir(path.substr(0, i).c_str(), 0755);
}

void drawStatus(const std::string& disc, uint64_t startMs) {
    if (g_fatal) return;
    float fps = 0, worst = 0;
    perfStats(&fps, &worst);
    std::lock_guard<std::mutex> l(g_screenLock);
    consoleSelect(&g_top);
    std::printf("\x1b[1;1HFable (Xbox) boot test\x1b[K\n%s\x1b[K\n\n", disc.c_str());
    std::printf("frames %llu  %.1f fps (%.0f ms/frame, slowest %.0f)\x1b[K\n", static_cast<unsigned long long>(gpu::g_frameCount), fps,
                fps > 0 ? 1000.0f / fps : 0.0f, worst);
    std::printf("guest memory %lu KB (peak %lu KB)\x1b[K\n", static_cast<unsigned long>(memBackedBytes() / 1024),
                static_cast<unsigned long>(memBackedPeakBytes() / 1024));
    std::printf("window to guest 0x%08lX\x1b[K\n", static_cast<unsigned long>(memWindowEnd()));
    std::printf("free linear %lu KB\x1b[K\n", static_cast<unsigned long>(linearSpaceFree() / 1024));
    std::printf("up %llu s\x1b[K\n\nSTART+SELECT: quit\x1b[K\n", static_cast<unsigned long long>((osGetTime() - startMs) / 1000));
    consoleSelect(&g_bottom);
}
}  // namespace

}  // namespace xb

// Heaps. The heap area belongs to the guest window, so the host's malloc heap and libctru's
// linear heap (framebuffers, GPU buffers) are two blocks of linear FCRAM.
extern "C" void __system_allocateHeaps(void) {
    extern char* fake_heap_start;
    extern char* fake_heap_end;
    extern u32 __ctru_linear_heap;
    constexpr u32 kLinear = 4u << 20, kHostHeap = 24u << 20;
    u32 out = 0;
    svcControlMemory(&out, 0, 0, kLinear, MEMOP_ALLOC_LINEAR, static_cast<MemPerm>(MEMPERM_READ | MEMPERM_WRITE));
    __ctru_linear_heap = out;
    __ctru_linear_heap_size = kLinear;
    out = 0;
    svcControlMemory(&out, 0, 0, kHostHeap, MEMOP_ALLOC_LINEAR, static_cast<MemPerm>(MEMPERM_READ | MEMPERM_WRITE));
    fake_heap_start = reinterpret_cast<char*>(out);
    fake_heap_end = reinterpret_cast<char*>(out + kHostHeap);
}

// newlib has no pread/pwrite here: files.cpp's are a locked seek + read/write.
static std::mutex g_preadLock;
extern "C" ssize_t pread(int fd, void* buf, size_t n, off_t off) {
    std::lock_guard<std::mutex> l(g_preadLock);
    if (lseek(fd, off, SEEK_SET) < 0) return -1;
    return read(fd, buf, n);
}
extern "C" ssize_t pwrite(int fd, const void* buf, size_t n, off_t off) {
    std::lock_guard<std::mutex> l(g_preadLock);
    if (lseek(fd, off, SEEK_SET) < 0) return -1;
    return write(fd, buf, n);
}

int main() {
    using namespace xb;
    osSetSpeedupEnable(true);  // New 3DS: 804 MHz and the L2 cache
    gfxInitDefault();
    consoleInit(GFX_TOP, &g_top);
    consoleInit(GFX_BOTTOM, &g_bottom);
    consoleSelect(&g_bottom);
    irrstInit();
    platformThreadInit();
    const uint64_t start = osGetTime();
    bool n3ds = false;
    APT_CheckNew3DS(&n3ds);
    svcGetThreadPriority(&g_mainPrio, CUR_THREAD_HANDLE);
    if (n3ds) g_gameCore = 2;
    std::printf("Fable boot test build 2 (%s, app memory %lu KB)\n", n3ds ? "New 3DS" : "old 3DS: too slow",
                static_cast<unsigned long>(osGetMemRegionSize(MEMREGION_APPLICATION) / 1024));

    mkdirs(g_dataDir);
    const std::string hdd = std::string(g_dataDir) + "/hdd";
    mkdirs(hdd);
    loadSettings(std::string(g_dataDir) + "/fable_xbox.ini");
    if (const char* e = getenv("XBOX_LOG")) g_logLevel = atoi(e);

    const std::string disc = findDisc();
    if (disc.empty()) platformFatal("No Fable xiso in the SD card root.\nCopy the disc image (.iso or .xiso) to sdmc:/ and start again.");
    AddDevice(&g_isoDevice);
    XLOG(0, "disc: %s", disc.c_str());
    std::vector<uint8_t> xbe;
    if (!disc::readFile("\\default.xbe", xbe)) platformFatal("The disc image has no default.xbe.");

    bootGame(std::move(xbe), "iso:", hdd);

    uint64_t lastStatus = 0;
    while (aptMainLoop()) {
        input::update();
        const u32 down = hidKeysHeld();
        if (g_fatal) {
            if (hidKeysDown() & KEY_START) break;
        } else if ((down & (KEY_START | KEY_SELECT)) == (KEY_START | KEY_SELECT)) {
            break;
        }
        if (osGetTime() - lastStatus >= 500) {
            lastStatus = osGetTime();
            drawStatus(disc, start);
        }
        gspWaitForVBlank();
    }
    fflush(nullptr);
    gfxExit();
    _exit(0);  // guest threads are still running
}
