// The GL command queue (gl_queue.hpp): a single-producer (game thread) single-consumer
// (render thread) ring of records, replayed in order on the thread that owns the context.
#include "gl_queue_gen.hpp"
#include "host.hpp"

#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#include <vector>
#include <time.h>
#if defined(__linux__)
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace host::gles::gq {
namespace {

struct Header { uint32_t op, len; };  // len: whole record, header included, multiple of 8

inline uint64_t nowNs() { timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return static_cast<uint64_t>(t.tv_sec) * 1000000000ull + static_cast<uint64_t>(t.tv_nsec); }

// Cache lines: the producer's and the consumer's hot fields live on separate lines. They used
// to share one (with g_size / g_ring as well), so every recorded call and every replayed record
// pulled the line over from the other core (A53s: a snoop per access) - false sharing on the
// hottest path of both threads.
constexpr size_t kLine = 64;
alignas(kLine) size_t g_size = 0;  // read-only while the thread runs
uint8_t* g_ring = nullptr;
bool g_cacheIdx = true;  // FABLE_GLES_QUEUE_CACHE=0: re-read the other side's index on every record (the old traffic)
bool g_futex = true;     // FABLE_GLES_QUEUE_FUTEX=0: the producer lock is a std::mutex
alignas(kLine) std::atomic<uint64_t> g_head{0};  // bytes produced (monotonic; position = value % size)
alignas(kLine) std::atomic<uint64_t> g_tail{0};  // bytes consumed
// Producer-private (under the producer lock): the ring position of g_head (no division per
// record), the record being written, and the consumer's tail as last read - it is read again
// only when that copy shows too little room (it can only lag: the room it shows is real).
struct alignas(kLine) Producer {
    uint64_t pendingHead = 0;      // the record being written ends here
    size_t pos = 0, pendingPos = 0;
    uint64_t tailSeen = 0;
} g_prod;
alignas(kLine) std::atomic<bool> g_consumerIdle{false}, g_producerWaiting{false};  // rarely written: read on every record
alignas(kLine) std::atomic<uint32_t> g_producerLock{0};  // 0 free, 1 held, 2 held and a producer sleeps on it
std::mutex g_producer;  // begin()..commit(): the movie streaming thread records calls too (the guest's texture renderer filter)
std::mutex g_mutex;
std::condition_variable g_cvConsumer, g_cvProducer, g_cvSync;
std::thread g_thread;
std::atomic<bool> g_stop{false};
bool g_enabled = false, g_started = false;
thread_local bool t_renderThread = false;
int g_inFlight = 1;

std::atomic<uint64_t> g_framesQueued{0}, g_framesDone{0};
std::atomic<uint64_t> g_renderNs{0};  // the last replayed frame's render-thread time
double g_waitMs = 0;
// replayed records by op, this frame (render thread; a line of its own), and the last frame's total
struct alignas(kLine) OpCounts { uint32_t n[OP_count + 1]; } g_opCounts{};
std::atomic<uint32_t> g_lastFrameCalls{0};

// The producer lock: a futex lock whose free case (nearly always) is one compare-and-swap to take
// and one exchange to release, no pthread call or futex system call (the atomics may still be
// outlined helpers, without -mno-outline-atomics); taken, it spins briefly and then sleeps: the
// other producer (the movie thread) holds it across a frame's texture copy, and any producer
// across reserve()'s wait for ring room, for milliseconds - spinning or yielding through that
// burnt a core (2026-10-12).
#if defined(__linux__)
inline void futexWait(std::atomic<uint32_t>& a, uint32_t v) { syscall(SYS_futex, reinterpret_cast<uint32_t*>(&a), FUTEX_WAIT_PRIVATE, v, nullptr, nullptr, 0); }
inline void futexWake(std::atomic<uint32_t>& a) { syscall(SYS_futex, reinterpret_cast<uint32_t*>(&a), FUTEX_WAKE_PRIVATE, 1, nullptr, nullptr, 0); }
#endif
inline void lockProducer() {
#if defined(__linux__)
    if (g_futex) {
        uint32_t c = 0;
        if (g_producerLock.compare_exchange_strong(c, 1, std::memory_order_acquire, std::memory_order_relaxed)) return;
        for (int i = 0; i < 100; ++i) {  // a record's copy is usually over by now
            c = 0;
            if (g_producerLock.load(std::memory_order_relaxed) == 0 && g_producerLock.compare_exchange_weak(c, 1, std::memory_order_acquire, std::memory_order_relaxed)) return;
        }
        // sleep until the holder wakes us; the state stays 2 while anyone may sleep on it
        c = g_producerLock.exchange(2, std::memory_order_acquire);
        while (c != 0) {
            futexWait(g_producerLock, 2);
            c = g_producerLock.exchange(2, std::memory_order_acquire);
        }
        return;
    }
#endif
    g_producer.lock();
}
inline void unlockProducer() {
#if defined(__linux__)
    if (g_futex) {
        if (g_producerLock.exchange(0, std::memory_order_release) == 2) futexWake(g_producerLock);
        return;
    }
#endif
    g_producer.unlock();
}

std::function<bool()> g_makeCurrent;
std::function<void()> g_release;

// A record that would cross the end of the ring is preceded by a pad record to the end.
void* reserve(size_t len) {
    const uint64_t head = g_head.load(std::memory_order_relaxed);  // written only under the producer lock
    size_t pos = g_prod.pos;
    uint64_t need = len;
    size_t padLen = 0;
    if (pos + len > g_size) { padLen = g_size - pos; need = padLen + len; }
    // wait for room: the consumer frees space as it advances
    if (!g_cacheIdx || g_size - (head - g_prod.tailSeen) < need) {
        g_prod.tailSeen = g_tail.load(std::memory_order_acquire);
        while (g_size - (head - g_prod.tailSeen) < need) {
            g_producerWaiting.store(true);
            std::unique_lock<std::mutex> lk(g_mutex);
            g_cvProducer.wait_for(lk, std::chrono::milliseconds(1), [&] { return g_size - (head - g_tail.load(std::memory_order_acquire)) >= need; });
            g_producerWaiting.store(false);
            g_prod.tailSeen = g_tail.load(std::memory_order_acquire);
        }
    }
    if (padLen) {
        Header* h = reinterpret_cast<Header*>(g_ring + pos);
        h->op = OP_pad, h->len = static_cast<uint32_t>(padLen);
        pos = 0;
    }
    g_prod.pendingHead = head + need;
    g_prod.pendingPos = pos + len == g_size ? 0 : pos + len;
    return g_ring + pos;
}

void wakeConsumer() {
    if (g_consumerIdle.load(std::memory_order_acquire)) {
        std::lock_guard<std::mutex> lk(g_mutex);
        g_cvConsumer.notify_one();
    }
}

void threadMain() {
    t_renderThread = true;
    if (!g_makeCurrent()) { host::log("gles: the render thread could not take the context"); }
    // The busy time is taken per burst of records (one clock read when a burst starts, one
    // when the queue runs dry or a frame ends), not per record: two clock_gettime calls per
    // replayed GL call were 1.3-2.1 % of the device's samples.
    uint64_t burstStart = 0, frameBusy = 0;
    // The consumer's own tail and ring position, and the producer's head as last read: it is
    // read again only once the records up to it are replayed.
    uint64_t tail = g_tail.load(std::memory_order_relaxed), head = tail;
    size_t pos = 0;
    uint32_t frameCalls = 0;
    for (;;) {
        if (!g_cacheIdx || tail == head) head = g_head.load(std::memory_order_acquire);
        if (tail == head) {
            if (burstStart) frameBusy += nowNs() - burstStart, burstStart = 0;
            if (g_stop.load()) break;
            // a short spin (the producer is usually about to add more), then sleep: sched_yield in
            // a loop burned a core (15 % of the device's samples) and the battery. The clock is
            // read every 64 polls, not on each.
            bool got = false;
            const uint64_t spinUntil = nowNs() + 50000;
            for (unsigned i = 1; !got; ++i) {
                got = g_head.load(std::memory_order_acquire) != tail;
                if (!got && (i & 63) == 0 && nowNs() >= spinUntil) break;
            }
            if (!got) {
                g_consumerIdle.store(true, std::memory_order_release);
                std::unique_lock<std::mutex> lk(g_mutex);
                g_cvConsumer.wait_for(lk, std::chrono::milliseconds(2), [&] { return g_head.load(std::memory_order_acquire) != tail || g_stop.load(); });
                g_consumerIdle.store(false, std::memory_order_release);
            }
            continue;
        }
        if (!burstStart) burstStart = nowNs();
        const Header* h = reinterpret_cast<const Header*>(g_ring + pos);
        const uint32_t op = h->op, len = h->len;
        if (len < sizeof(Header) || (len & 7) || len > g_size - pos) { host::log("gles: render thread: corrupt record (op %u, len %u) at %llu", op, len, static_cast<unsigned long long>(tail)); abort(); }
        if (op == OP_call) {
            auto* fn = *reinterpret_cast<std::function<void()>* const*>(h + 1);
            ++g_opCounts.n[OP_call], ++frameCalls;
            (*fn)();
            delete fn;
        } else if (op == OP_frame) {
            const uint64_t now = nowNs();
            frameBusy += now - burstStart, burstStart = now;
            g_renderNs.store(frameBusy);
            frameBusy = 0;
            g_lastFrameCalls.store(frameCalls, std::memory_order_relaxed);
            frameCalls = 0;
            std::memset(g_opCounts.n, 0, sizeof g_opCounts.n);
            g_framesDone.fetch_add(1, std::memory_order_acq_rel);
            { std::lock_guard<std::mutex> lk(g_mutex); g_cvProducer.notify_all(); }
        } else if (op != OP_pad) {
            ++g_opCounts.n[op < OP_count ? op : OP_count], ++frameCalls;
            replay(op, h + 1);
        }
        tail += len;
        pos += len;
        if (pos == g_size) pos = 0;  // a record (or the pad before a wrapped one) ends exactly at the end
        g_tail.store(tail, std::memory_order_release);
        if (g_producerWaiting.load(std::memory_order_acquire)) { std::lock_guard<std::mutex> lk(g_mutex); g_cvProducer.notify_all(); }
    }
    if (g_release) g_release();
}

}  // namespace

bool enabled() { return g_enabled; }
bool onRenderThread() { return t_renderThread; }
bool direct() { return !g_started || t_renderThread; }

void* begin(uint32_t op, size_t args, size_t data) {
    lockProducer();  // released by commit()
    const size_t len = (sizeof(Header) + args + data + 7) & ~static_cast<size_t>(7);
    Header* h = static_cast<Header*>(reserve(len));
    h->op = op, h->len = static_cast<uint32_t>(len);
    return h + 1;
}

void commit() {
    g_prod.pos = g_prod.pendingPos;
    g_head.store(g_prod.pendingHead, std::memory_order_release);
    unlockProducer();
    wakeConsumer();
}

void signal(Sync& s) {
    s.done.store(1, std::memory_order_release);
    std::lock_guard<std::mutex> lk(g_mutex);
    g_cvSync.notify_all();
}

void wait(Sync& s) {
    const uint64_t spinUntil = nowNs() + 100000;
    while (nowNs() < spinUntil) if (s.done.load(std::memory_order_acquire)) return;
    std::unique_lock<std::mutex> lk(g_mutex);
    g_cvSync.wait(lk, [&] { return s.done.load(std::memory_order_acquire) != 0; });
}

void call(std::function<void()> fn) {
    if (direct()) { fn(); return; }
    auto** slot = static_cast<std::function<void()>**>(begin(OP_call, sizeof(void*), 0));
    *slot = new std::function<void()>(std::move(fn));
    commit();
}

void flush() {
    if (direct()) return;
    Sync s;
    call([&s] { signal(s); });
    wait(s);
}

void frameEnd() {
    if (direct()) return;
    begin(OP_frame, 0, 0);
    commit();
    const uint64_t queued = g_framesQueued.fetch_add(1, std::memory_order_acq_rel) + 1;
    const uint64_t t0 = nowNs();
    // at most g_inFlight frames unreplayed: frame N's game work overlaps frame N-1's replay
    while (queued - g_framesDone.load(std::memory_order_acquire) > static_cast<uint64_t>(g_inFlight)) {
        std::unique_lock<std::mutex> lk(g_mutex);
        g_cvProducer.wait_for(lk, std::chrono::milliseconds(1), [&] { return queued - g_framesDone.load(std::memory_order_acquire) <= static_cast<uint64_t>(g_inFlight); });
    }
    g_waitMs = (nowNs() - t0) / 1e6;
}

double renderMs() { return g_renderNs.load() / 1e6; }
double waitMs() { return g_waitMs; }
const uint32_t* frameOpCounts() { return g_opCounts.n; }
uint32_t lastFrameCalls() { return g_lastFrameCalls.load(std::memory_order_relaxed); }

bool start(std::function<bool()> makeCurrent, std::function<void()> release) {
    const char* e = getenv("FABLE_GLES_THREAD");
    g_enabled = !(e && atoi(e) == 0);
    if (!g_enabled) return false;
    if (const char* f = getenv("FABLE_GLES_QUEUE_FRAMES")) g_inFlight = std::max(0, atoi(f));  // 0: the game thread waits for every frame (diagnostic)
    const char* mb = getenv("FABLE_GLES_QUEUE_MB");
    g_size = static_cast<size_t>(mb ? std::max(8, atoi(mb)) : 48) << 20;
    g_ring = static_cast<uint8_t*>(std::malloc(g_size));
    if (!g_ring) { g_enabled = false; return false; }
    g_makeCurrent = std::move(makeCurrent), g_release = std::move(release);
    if (const char* c = getenv("FABLE_GLES_QUEUE_CACHE")) g_cacheIdx = atoi(c) != 0;
    if (const char* c = getenv("FABLE_GLES_QUEUE_FUTEX")) g_futex = atoi(c) != 0;
    g_stop = false;
    g_head = g_tail = 0;
    g_prod = Producer{};
    g_thread = std::thread(threadMain);
    g_started = true;
    std::atexit([] { stop(); });  // exit() while the thread runs would destroy its statics under it
    host::log("gles: render thread on (%zu MB queue, %d frame%s in flight, index cache %s, producer %s)", g_size >> 20, g_inFlight, g_inFlight == 1 ? "" : "s",
              g_cacheIdx ? "on" : "off", g_futex ? "futex lock" : "mutex");
    return true;
}

void stop() {
    if (!g_started) return;
    if (t_renderThread) return;  // exit() called from a queued function: nothing to join from here
    flush();
    g_stop = true;
    { std::lock_guard<std::mutex> lk(g_mutex); g_cvConsumer.notify_all(); }
    g_thread.join();
    g_started = false;
    std::free(g_ring);
    g_ring = nullptr;
}

void genNames(int kind, int n, unsigned* out) {
    // A refill is a round trip: the game thread waits until the render thread has replayed
    // everything recorded before it (a level load's texture uploads included), so the batch
    // is large enough that a region load refills once or twice instead of every 64 objects.
    constexpr int kBatch = 256;
    static std::vector<unsigned> pools[GEN_count];
    std::vector<unsigned>& pool = pools[kind];
    while (n > 0) {
        if (pool.empty()) {
            pool.resize(kBatch);
            unsigned* buf = pool.data();
            auto fn = genFn(kind);
            Sync s;
            call([fn, buf, &s] { fn(kBatch, buf); signal(s); });
            wait(s);
        }
        *out++ = pool.back();
        pool.pop_back();
        --n;
    }
}

size_t texBytes(int width, int height, GLenum format, GLenum type) {
    size_t comps = 4;
    switch (format) {
    case GL_RED: case GL_RED_INTEGER: case GL_ALPHA: case GL_LUMINANCE: case GL_DEPTH_COMPONENT: comps = 1; break;
    case GL_RG: case GL_RG_INTEGER: case GL_LUMINANCE_ALPHA: case GL_DEPTH_STENCIL: comps = 2; break;
    case GL_RGB: case GL_RGB_INTEGER: comps = 3; break;
    default: comps = 4; break;
    }
    size_t per = 1;
    switch (type) {
    case GL_UNSIGNED_BYTE: case GL_BYTE: per = comps; break;
    case GL_UNSIGNED_SHORT: case GL_SHORT: case GL_HALF_FLOAT: per = 2 * comps; break;
    case GL_UNSIGNED_INT: case GL_INT: case GL_FLOAT: per = 4 * comps; break;
    case GL_UNSIGNED_SHORT_5_6_5: case GL_UNSIGNED_SHORT_4_4_4_4: case GL_UNSIGNED_SHORT_5_5_5_1: per = 2; break;
    case GL_UNSIGNED_INT_24_8: case GL_UNSIGNED_INT_2_10_10_10_REV: case GL_UNSIGNED_INT_10F_11F_11F_REV: case GL_UNSIGNED_INT_5_9_9_9_REV: per = 4; break;
    case GL_FLOAT_32_UNSIGNED_INT_24_8_REV: per = 8; break;
    default: per = 4 * comps; break;
    }
    return static_cast<size_t>(width) * static_cast<size_t>(height) * per;  // GL_UNPACK_ALIGNMENT is 1 in the renderer
}
}  // namespace host::gles::gq

#include "gl_queue_gen.inc"
