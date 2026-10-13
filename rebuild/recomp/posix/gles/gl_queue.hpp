// The GL command queue: the game thread records the renderer's gl* calls and a render
// thread that owns the context replays them, so the driver's per-draw CPU work (the Mali
// blob spends ~35 us per draw on the handheld) overlaps the game's own frame instead of
// adding to it. One frame may be in flight (FABLE_GLES_QUEUE_FRAMES); FABLE_GLES_THREAD=0
// turns the thread off and every wrapper calls the driver directly.
//
// Records are variable-size and contiguous in a ring (header, argument struct, data copy).
// Calls whose result the game thread needs now (glGen*, glGet*, glCreate*, glReadPixels,
// glCheckFramebufferStatus) are recorded with a Sync the caller waits on: a round trip
// through the queue, used at creation time and for diagnostics, never per draw. Host
// functions can be run on the render thread with call() (the swap, the frame readback).
#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>

typedef unsigned int GLenum;

namespace host::gles::gq {
struct Sync {
    std::atomic<int> done{0};
    union { unsigned u; int i; GLenum e; const unsigned char* p; };
};

bool enabled();       // the queue is on (FABLE_GLES_THREAD, default on)
bool direct();        // wrappers call the driver directly: the queue is off or this is the render thread
bool onRenderThread();

// Records: begin() reserves a record (argument struct of `args` bytes followed by `data`
// bytes of payload) and returns the struct's address; commit() publishes it.
void* begin(uint32_t op, size_t args, size_t data);
void commit();
void wait(Sync& s);    // game thread: until the render thread signalled the record
void signal(Sync& s);  // render thread
void call(std::function<void()> fn);  // run on the render thread, in order with the GL calls
void flush();          // wait until everything recorded so far has been replayed

// Frames: Present records a frame boundary; the game thread then waits until at most
// `inFlight` frames are unreplayed.
void frameEnd();
double renderMs();     // render-thread time spent on the last replayed frame (ms)
double waitMs();       // time the game thread spent waiting at the last frame boundary (ms)

// Lifecycle: start() launches the thread, which runs makeCurrent() first (the creating thread
// must have released the context); stop() drains the queue, runs release() there and joins.
bool start(std::function<bool()> makeCurrent, std::function<void()> release);
void stop();

// Object names: glGen* takes them from a pool the render thread refills 64 at a time.
enum GenKind { GEN_buffers, GEN_framebuffers, GEN_queries, GEN_renderbuffers, GEN_samplers, GEN_textures, GEN_vertexArrays, GEN_count };
void genNames(int kind, int n, unsigned* out);
void (*genFn(int kind))(int, unsigned*);  // the driver's glGen* for a kind (gl_queue_gen.inc)

bool loadReal(void* (*getProc)(const char*));  // the driver entry points (gl_queue_gen.inc)
void replay(uint32_t op, const void* rec);     // one record (gl_queue_gen.inc)
size_t texBytes(int width, int height, GLenum format, GLenum type);  // glTexSubImage2D payload
}  // namespace host::gles::gq
