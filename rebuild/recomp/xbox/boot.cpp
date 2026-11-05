// Boot sequence shared by the hosts (main.cpp, n3ds/platform.cpp): maps default.xbe into guest
// memory, binds the kernel thunk table, and runs the XBE entry point on a guest thread.
#include "gpu.hpp"
#include "xhost.hpp"

#include <chrono>
#include <thread>
#include <sys/stat.h>

namespace xb {
void coreInit();
void kernelTick();

// mkdir -p
static void mkdirs(const std::string& path) {
    for (size_t i = 1; i <= path.size(); ++i)
        if (i == path.size() || path[i] == '/') mkdir(path.substr(0, i).c_str(), 0755);
}

std::vector<uint8_t> g_xbe;
static uint32_t g_base;

static uint32_t le32(size_t o) { uint32_t v; std::memcpy(&v, &g_xbe[o], 4); return v; }

// Section headers live in the mapped image header (guest memory); reference counts are
// guest DWORD/WORDs. Loading copies the section's raw data afresh (as reading it from disc
// would) when its count goes 0 -> 1.
uint32_t xbeLoadSection(uint32_t sh) {
    const uint32_t refs = rd32(sh + 0x18);
    if (refs == 0) {
        const uint32_t va = rd32(sh + 4), vs = rd32(sh + 8), raw = rd32(sh + 0x0C), rs = rd32(sh + 0x10);
        std::memset(gp(va), 0, vs);
        if (raw + std::min(rs, vs) <= g_xbe.size()) std::memcpy(gp(va), &g_xbe[raw], std::min(rs, vs));
        XLOG(1, "XeLoadSection %s (0x%08X, %u bytes)", gstr(rd32(sh + 0x14)), va, vs);
    }
    wr32(sh + 0x18, refs + 1);
    wr16(rd32(sh + 0x1C), static_cast<uint16_t>(rd16(rd32(sh + 0x1C)) + 1));
    wr16(rd32(sh + 0x20), static_cast<uint16_t>(rd16(rd32(sh + 0x20)) + 1));
    return ST_SUCCESS;
}
uint32_t xbeUnloadSection(uint32_t sh) {
    const uint32_t refs = rd32(sh + 0x18);
    if (!refs) return ST_INVALID_PARAMETER;
    wr32(sh + 0x18, refs - 1);
    wr16(rd32(sh + 0x1C), static_cast<uint16_t>(rd16(rd32(sh + 0x1C)) - 1));
    wr16(rd32(sh + 0x20), static_cast<uint16_t>(rd16(rd32(sh + 0x20)) - 1));
    if (refs == 1) XLOG(2, "XeUnloadSection %s", gstr(rd32(sh + 0x14)));
    return ST_SUCCESS;
}
KFUNC(XeLoadSection, 1) { return xbeLoadSection(ARG(c, 0)); }
KFUNC(XeUnloadSection, 1) { return xbeUnloadSection(ARG(c, 0)); }

static void mapXbe() {
    g_base = le32(0x104);
    const uint32_t headers = le32(0x108), imageSize = le32(0x10C);
    if (g_base != 0x10000) die("unexpected XBE base 0x%08X", g_base);
    std::memcpy(gp(g_base), g_xbe.data(), std::min<size_t>(headers, g_xbe.size()));
    const uint32_t n = le32(0x11C), sh = le32(0x120);
    for (uint32_t i = 0; i < n; ++i) {
        const uint32_t h = sh + 56 * i;
        const uint32_t flags = rd32(h), va = rd32(h + 4), vs = rd32(h + 8), raw = rd32(h + 0x0C), rs = rd32(h + 0x10);
        std::memset(gp(va), 0, vs);
        std::memcpy(gp(va), &g_xbe[raw], std::min(rs, vs));
        // The kernel loads preload sections at boot and counts them as referenced. Others are
        // mapped as well (so stray references work) but left at count 0 for XeLoadSection.
        if (flags & 0x2) {
            wr32(h + 0x18, 1);
            wr16(rd32(h + 0x1C), static_cast<uint16_t>(rd16(rd32(h + 0x1C)) + 1));
            wr16(rd32(h + 0x20), static_cast<uint16_t>(rd16(rd32(h + 0x20)) + 1));
        }
    }
    // Kernel imports.
    const uint32_t thunk = le32(0x158) ^ 0x5B6D40B6u;
    int count = 0;
    for (uint32_t a = thunk; rd32(a); a += 4, ++count) {
        const uint32_t v = rd32(a);
        if (!(v & 0x80000000u)) die("kernel import by name is not supported (0x%08X)", v);
        wr32(a, kernelResolve(static_cast<int>(v & 0x7FFFFFFFu)));
    }
    XLOG(1, "XBE mapped: %u sections, %d kernel imports, entry 0x%08X", n, count, le32(0x128) ^ 0xA8FC57ABu);
}

static void reserveXbe() {
    if (!physAllocAt(le32(0x104), le32(0x10C))) die("cannot reserve the XBE image range");
}

static uint32_t tlsSize() {
    const uint32_t tls = le32(0x12C);
    if (!tls) return 0;
    const uint32_t start = rd32(tls), end = rd32(tls + 4), zero = rd32(tls + 16);
    return (((end - start) + zero + 15) & ~15u) + 4;
}


// Starts the game: `xbe` is default.xbe, `game` the disc folder (or the iso: device), `hdd` the
// hard-disk folder. Returns once the entry thread is running.
void bootGame(std::vector<uint8_t> xbe, const std::string& game, const std::string& hdd) {
    g_xbe = std::move(xbe);
    if (g_xbe.size() < 0x180 || std::memcmp(g_xbe.data(), "XBEH", 4) != 0) die("default.xbe is not an XBE");
    memInit();
    coreInit();
    reserveXbe();
    kernelInit();
    mapXbe();
    filesInit(game, hdd);
    {
        // Fable reads T:\xboot.ini before D:\xboot.ini. Its demand paging (its own page tables,
        // a page-fault handler and a swap file on the cache partition, to fit 64 MB) is not needed
        // with host memory behind the whole address space: turn it off.
        const std::string dir = hdd + "/Partition1/TDATA/4d5300d1";
        mkdirs(dir);
        const std::string ini = dir + "/xboot.ini";
        if (FILE* f = fopen(ini.c_str(), "r")) fclose(f);
        else if (FILE* w = fopen(ini.c_str(), "w")) {
            fputs("// Written by FableXbox: host memory replaces the game's demand paging.\r\n"
                  "UseDemandPaging        = false\r\n"
                  "PhysicalSize           = 26544 KB\r\n"
                  "VirtualSize            = 576 MB\r\n"
                  "DiskSize               = 64 MB\r\n", w);
            fclose(w);
        }
    }
    symlinkCreate("\\??\\D:", "\\Device\\CdRom0");
    hleInstall();
    gpu::init();
    std::thread([] {
        setThreadName("kernel tick");
        for (;;) {
            kernelTick();
            debugWatchRearm();
            recomp_preempt_flag = 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }).detach();
    g_gil.lock();
    workerStart();
    XThread* t = newThread(std::max<uint32_t>(le32(0x130), 0x10000), tlsSize(), 0);
    startThread(t, le32(0x128) ^ 0xA8FC57ABu, 0, 0, false);
    g_gil.unlock();
}

} // namespace xb
