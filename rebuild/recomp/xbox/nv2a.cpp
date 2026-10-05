// NV2A GPU device model: MMIO registers, the PFIFO DMA pusher, PGRAPH object dispatch, the
// Kelvin (3D) state machine's synchronisation methods, vblank and interrupts.
//
// Register state lives in guest memory at 0xFD000000 (so plain loads see the last value);
// Direct3D's own functions are lifted with --mmio, so their register accesses come here.
// The pusher runs on its own host thread, consuming the push buffer between DMA_GET and
// DMA_PUT. Rendering is delegated to the renderer (gpu.hpp).
#include "gpu.hpp"
#include "nv2a_methods.h"
#include "xhost.hpp"

#include <atomic>
#include <chrono>
#include <thread>

namespace xb {
extern void (*g_irqEoi)(uint32_t vector);
}

namespace xb::gpu {

namespace {
constexpr uint32_t kMmio = 0xFD000000u;
constexpr uint32_t kRamin = kMmio + 0x700000u;
constexpr uint32_t kGpuIrq = 3;

// Blocks.
constexpr uint32_t PMC = 0x000000, PFIFO = 0x002000, PTIMER = 0x009000, PFB = 0x100000, PGRAPH = 0x400000,
                   PCRTC = 0x600000, PRAMDAC = 0x680000, USER = 0x800000;

std::mutex g_dev;                 // device state (not the guest lock)
std::condition_variable g_kick;   // wakes the pusher
std::atomic<bool> g_kicked{false};

uint32_t& R(uint32_t off) { return *reinterpret_cast<uint32_t*>(gp(kMmio + off)); }

struct Intr { uint32_t pending = 0, enabled = 0; };
Intr g_pmcEn, g_pfifo, g_pgraph, g_pcrtc, g_ptimer;
uint32_t g_pmcEnabled = 0;
bool g_irqLine = false;

bool g_waitNop = false, g_waitCtx = false, g_waitFlip = false;
uint32_t g_coreClockHz = 233333324;
uint32_t g_ptNum = 1, g_ptDen = 1;
uint64_t g_frameCount = 0;

// PGRAPH: subchannel -> object instance and class.
uint32_t g_subInst[8], g_subClass[8];

State g_state;  // Kelvin state shared with the renderer

// ---- 2D: context surfaces (NV062) and image blit (NV09F), done in guest memory ----
struct Surf2D { uint32_t dmaSrc = 0, dmaDst = 0, format = 0, pitch = 0, offSrc = 0, offDst = 0; } g_s2d;
struct Blit { uint32_t in = 0, out = 0, op = 3; } g_blit;
uint32_t bppOf(uint32_t fmt) {
    switch (fmt) {
    case 0x01: return 1;                  // Y8
    case 0x04: case 0x02: case 0x03: return 2;  // R5G6B5, X1R5G5B5...
    default: return 4;                    // X8R8G8B8, A8R8G8B8, Y32
    }
}


uint32_t pmcPending() {
    uint32_t p = 0;
    if (g_pfifo.pending & g_pfifo.enabled) p |= 1u << 8;
    if (g_pgraph.pending & g_pgraph.enabled) p |= 1u << 12;
    if (g_ptimer.pending & g_ptimer.enabled) p |= 1u << 20;
    if (g_pcrtc.pending & g_pcrtc.enabled) p |= 1u << 24;
    return p;
}

// Called with g_dev held. The ISR runs on the kernel worker.
void updateIrq(bool haveGil) {
    const bool line = pmcPending() != 0 && g_pmcEnabled != 0;  // INTR_EN_0 is a master enable
    if (line && !g_irqLine) {
        g_irqLine = true;
        (void)haveGil;
        interruptRaise(kGpuIrq);
    } else if (!line) {
        g_irqLine = false;
    }
}

// After the ISR: a source still pending re-asserts the line.
void eoi(uint32_t vector) {
    if (vector != kGpuIrq) return;
    std::lock_guard<std::mutex> l(g_dev);
    g_irqLine = false;
    updateIrq(true);
}

uint64_t ptimerTicks() {
    const uint64_t ns = monoTime100ns() * 100;
    const unsigned __int128 t = static_cast<unsigned __int128>(ns) * g_coreClockHz / 1000000000u * g_ptDen / (g_ptNum ? g_ptNum : 1);
    return static_cast<uint64_t>(t) << 5;
}

uint32_t surfaceField(uint32_t shift) { return (R(PGRAPH + 0x710) >> shift) & 7; }
void setSurfaceField(uint32_t shift, uint32_t v) {
    R(PGRAPH + 0x710) = (R(PGRAPH + 0x710) & ~(7u << shift)) | ((v & 7) << shift);
}
bool flipStallDone() { return surfaceField(24) != surfaceField(20); }  // READ_3D != WRITE_3D

void kick() {
    g_kicked = true;
    g_kick.notify_all();
}
}  // namespace

State& state() { return g_state; }

void surfaces2d(uint32_t method, uint32_t param) {
    switch (method) {
    case NV062_SET_CONTEXT_DMA_IMAGE_SOURCE: g_s2d.dmaSrc = param; break;
    case NV062_SET_CONTEXT_DMA_IMAGE_DESTIN: g_s2d.dmaDst = param; break;
    case NV062_SET_COLOR_FORMAT: g_s2d.format = param; break;
    case NV062_SET_PITCH: g_s2d.pitch = param; break;
    case NV062_SET_OFFSET_SOURCE: g_s2d.offSrc = param; break;
    case NV062_SET_OFFSET_DESTIN: g_s2d.offDst = param; break;
    default: break;
    }
}

void imageBlit(uint32_t method, uint32_t param) {
    switch (method) {
    case NV09F_SET_OPERATION: g_blit.op = param; break;
    case NV09F_CONTROL_POINT_IN: g_blit.in = param; break;
    case NV09F_CONTROL_POINT_OUT: g_blit.out = param; break;
    case NV09F_SIZE: {
        renderer().flush();
        const uint32_t w = param & 0xFFFF, h = param >> 16, bpp = bppOf(g_s2d.format);
        const uint32_t sp = g_s2d.pitch & 0xFFFF, dp = g_s2d.pitch >> 16;
        const uint32_t src = kContigBase + dmaAddress(g_s2d.dmaSrc, nullptr) + g_s2d.offSrc;
        const uint32_t dst = kContigBase + dmaAddress(g_s2d.dmaDst, nullptr) + g_s2d.offDst;
        const uint32_t sx = g_blit.in & 0xFFFF, sy = g_blit.in >> 16, dx = g_blit.out & 0xFFFF, dy = g_blit.out >> 16;
        for (uint32_t y = 0; y < h; ++y)
            std::memmove(gp(dst + (dy + y) * dp + dx * bpp), gp(src + (sy + y) * sp + sx * bpp), static_cast<size_t>(w) * bpp);
        XLOG(3, "NV2A blit %ux%u (%u bpp) 0x%08X -> 0x%08X", w, h, bpp, src, dst);
        break;
    }
    default: break;
    }
}

uint32_t dmaAddress(uint32_t instance, uint32_t* limit) {
    const uint32_t flags = rd32(kRamin + instance), lim = rd32(kRamin + instance + 4), frame = rd32(kRamin + instance + 8);
    if (limit) *limit = lim;
    return ((frame & 0xFFFFF000u) | (flags >> 20)) & 0x07FFFFFFu;
}

// ============================================================================================
// MMIO
// ============================================================================================
uint32_t mmioRead(uint32_t a, int size) {
    const uint32_t off = a - kMmio;
    std::lock_guard<std::mutex> l(g_dev);
    uint32_t r;
    switch (off & ~3u) {
    case PMC + 0x000: r = 0x02A000A3; break;  // NV2A A3
    case PMC + 0x100: r = pmcPending(); break;
    case PMC + 0x140: r = g_pmcEnabled; break;
    case PFIFO + 0x100: r = g_pfifo.pending; break;
    case PFIFO + 0x140: r = g_pfifo.enabled; break;
    case PFIFO + 0x400: r = 0x10; break;  // RUNOUT_STATUS: low mark (empty)
    case PFIFO + 0x1214: r = R(off & ~3u) | 0x10; break;  // CACHE1_STATUS: empty
    case PTIMER + 0x100: r = g_ptimer.pending; break;
    case PTIMER + 0x400: r = static_cast<uint32_t>(ptimerTicks()); break;
    case PTIMER + 0x410: r = static_cast<uint32_t>(ptimerTicks() >> 32) & 0x1FFFFFFF; break;
    case PFB + 0x20C: r = kPhysSize; break;  // CSTATUS: RAM size
    case PFB + 0x410: r = 0; break;           // WBC: no flush pending
    case PGRAPH + 0x100: r = g_pgraph.pending; break;
    case PGRAPH + 0x140: r = g_pgraph.enabled; break;
    case PGRAPH + 0x700: r = 0; break;        // STATUS: idle
    case PCRTC + 0x100: r = g_pcrtc.pending; break;
    case PCRTC + 0x140: r = g_pcrtc.enabled; break;
    case PCRTC + 0x808: {                     // RASTER: scanline position
        const uint64_t t = monoTime100ns() % 166833;
        r = static_cast<uint32_t>(t * 525 / 166833);
        break;
    }
    case PRAMDAC + 0x514: r = 0xE8000000u; break;  // PLL_TEST_COUNTER: all PLLs locked
    default:
        if (off >= USER && off < USER + 0x800000) {
            switch (off & 0xFFFC) {
            case 0x40: r = R(PFIFO + 0x1240); break;  // DMA_PUT
            case 0x44: r = R(PFIFO + 0x1244); break;  // DMA_GET
            case 0x48: r = R(PFIFO + 0x1248); break;  // REF
            default: r = R(off & ~3u); break;
            }
        } else {
            r = R(off & ~3u);
        }
        break;
    }
    if (size == 4) return r;
    const uint32_t shift = (off & 3) * 8;
    return (r >> shift) & (size == 1 ? 0xFFu : 0xFFFFu);
}

void mmioWrite(uint32_t a, uint32_t v, int size) {
    const uint32_t off = a - kMmio;
    std::lock_guard<std::mutex> l(g_dev);
    if (size != 4) {  // sub-word writes: merge into the register
        const uint32_t shift = (off & 3) * 8, m = (size == 1 ? 0xFFu : 0xFFFFu) << shift;
        v = (R(off & ~3u) & ~m) | ((v << shift) & m);
    }
    const uint32_t reg = off & ~3u;
    if (g_logLevel >= 3 || (g_logLevel >= 2 && (reg < 0x4000 || reg >= USER))) XLOG(2, "NV2A write 0x%06X = 0x%08X", reg, v);
    switch (reg) {
    case PMC + 0x100: break;  // derived from the sources
    case PMC + 0x140: g_pmcEnabled = v; updateIrq(true); break;
    case PFIFO + 0x100: g_pfifo.pending &= ~v; updateIrq(true); break;
    case PFIFO + 0x140: g_pfifo.enabled = v; updateIrq(true); break;
    case PTIMER + 0x100: g_ptimer.pending &= ~v; updateIrq(true); break;
    case PTIMER + 0x140: g_ptimer.enabled = v; updateIrq(true); break;
    case PTIMER + 0x200: g_ptNum = v ? v : 1; break;
    case PTIMER + 0x210: g_ptDen = v ? v : 1; break;
    case PGRAPH + 0x100:
        g_pgraph.pending &= ~v;
        if (!(g_pgraph.pending & (1u << 20))) g_waitNop = false;
        if (!(g_pgraph.pending & (1u << 12))) g_waitCtx = false;
        updateIrq(true);
        kick();
        break;
    case PGRAPH + 0x140: g_pgraph.enabled = v; updateIrq(true); break;
    case PGRAPH + 0x71C:  // INCREMENT
        if (v & 2) {
            const uint32_t mod = surfaceField(28);
            setSurfaceField(24, mod ? (surfaceField(24) + 1) % mod : 0);
            kick();
        }
        break;
    case PGRAPH + 0x720: R(reg) = v; kick(); break;  // FIFO access
    case PGRAPH + 0x788:                               // CHANNEL_CTX_TRIGGER
        if (v & 1) R(PGRAPH + 0x148) = rd32(kRamin + ((R(PGRAPH + 0x784) & 0xFFFF) << 4));  // CTX_USER
        break;
    case PCRTC + 0x100: g_pcrtc.pending &= ~v; updateIrq(true); break;
    case PCRTC + 0x140: g_pcrtc.enabled = v; updateIrq(true); break;
    case PCRTC + 0x800:
        R(reg) = v & 0x07FFFFFF;
        g_state.scanout = v & 0x07FFFFFF;
        break;
    case PRAMDAC + 0x500: {  // NVPLL: core clock
        R(reg) = v;
        const uint32_t m = v & 0xFF, n = (v >> 8) & 0xFF, p = (v >> 16) & 0xF;
        g_coreClockHz = m ? static_cast<uint32_t>(16666666ull * n / (1u << p) / m) : 0;
        break;
    }
    default:
        if (off >= USER && off < USER + 0x800000) {
            switch (off & 0xFFFC) {
            case 0x40: R(PFIFO + 0x1240) = v; kick(); break;
            case 0x44: R(PFIFO + 0x1244) = v; break;
            case 0x48: R(PFIFO + 0x1248) = v; break;
            default: R(reg) = v; break;
            }
            return;
        }
        R(reg) = v;
        if (reg == PFIFO + 0x1240 || reg == PFIFO + 0x1220 || reg == PFIFO + 0x1200) kick();
        return;
    }
    if (!(reg >= USER)) R(reg) = (reg == PCRTC + 0x800) ? (v & 0x07FFFFFF) : v;
}

// ============================================================================================
// PGRAPH
// ============================================================================================
namespace {

uint32_t ramhtLookup(uint32_t handle, uint32_t* engine) {
    const uint32_t ramht = R(PFIFO + 0x210);
    const uint32_t size = 1u << (((ramht >> 16) & 3) + 12);
    const uint32_t base = ((ramht >> 4) & 0x1F) << 12;
    const uint32_t bits = static_cast<uint32_t>(__builtin_ctz(size)) - 1;
    uint32_t hash = 0;
    for (uint32_t h = handle; h; h >>= bits) hash ^= h & ((1u << bits) - 1);
    const uint32_t chid = R(PFIFO + 0x1204) & 0x1F;
    hash ^= chid << (bits - 4);
    // Linear probe (the hardware searches a few entries on collision).
    for (uint32_t i = 0; i < 16; ++i) {
        const uint32_t e = base + ((hash + i) * 8) % size;
        if (rd32(kRamin + e) == handle) {
            const uint32_t ctx = rd32(kRamin + e + 4);
            if (engine) *engine = (ctx >> 16) & 3;
            return (ctx & 0xFFFF) << 4;
        }
    }
    XLOG(0, "NV2A: RAMHT lookup failed for handle 0x%08X", handle);
    return 0;
}

void raisePgraph(uint32_t bit) {
    g_pgraph.pending |= bit;
    updateIrq(false);
}

void kelvin(uint32_t method, uint32_t param, const uint32_t* params, uint32_t count, bool inc, uint32_t& used);

// Returns words consumed (>= 1).
uint32_t pgraphMethod(uint32_t sub, uint32_t method, uint32_t param, const uint32_t* params, uint32_t avail, bool inc) {
    if (method == 0) {
        g_subInst[sub] = param;
        g_subClass[sub] = rd32(kRamin + param) & 0xFF;
        // First use of the channel: Direct3D's ISR loads the context (software switch).
        if (!(R(PGRAPH + 0x144) & (1u << 16))) {
            g_waitCtx = true;
            raisePgraph(1u << 12);
        }
        XLOG(2, "NV2A: subchannel %u = object 0x%X class 0x%02X", sub, param, g_subClass[sub]);
        return 1;
    }
    uint32_t used = 1;
    switch (g_subClass[sub]) {
    case NV_KELVIN_PRIMITIVE: kelvin(method, param, params, avail, inc, used); break;
    case NV_CONTEXT_SURFACES_2D: surfaces2d(method, param); break;
    case NV_IMAGE_BLIT: imageBlit(method, param); break;
    case NV_BETA: break;
    default:
        XLOG(2, "NV2A: method 0x%X on class 0x%02X ignored", method, g_subClass[sub]);
        break;
    }
    return used;
}

void kelvin(uint32_t method, uint32_t param, const uint32_t* params, uint32_t avail, bool inc, uint32_t& used) {
    State& s = g_state;
    if (g_logLevel >= 4) XLOG(4, "kelvin 0x%04X = 0x%08X", method, param);
    if (method < 0x2000) s.regs[method / 4] = param;
    switch (method) {
    case NV097_NO_OPERATION:
        if (param) {  // software method: the ISR answers, the pusher waits
            R(PGRAPH + 0x704) = method;
            R(PGRAPH + 0x708) = param;
            R(PGRAPH + 0x108) = 1;  // NSOURCE: notification
            g_waitNop = true;
            raisePgraph(1u << 20);
        }
        break;
    case NV097_WAIT_FOR_IDLE: renderer().flush(); break;
    case NV097_SET_FLIP_READ: setSurfaceField(24, param); break;
    case NV097_SET_FLIP_WRITE: setSurfaceField(20, param); break;
    case NV097_SET_FLIP_MODULO: setSurfaceField(28, param); break;
    case NV097_FLIP_INCREMENT_WRITE: {
        const uint32_t mod = surfaceField(28);
        setSurfaceField(20, mod ? (surfaceField(20) + 1) % mod : 0);
        ++g_frameCount;
        break;
    }
    case NV097_FLIP_STALL:
        renderer().flush();
        renderer().endFrame();
        g_waitFlip = true;
        break;
    case NV097_SET_CONTEXT_DMA_NOTIFIES: case NV097_SET_CONTEXT_DMA_A: case NV097_SET_CONTEXT_DMA_B:
    case NV097_SET_CONTEXT_DMA_STATE: case NV097_SET_CONTEXT_DMA_COLOR: case NV097_SET_CONTEXT_DMA_ZETA:
    case NV097_SET_CONTEXT_DMA_VERTEX_A: case NV097_SET_CONTEXT_DMA_VERTEX_B: case NV097_SET_CONTEXT_DMA_SEMAPHORE:
    case NV097_SET_CONTEXT_DMA_REPORT:
        break;  // stored in regs (instance addresses, already translated by the puller)
    case NV097_BACK_END_WRITE_SEMAPHORE_RELEASE: {
        renderer().flush();
        const uint32_t base = dmaAddress(s.regs[NV097_SET_CONTEXT_DMA_SEMAPHORE / 4], nullptr);
        wr32(kContigBase + base + s.regs[NV097_SET_SEMAPHORE_OFFSET / 4], param);
        XLOG(3, "NV2A semaphore 0x%08X <- %u (dma 0x%X)", base + s.regs[NV097_SET_SEMAPHORE_OFFSET / 4], param, s.regs[NV097_SET_CONTEXT_DMA_SEMAPHORE / 4]);
        break;
    }
    case NV097_GET_REPORT: {
        renderer().flush();
        const uint32_t base = dmaAddress(s.regs[NV097_SET_CONTEXT_DMA_REPORT / 4], nullptr);
        const uint32_t a = kContigBase + base + (param & 0x00FFFFFF);
        wr64(a, ptimerTicks());
        wr32(a + 8, renderer().zpassCount());
        wr32(a + 12, 0);
        break;
    }
    case NV097_CLEAR_SURFACE: renderer().clear(param); break;
    case NV097_SET_BEGIN_END:
        if (param) renderer().begin(param);
        else renderer().end();
        break;
    case NV097_DRAW_ARRAYS: {
        // Non-incrementing runs batch several (start, count) ranges.
        uint32_t n = inc ? 1 : avail;
        for (uint32_t i = 0; i < n; ++i) renderer().drawArrays(params[i] & 0xFFFFFF, (params[i] >> 24) + 1);
        used = n;
        break;
    }
    case NV097_INLINE_ARRAY: {
        const uint32_t n = inc ? 1 : avail;
        renderer().inlineArray(params, n);
        used = n;
        break;
    }
    // PGRAPH mirrors Direct3D reads back in its interrupt handler: it passes software-callback
    // pointers and arguments through the clear values.
    case NV097_SET_ZSTENCIL_CLEAR_VALUE: R(PGRAPH + 0x1A88) = param; break;
    case NV097_SET_COLOR_CLEAR_VALUE: R(PGRAPH + 0x186C) = param; break;
    case NV097_SET_TRANSFORM_PROGRAM_LOAD: s.programLoad = param; break;
    case NV097_SET_TRANSFORM_CONSTANT_LOAD: s.constantLoad = param; break;
    default:
        if (method >= NV097_SET_TRANSFORM_PROGRAM && method < NV097_SET_TRANSFORM_PROGRAM + 0x80) {
            const uint32_t word = (method - NV097_SET_TRANSFORM_PROGRAM) / 4 % 4;
            if (s.programLoad < 136) s.program[s.programLoad][word] = param;
            if (word == 3) ++s.programLoad;
            ++s.programGeneration;
        } else if (method >= NV097_SET_TRANSFORM_CONSTANT && method < NV097_SET_TRANSFORM_CONSTANT + 0x80) {
            const uint32_t word = (method - NV097_SET_TRANSFORM_CONSTANT) / 4 % 4;
            if (s.constantLoad < 192) std::memcpy(&s.constants[s.constantLoad][word], &param, 4);
            if (word == 3) ++s.constantLoad;
            s.constantsDirty = true;
        } else if (method == 0x1800 || method == 0x1808) {  // ARRAY_ELEMENT16 / ARRAY_ELEMENT32
            const uint32_t n = inc ? 1 : avail;
            renderer().arrayElements(params, n, method == 0x1800);
            used = n;
        } else if (method >= 0x1880 && method < 0x1B00) {  // inline vertex attributes (2F, 2S, 4UB, 4S, 4F)
            renderer().vertexAttribute(method, param);
        }
        break;
    }
}

}  // namespace

// ============================================================================================
// DMA pusher
// ============================================================================================
namespace {
uint32_t g_dmaState = 0;   // method, subchannel, count, non-inc
uint32_t g_subroutine = 0; // return address | 1 when active
struct Pending { uint32_t method, sub, count; bool inc; } g_cmd{0, 0, 0, true};

bool shouldStall() {
    if (!(R(PGRAPH + 0x720) & 1)) return true;  // PGRAPH FIFO access disabled
    if (g_waitNop || g_waitCtx) return true;
    if (g_waitFlip) {
        if (!flipStallDone()) return true;
        g_waitFlip = false;
    }
    return false;
}

void runPusher() {
    if (!(R(PFIFO + 0x1200) & 1) || !(R(PFIFO + 0x1220) & 1)) return;  // PUSH0 / DMA_PUSH access
    uint32_t limit = 0;
    const uint32_t base = dmaAddress((R(PFIFO + 0x122C) & 0xFFFF) << 4, &limit);
    int budget = 20000;
    while (!shouldStall() && budget-- > 0) {
        uint32_t get = R(PFIFO + 0x1244);
        const uint32_t put = R(PFIFO + 0x1240);
        if (get == put) break;
        if (get > limit || get >= kPhysSize) {
            XLOG(0, "NV2A: DMA_GET 0x%X beyond the push buffer (limit 0x%X)", get, limit);
            R(PFIFO + 0x1244) = put;
            break;
        }
        // GPU accesses go through the contiguous mirror: physical page 0 (where the first
        // jump into the push buffer lives) is a null guard in the identity view.
        const uint32_t* words = reinterpret_cast<const uint32_t*>(gp(kContigBase + base + get));
        const uint32_t word = words[0];
        if (g_cmd.count) {
            const uint32_t avail = (put > get ? put - get : 0) / 4;
            const uint32_t n = std::min(g_cmd.count, avail);
            uint32_t done = 0;
            while (done < n && !shouldStall()) {
                uint32_t param = words[done];
                if (g_cmd.method == 0 || (g_cmd.method >= 0x180 && g_cmd.method < 0x200)) param = ramhtLookup(param, nullptr);
                const uint32_t used = pgraphMethod(g_cmd.sub, g_cmd.method, param, words + done, g_cmd.inc ? 1 : n - done, g_cmd.inc);
                done += used;
                if (g_cmd.inc) g_cmd.method += 4 * used;
            }
            g_cmd.count -= done;
            R(PFIFO + 0x1244) = get + 4 * done;
            continue;
        }
        get += 4;
        if ((word & 0xE0000003) == 0x20000000) {  // old jump
            get = word & 0x1FFFFFFF;
        } else if ((word & 3) == 1) {  // jump
            get = word & 0xFFFFFFFC;
        } else if ((word & 3) == 2) {  // call
            g_subroutine = get | 1;
            get = word & 0xFFFFFFFC;
        } else if (word == 0x00020000) {  // return
            if (g_subroutine & 1) get = g_subroutine & ~3u;
            g_subroutine = 0;
        } else if ((word & 0xE0030003) == 0) {  // increasing methods
            g_cmd = {word & 0x1FFC, (word >> 13) & 7, (word >> 18) & 0x7FF, true};
        } else if ((word & 0xE0030003) == 0x40000000) {  // non-increasing methods
            g_cmd = {word & 0x1FFC, (word >> 13) & 7, (word >> 18) & 0x7FF, false};
        } else {
            XLOG(0, "NV2A: bad push buffer word 0x%08X at 0x%X", word, get - 4);
        }
        R(PFIFO + 0x1244) = get;
    }
}


void pusherMain() {
    using clock = std::chrono::steady_clock;
    auto nextVblank = clock::now() + std::chrono::microseconds(16683);
    std::unique_lock<std::mutex> l(g_dev);
    for (;;) {
        g_kick.wait_until(l, std::min(nextVblank, clock::now() + std::chrono::milliseconds(2)), [] { return g_kicked.load(); });
        g_kicked = false;
        runPusher();
        if (clock::now() >= nextVblank) {
            nextVblank += std::chrono::microseconds(16683);
            if (clock::now() > nextVblank) nextVblank = clock::now() + std::chrono::microseconds(16683);
            g_pcrtc.pending |= 1;  // VBLANK
            updateIrq(false);
            renderer().vblank();
        }
    }
}
}  // namespace

void init() {
    g_irqEoi = eoi;
    R(PFIFO + 0x1214) |= 0x10;
    std::thread(pusherMain).detach();
}

}  // namespace xb::gpu

// A guest thread polling a device register lets others run (as on a timer interrupt).
static void pollYield(uint32_t) { RECOMP_SAFEPOINT; }

extern "C" uint32_t recomp_mmio_read(uint32_t a, int size) {
    pollYield(a);
    if (a >= 0xFD000000u && a < 0xFE000000u) return xb::gpu::mmioRead(a, size);
    return size == 4 ? rd32(a) : size == 2 ? rd16(a) : rd8(a);  // MCPX (APU, USB...): plain memory
}
extern "C" void recomp_mmio_write(uint32_t a, uint32_t v, int size) {
    if (a >= 0xFD000000u && a < 0xFE000000u) return xb::gpu::mmioWrite(a, v, size);
    if (size == 4) wr32(a, v);
    else if (size == 2) wr16(a, static_cast<uint16_t>(v));
    else wr8(a, static_cast<uint8_t>(v));
}
