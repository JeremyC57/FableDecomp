// Guest memory: physical RAM mirrors, page allocator, small-block pool.
#include "xhost.hpp"

#include <map>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace xb {

namespace {
constexpr uint32_t kPage = 0x1000;
constexpr uint32_t kPages = kPhysSize / kPage;
std::vector<uint8_t> g_used;               // per page: 0 free, 1 used
std::map<uint32_t, uint32_t> g_allocs;     // start page -> page count
std::mutex g_memLock;

// Pool: first-fit free list over 64 KB chunks taken from the page allocator. Each block has
// an 8-byte header { size, magic } before the user pointer.
constexpr uint32_t kPoolMagic = 0x4C4F4F50u;  // "POOL"
std::map<uint32_t, uint32_t> g_poolFree;       // addr -> size (blocks including header)
std::mutex g_poolLock;
}

void memInit() {
    if (!recomp_init_memory()) die("cannot reserve the 4 GiB guest address space");
    const int fd = static_cast<int>(syscall(SYS_memfd_create, "xbox-ram", 0));
    if (fd < 0 || ftruncate(fd, kPhysSize) != 0) die("cannot create the guest RAM (memfd)");
    for (uint32_t base : {0u, kContigBase, kWcBase}) {
        void* want = g_mem + base;
        void* p = mmap(want, kPhysSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
        if (p != want) die("cannot map guest RAM at 0x%08X", base);
    }
    close(fd);
    g_used.assign(kPages, 0);
    // Page 0..15 (null pointer guard) and the GPU instance memory at the top are reserved.
    for (uint32_t p = 0; p < 0x10; ++p) g_used[p] = 1;
    for (uint32_t p = (kPhysSize - kGpuInstanceSize) / kPage; p < kPages; ++p) g_used[p] = 1;
    mprotect(g_mem, 0x10000, PROT_NONE);  // null page faults (identity view only)
}

uint32_t physAlloc(uint32_t size, uint32_t align, uint32_t lo, uint32_t hi, bool fromTop) {
    if (!size) return 0;
    std::lock_guard<std::mutex> l(g_memLock);
    const uint32_t n = (size + kPage - 1) / kPage;
    const uint32_t step = std::max<uint32_t>(1, (align ? align : kPage) / kPage);
    uint32_t first = lo / kPage, last = std::min<uint32_t>(hi >= kPhysSize ? kPhysSize - 1 : hi, kPhysSize - 1) / kPage;
    if (last + 1 < n) return 0;
    auto freeRun = [&](uint32_t p) {
        for (uint32_t i = 0; i < n; ++i)
            if (g_used[p + i]) return false;
        return true;
    };
    if (fromTop) {
        uint32_t p = (last + 1 - n) / step * step;
        for (;; p -= step) {
            if (p < first) return 0;
            if (freeRun(p)) break;
            if (p < step) return 0;
        }
        for (uint32_t i = 0; i < n; ++i) g_used[p + i] = 1;
        g_allocs[p] = n;
        return p * kPage;
    }
    for (uint32_t p = (first + step - 1) / step * step; p + n <= last + 1; p += step)
        if (freeRun(p)) {
            for (uint32_t i = 0; i < n; ++i) g_used[p + i] = 1;
            g_allocs[p] = n;
            return p * kPage;
        }
    return 0;
}

bool physAllocAt(uint32_t addr, uint32_t size) {
    std::lock_guard<std::mutex> l(g_memLock);
    const uint64_t end = (static_cast<uint64_t>(addr) + size + kPage - 1) / kPage;
    const uint32_t p0 = addr / kPage;
    if (end > kPages || end <= p0) return false;
    const uint32_t n = static_cast<uint32_t>(end - p0);
    for (uint32_t i = 0; i < n; ++i)
        if (g_used[p0 + i]) return false;
    for (uint32_t i = 0; i < n; ++i) g_used[p0 + i] = 1;
    g_allocs[p0] = n;
    return true;
}

void physFree(uint32_t addr) {
    std::lock_guard<std::mutex> l(g_memLock);
    auto it = g_allocs.find(physOf(addr) / kPage);
    if (it == g_allocs.end()) return;
    for (uint32_t i = 0; i < it->second; ++i) g_used[it->first + i] = 0;
    g_allocs.erase(it);
}

uint32_t physAllocSize(uint32_t addr) {
    std::lock_guard<std::mutex> l(g_memLock);
    auto it = g_allocs.find(physOf(addr) / kPage);
    return it == g_allocs.end() ? 0 : it->second * kPage;
}

uint32_t physFreeBytes() {
    std::lock_guard<std::mutex> l(g_memLock);
    uint32_t n = 0;
    for (uint8_t u : g_used) n += !u;
    return n * kPage;
}

uint32_t poolAlloc(uint32_t size) {
    const uint32_t need = (size + 8 + 15) & ~15u;
    std::lock_guard<std::mutex> l(g_poolLock);
    for (int attempt = 0; attempt < 2; ++attempt) {
        for (auto it = g_poolFree.begin(); it != g_poolFree.end(); ++it) {
            if (it->second < need) continue;
            const uint32_t a = it->first, have = it->second;
            g_poolFree.erase(it);
            if (have - need >= 32) g_poolFree[a + need] = have - need;
            const uint32_t used = have - need >= 32 ? need : have;
            wr32(a, used);
            wr32(a + 4, kPoolMagic);
            return a + 8;
        }
        const uint32_t chunk = std::max<uint32_t>(0x10000, (need + 0xFFFF) & ~0xFFFFu);
        const uint32_t c = physAlloc(chunk, kPage, 0x10000, kPhysSize, false);
        if (!c) return 0;
        g_poolFree[c] = chunk;  // chunks are not coalesced with each other
    }
    return 0;
}

uint32_t poolAllocZero(uint32_t size) {
    const uint32_t a = poolAlloc(size);
    if (a) std::memset(gp(a), 0, size);
    return a;
}

void poolFree(uint32_t addr) {
    if (!addr) return;
    const uint32_t b = addr - 8;
    if (rd32(b + 4) != kPoolMagic) {
        XLOG(1, "poolFree: bad block 0x%08X", addr);
        return;
    }
    std::lock_guard<std::mutex> l(g_poolLock);
    uint32_t size = rd32(b);
    wr32(b + 4, 0);
    uint32_t start = b;
    auto next = g_poolFree.find(b + size);
    if (next != g_poolFree.end()) {
        size += next->second;
        g_poolFree.erase(next);
    }
    auto prev = g_poolFree.lower_bound(b);
    if (prev != g_poolFree.begin()) {
        --prev;
        if (prev->first + prev->second == b) {
            start = prev->first;
            size += prev->second;
            g_poolFree.erase(prev);
        }
    }
    g_poolFree[start] = size;
}

uint32_t poolSize(uint32_t addr) {
    const uint32_t b = addr - 8;
    return rd32(b + 4) == kPoolMagic ? rd32(b) - 8 : 0;
}

// ============================================================================================
// Virtual memory (NtAllocateVirtualMemory): its own address range, separate from physical RAM,
// as on the console. Reservations are 64 KB aligned; pages are committed lazily by the host.
// ============================================================================================
namespace {
std::map<uint32_t, uint32_t> g_va;  // base -> size
std::mutex g_vaLock;
}

uint32_t vaReserve(uint32_t base, uint32_t size, bool topDown) {
    size = (size + 0xFFF) & ~0xFFFu;
    std::lock_guard<std::mutex> l(g_vaLock);
    auto overlaps = [&](uint32_t b, uint32_t n) {
        auto it = g_va.upper_bound(b + n - 1);
        if (it == g_va.begin()) return false;
        --it;
        return it->first + it->second > b;
    };
    if (base) {
        base &= ~0xFFFu;
        if (base < kVaBase || base + size > kVaEnd || overlaps(base, size)) return 0;
        g_va[base] = size;
        return base;
    }
    if (topDown) {
        uint32_t b = (kVaEnd - size) & ~0xFFFFu;
        while (b >= kVaBase) {
            if (!overlaps(b, size)) { g_va[b] = size; return b; }
            auto it = g_va.upper_bound(b + size - 1);
            --it;
            if (it->first < kVaBase + size) return 0;
            b = (it->first - size) & ~0xFFFFu;
        }
        return 0;
    }
    uint32_t b = kVaBase;
    for (const auto& [rb, rs] : g_va) {
        if (rb >= b + size) break;
        b = std::max(b, (rb + rs + 0xFFFF) & ~0xFFFFu);
    }
    if (b + size > kVaEnd) return 0;
    g_va[b] = size;
    return b;
}

// Region containing `a`: base and size, or {0, 0}.
std::pair<uint32_t, uint32_t> vaRegion(uint32_t a) {
    std::lock_guard<std::mutex> l(g_vaLock);
    auto it = g_va.upper_bound(a);
    if (it == g_va.begin()) return {0, 0};
    --it;
    if (a >= it->first + it->second) return {0, 0};
    return {it->first, it->second};
}

void vaDecommit(uint32_t base, uint32_t size) {
    madvise(g_mem + base, size, MADV_DONTNEED);  // zero-fill on next touch, frees host memory
}

bool vaRelease(uint32_t base) {
    uint32_t size;
    {
        std::lock_guard<std::mutex> l(g_vaLock);
        auto it = g_va.find(base);
        if (it == g_va.end()) return false;
        size = it->second;
        g_va.erase(it);
    }
    vaDecommit(base, size);
    return true;
}

std::string readAnsiString(uint32_t s) {
    if (!s) return {};
    const uint16_t len = rd16(s);
    const uint32_t buf = rd32(s + 4);
    if (!buf) return {};
    return std::string(gstr(buf), len);
}

uint32_t newAnsiString(const std::string& s) {
    const uint32_t str = poolAlloc(8), buf = poolAlloc(static_cast<uint32_t>(s.size()) + 1);
    std::memcpy(gp(buf), s.c_str(), s.size() + 1);
    wr16(str, static_cast<uint16_t>(s.size()));
    wr16(str + 2, static_cast<uint16_t>(s.size() + 1));
    wr32(str + 4, buf);
    return str;
}

} // namespace xb
