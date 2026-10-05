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

#include <sys/mman.h>
#include <unordered_map>

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
#else
#error "coro_swap: unsupported host architecture"
#endif
extern "C" void coro_swap(void** saveSp, void* newSp);

namespace xb {
namespace {

constexpr uint32_t kSavedEsp = 0x990A8C;  // the other side's guest ESP
constexpr uint32_t kHolder = 0x990A88;    // the coroutine being resumed (stack holder)
constexpr size_t kHostStack = 4u << 20;

struct Fiber {
    uint32_t key = 0, resumeAddr = 0;
    void* sp = nullptr;
    void* stack = nullptr;
    void* landings = nullptr;
};

std::unordered_map<uint32_t, Fiber*> g_fibers;  // GIL held
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
    f->stack = mmap(nullptr, kHostStack, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (f->stack == MAP_FAILED) die("coroutine stack: out of memory");
    mprotect(f->stack, 4096, PROT_NONE);  // guard page
    auto top = reinterpret_cast<uintptr_t>(f->stack) + kHostStack;
    top &= ~uintptr_t{15};
    auto* s = reinterpret_cast<uintptr_t*>(top);
#if defined(__x86_64__)
    *--s = 0;                                          // fiberMain's return slot (never used)
    *--s = reinterpret_cast<uintptr_t>(&fiberMain);    // coro_swap's ret
    for (int i = 0; i < 6; ++i) *--s = 0;              // rbp rbx r12-r15
#else
    s -= 20;                                           // 160-byte frame
    for (int i = 0; i < 20; ++i) s[i] = 0;
    s[11] = reinterpret_cast<uintptr_t>(&fiberMain);   // x30
#endif
    f->sp = s;
    return f;
}

void destroy(Fiber* f) {
    munmap(f->stack, kHostStack);
    delete f;
}

}  // namespace
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
        Fiber*& f = g_fibers[key];
        if (!f || f->resumeAddr != target) {
            // A fresh image returns into its entry (0x4B470 or a coroutine function); one we
            // didn't see yield (a reused holder) starts over the same way.
            if (f) XLOG(2, "coroutine %08X: restarting at 0x%08X (last yield 0x%08X)", key, target, f->resumeAddr);
            if (f) destroy(f);
            f = create(key);
            t_startCtx = c;
        }
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
