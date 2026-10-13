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

namespace host::gles::gq {
namespace {

struct Header { uint32_t op, len; };  // len: whole record, header included, multiple of 8

inline uint64_t nowNs() { timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return static_cast<uint64_t>(t.tv_sec) * 1000000000ull + static_cast<uint64_t>(t.tv_nsec); }

size_t g_size = 0;
uint8_t* g_ring = nullptr;
std::atomic<uint64_t> g_head{0};  // bytes produced (monotonic; position = value % size)
std::atomic<uint64_t> g_tail{0};  // bytes consumed
std::atomic<bool> g_consumerIdle{false}, g_producerWaiting{false};
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
uint64_t g_pendingHead = 0;           // producer: the record being written ends here

std::function<bool()> g_makeCurrent;
std::function<void()> g_release;

// A record that would cross the end of the ring is preceded by a pad record to the end.
void* reserve(size_t len) {
    const uint64_t head = g_head.load(std::memory_order_relaxed);
    size_t pos = static_cast<size_t>(head % g_size);
    uint64_t need = len;
    size_t padLen = 0;
    if (pos + len > g_size) { padLen = g_size - pos; need = padLen + len; }
    // wait for room: the consumer frees space as it advances
    while (g_size - (head - g_tail.load(std::memory_order_acquire)) < need) {
        g_producerWaiting.store(true);
        std::unique_lock<std::mutex> lk(g_mutex);
        g_cvProducer.wait_for(lk, std::chrono::milliseconds(1), [&] { return g_size - (head - g_tail.load(std::memory_order_acquire)) >= need; });
        g_producerWaiting.store(false);
    }
    if (padLen) {
        Header* h = reinterpret_cast<Header*>(g_ring + pos);
        h->op = OP_pad, h->len = static_cast<uint32_t>(padLen);
        pos = 0;
    }
    g_pendingHead = head + need;
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
    uint64_t frameStart = 0, frameBusy = 0;
    for (;;) {
        uint64_t tail = g_tail.load(std::memory_order_relaxed);
        uint64_t head = g_head.load(std::memory_order_acquire);
        if (tail == head) {
            if (g_stop.load()) break;
            // spin briefly (the producer is usually about to add more), then sleep
            // a short spin (the producer is usually about to add more), then sleep: sched_yield in
            // a loop burned a core (15 % of the device's samples) and the battery
            bool got = false;
            const uint64_t spinUntil = nowNs() + 50000;
            while (!got && nowNs() < spinUntil) got = g_head.load(std::memory_order_acquire) != tail;
            if (!got) {
                g_consumerIdle.store(true, std::memory_order_release);
                std::unique_lock<std::mutex> lk(g_mutex);
                g_cvConsumer.wait_for(lk, std::chrono::milliseconds(2), [&] { return g_head.load(std::memory_order_acquire) != tail || g_stop.load(); });
                g_consumerIdle.store(false, std::memory_order_release);
            }
            continue;
        }
        const uint64_t t0 = nowNs();
        if (!frameStart) frameStart = t0;
        const size_t pos = static_cast<size_t>(tail % g_size);
        const Header* h = reinterpret_cast<const Header*>(g_ring + pos);
        const uint32_t op = h->op, len = h->len;
        if (op == OP_call) {
            auto* fn = *reinterpret_cast<std::function<void()>* const*>(h + 1);
            (*fn)();
            delete fn;
        } else if (op == OP_frame) {
            frameBusy += nowNs() - t0;
            g_renderNs.store(frameBusy);
            frameBusy = 0, frameStart = 0;
            g_framesDone.fetch_add(1, std::memory_order_acq_rel);
            { std::lock_guard<std::mutex> lk(g_mutex); g_cvProducer.notify_all(); }
        } else if (op != OP_pad) {
            replay(op, h + 1);
        }
        if (op != OP_frame) frameBusy += nowNs() - t0;
        g_tail.store(tail + len, std::memory_order_release);
        if (g_producerWaiting.load(std::memory_order_acquire)) { std::lock_guard<std::mutex> lk(g_mutex); g_cvProducer.notify_all(); }
    }
    if (g_release) g_release();
}

}  // namespace

bool enabled() { return g_enabled; }
bool onRenderThread() { return t_renderThread; }
bool direct() { return !g_started || t_renderThread; }

void* begin(uint32_t op, size_t args, size_t data) {
    const size_t len = (sizeof(Header) + args + data + 7) & ~static_cast<size_t>(7);
    Header* h = static_cast<Header*>(reserve(len));
    h->op = op, h->len = static_cast<uint32_t>(len);
    return h + 1;
}

void commit() {
    g_head.store(g_pendingHead, std::memory_order_release);
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
    g_stop = false;
    g_head = g_tail = 0;
    g_thread = std::thread(threadMain);
    g_started = true;
    std::atexit([] { stop(); });  // exit() while the thread runs would destroy its statics under it
    host::log("gles: render thread on (%zu MB queue, %d frame%s in flight)", g_size >> 20, g_inFlight, g_inFlight == 1 ? "" : "s");
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
    static std::vector<unsigned> pools[GEN_count];
    std::vector<unsigned>& pool = pools[kind];
    while (n > 0) {
        if (pool.empty()) {
            pool.resize(64);
            unsigned* buf = pool.data();
            auto fn = genFn(kind);
            Sync s;
            call([fn, buf, &s] { fn(64, buf); signal(s); });
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
