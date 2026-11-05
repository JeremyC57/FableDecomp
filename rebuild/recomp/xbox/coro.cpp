// Fable's stack-copying coroutines (script/AI micro-threads) on host stacks.
//
// The game runs coroutines on one fixed guest stack area: resuming (0x4B490) copies a saved
// stack image into it and calls the switch routine 0x4B440, which pushes the callee-saved
// registers and the SEH head, exchanges ESP with [0x990A8C], pops the other side's and
// returns on the other stack. Yielding calls the same routine from inside the coroutine.
// A new coroutine's image returns into its entry (the trampoline 0x4B470, or a function).
//
// A static recompile can't "return" onto another guest stack: the host call stack has to
// switch too. The hook does the guest-side exchange exactly, then switches host stacks:
// each coroutine (keyed by its stack holder, [0x990A88]) gets its own host stack, entered
// resumed when the target is the address it last yielded from, entered fresh otherwise.
#include "xhost.hpp"

#include <algorithm>
#include <cstdlib>
#include <unordered_map>
#if !defined(__3DS__)
#include <sys/mman.h>
#endif

extern "C" void* recomp_landing_chain_get(void);
extern "C" void recomp_landing_chain_set(void* chain);

// void coro_swap(void** saveSp, void* newSp): saves the callee-saved registers on the current
// stack, stores its pointer, and resumes the stack at newSp (same layout).
#if defined(__x86_64__)
asm(R"(
    .text
    .globl coro_swap
    .type coro_swap, @function
coro_swap:
    pushq %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq %rsp, (%rdi)
    movq %rsi, %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp
    ret
    .size coro_swap, .-coro_swap
)");
#elif defined(__aarch64__)
asm(R"(
    .text
    .globl coro_swap
    .type coro_swap, %function
coro_swap:
    sub sp, sp, #160
    stp x19, x20, [sp, #0]
    stp x21, x22, [sp, #16]
    stp x23, x24, [sp, #32]
    stp x25, x26, [sp, #48]
    stp x27, x28, [sp, #64]
    stp x29, x30, [sp, #80]
    stp d8, d9, [sp, #96]
    stp d10, d11, [sp, #112]
    stp d12, d13, [sp, #128]
    stp d14, d15, [sp, #144]
    mov x2, sp
    str x2, [x0]
    mov sp, x1
    ldp x19, x20, [sp, #0]
    ldp x21, x22, [sp, #16]
    ldp x23, x24, [sp, #32]
    ldp x25, x26, [sp, #48]
    ldp x27, x28, [sp, #64]
    ldp x29, x30, [sp, #80]
    ldp d8, d9, [sp, #96]
    ldp d10, d11, [sp, #112]
    ldp d12, d13, [sp, #128]
    ldp d14, d15, [sp, #144]
    add sp, sp, #160
    ret
    .size coro_swap, .-coro_swap
)");
#elif defined(__arm__)
// ARMv6K (New 3DS): r4-r12 (r12 pads the frame to 8 bytes), lr, d8-d15.
asm(R"(
    .text
    .arm
    .globl coro_swap
    .type coro_swap, %function
coro_swap:
    push {r4-r12, lr}
    vpush {d8-d15}
    str sp, [r0]
    mov sp, r1
    vpop {d8-d15}
    pop {r4-r12, pc}
    .size coro_swap, .-coro_swap
)");
#else
#error "coro_swap: unsupported host architecture"
#endif
extern "C" void coro_swap(void** saveSp, void* newSp);

namespace xb {
namespace {

constexpr uint32_t kSavedEsp = 0x990A8C;  // the other side's guest ESP
constexpr uint32_t kHolder = 0x990A88;    // the coroutine being resumed (stack holder)
#if defined(__3DS__)
// Host stacks are real memory on the 3DS (no lazy commit): FABLE_PERF_STATS reports the deepest use.
constexpr size_t kHostStack = 64u << 10;  // deepest use seen on desktop: 6 KB (64 coroutines)
constexpr uint32_t kStackFill = 0xC0DEC0DEu;
#else
constexpr size_t kHostStack = 4u << 20;
#endif

struct Fiber {
    uint32_t key = 0, resumeAddr = 0;
    void* sp = nullptr;
    void* stack = nullptr;
    void* landings = nullptr;
};

std::unordered_map<uint32_t, Fiber*> g_fibers;  // GIL held; changes also under g_fibersLock (coroStats)
std::mutex g_fibersLock;
thread_local Fiber* t_cur;                      // the running coroutine (null: the main stack)
thread_local void* t_mainSp;
thread_local void* t_mainLandings;
thread_local Ctx* t_startCtx;

[[noreturn]] void fiberMain() {
    Ctx* c = t_startCtx;
    const uint32_t target = rd32(c->esp);
    c->esp += 4;
    recomp_dispatch(c, target);  // the entry trampoline loops forever, yielding
    die("coroutine entry 0x%08X returned", target);
}

Fiber* create(uint32_t key) {
    auto* f = new Fiber;
    f->key = key;
#if defined(__3DS__)
    f->stack = aligned_alloc(16, kHostStack);
    if (!f->stack) die("coroutine stack: out of memory (%zu coroutines)", g_fibers.size());
    for (size_t i = 0; i < kHostStack / 4; ++i) static_cast<uint32_t*>(f->stack)[i] = kStackFill;  // high-water mark
#else
    f->stack = mmap(nullptr, kHostStack, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (f->stack == MAP_FAILED) die("coroutine stack: out of memory");
    mprotect(f->stack, 4096, PROT_NONE);  // guard page
#endif
    auto top = reinterpret_cast<uintptr_t>(f->stack) + kHostStack;
    top &= ~uintptr_t{15};
    auto* s = reinterpret_cast<uintptr_t*>(top);
#if defined(__x86_64__)
    *--s = 0;                                          // fiberMain's return slot (never used)
    *--s = reinterpret_cast<uintptr_t>(&fiberMain);    // coro_swap's ret
    for (int i = 0; i < 6; ++i) *--s = 0;              // rbp rbx r12-r15
#elif defined(__aarch64__)
    s -= 20;                                           // 160-byte frame
    for (int i = 0; i < 20; ++i) s[i] = 0;
    s[11] = reinterpret_cast<uintptr_t>(&fiberMain);   // x30
#else
    s -= 26;                                           // d8-d15 (16 words), r4-r12, lr
    for (int i = 0; i < 26; ++i) s[i] = 0;
    s[25] = reinterpret_cast<uintptr_t>(&fiberMain);   // popped into pc
#endif
    f->sp = s;
    return f;
}

size_t g_deepest = 0;  // bytes, any coroutine so far

// Deepest stack use of a coroutine: untouched words keep the fill (3DS) or stay zero (mmap).
size_t stackUsed(const Fiber* f) {
    const auto* w = static_cast<const uint32_t*>(f->stack);
#if defined(__3DS__)
    const uint32_t untouched = kStackFill;
    size_t i = 0;
#else
    const uint32_t untouched = 0;
    size_t i = 4096 / 4;  // past the guard page
#endif
    while (i < kHostStack / 4 && w[i] == untouched) ++i;
    return kHostStack - i * 4;
}

void destroy(Fiber* f) {
    g_deepest = std::max(g_deepest, stackUsed(f));
#if defined(__3DS__)
    free(f->stack);
#else
    munmap(f->stack, kHostStack);
#endif
    delete f;
}

}  // namespace

// FABLE_PERF_STATS: live coroutines and the deepest host stack any has used.
void coroStats(size_t& count, size_t& deepest, size_t& stackSize) {
    std::lock_guard<std::mutex> l(g_fibersLock);
    for (const auto& [k, f] : g_fibers) g_deepest = std::max(g_deepest, stackUsed(f));
    count = g_fibers.size();
    deepest = g_deepest;
    stackSize = kHostStack;
}
}  // namespace xb

using namespace xb;

extern "C" void hle_CoroSwitch(Ctx* c) {
    const uint32_t retHere = rd32(c->esp);
    // Guest side, as 0x4B440 does it.
    uint32_t esp = c->esp;
    const uint32_t regs[5] = {c->ebx, c->ebp, c->esi, c->edi, rd32(c->fs_base)};
    for (uint32_t r : regs) {
        esp -= 4;
        wr32(esp, r);
    }
    const uint32_t other = rd32(kSavedEsp);
    wr32(kSavedEsp, esp);
    esp = other;
    wr32(c->fs_base, rd32(esp));
    c->edi = rd32(esp + 4);
    c->esi = rd32(esp + 8);
    c->ebp = rd32(esp + 12);
    c->ebx = rd32(esp + 16);
    esp += 20;
    c->esp = esp;
    const uint32_t target = rd32(esp);

    // Host side.
    if (!t_cur) {  // main -> coroutine
        const uint32_t key = rd32(kHolder);
        std::unique_lock<std::mutex> fl(g_fibersLock);
        Fiber*& f = g_fibers[key];
        if (!f || f->resumeAddr != target) {
            // A fresh image returns into its entry (0x4B470 or a coroutine function); one we
            // didn't see yield (a reused holder) starts over the same way.
            if (f) XLOG(1, "coroutine %08X: restarting at 0x%08X (last yield 0x%08X)", key, target, f->resumeAddr);
            if (f) destroy(f);
            f = create(key);
            t_startCtx = c;
        }
        fl.unlock();
        t_mainLandings = recomp_landing_chain_get();
        recomp_landing_chain_set(f->landings);
        t_cur = f;
        coro_swap(&t_mainSp, f->sp);
        // back on the main stack (the coroutine yielded)
    } else {  // coroutine -> main
        Fiber* f = t_cur;
        f->resumeAddr = retHere;
        f->landings = recomp_landing_chain_get();
        recomp_landing_chain_set(t_mainLandings);
        t_cur = nullptr;
        coro_swap(&f->sp, t_mainSp);
        // resumed
    }
    c->esp += 4;  // the other side left ESP at our return address
}
