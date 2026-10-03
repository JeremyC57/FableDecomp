// Fable's stack-copying coroutines, run on host fibers.
//
// The game switches between a "main" context and coroutines (world loading, among
// others) with a hand-written routine at 0x9D8650:
//     push ebx; push ebp; push esi; push edi; push fs:[0]
//     xchg esp, [0x13CA898]                     (via eax)
//     pop fs:[0]; pop edi; pop esi; pop ebp; pop ebx; ret
// The `ret` lands in the other context, which a lifted C function cannot do. The lifter
// replaces that function with host_coswitch (--hook 0x9D8650=host_coswitch), which does
// the same guest stack and register work and moves between host fibers: one per
// coroutine object (the global at 0x13CA894 when switching from main), and the thread's
// original fiber for main. A new coroutine is any image whose return address is not
// where that object's fiber last suspended. The coroutine's guest stack is copied in and out by the game
// itself, so its guest addresses are the same every time it resumes.
#include "host.hpp"

#include <string>
#include <unordered_map>

using namespace host;

namespace {

constexpr uint32_t kSavedEsp = 0x13CA898;   // the other context's esp
constexpr uint32_t kCurrent = 0x13CA894;    // the coroutine object being resumed
constexpr SIZE_T kFiberStack = 256u << 20;  // reserve; lifted code recurses on the host stack

struct ThreadFibers {
    void* main = nullptr;
    bool onMain = true;
    struct Co {
        void* fiber = nullptr;
        uint32_t suspendedAt = 0;  // return address of its last switch (where it resumes)
    };
    std::unordered_map<uint32_t, Co> coroutines;  // coroutine object -> fiber
    uint32_t current = 0;                         // coroutine running on a fiber (0: main)
    uint32_t handoffEsp = 0;                         // esp of the context being resumed
};
thread_local ThreadFibers t_fibers;
std::string hexs(uint32_t v) {
    char b[12];
    std::snprintf(b, sizeof b, "%X", v);
    return b;
}

// The second half of 0x9D8650, run by the context being resumed: pop fs:[0] and the
// callee-saved registers from the handed-over stack, then `ret`.
void resumeHere(Ctx* c, bool fresh) {
    uint32_t esp = t_fibers.handoffEsp;
    wr32(currentThread()->teb, rd32(esp));
    c->edi = rd32(esp + 4);
    c->esi = rd32(esp + 8);
    c->ebp = rd32(esp + 12);
    c->ebx = rd32(esp + 16);
    esp += 20;
    if (fresh) {
        // `ret` into the coroutine's entry; the next slot is the entry's own return
        // address and its arguments follow, as the game laid them out.
        const uint32_t entry = rd32(esp);
        c->esp = esp + 4;
        recomp_dispatch(c, entry);
        die("guest coroutine returned from its entry point");
    }
    c->esp = esp + 4;  // ret: back into the lifted caller of 0x9D8650 on this fiber
}

void WINAPI coroutineMain(void*) {
    recomp_landing_chain_set(nullptr);  // C++ landing pads are per fiber
    resumeHere(cur(), true);
}

}  // namespace

extern "C" void host_coswitch(Ctx* c) {
    ThreadFibers& tf = t_fibers;
    if (!tf.main) tf.main = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(nullptr);

    // first half: save this context on its guest stack and swap the saved esp
    uint32_t esp = c->esp;
    esp -= 4, wr32(esp, c->ebx);
    esp -= 4, wr32(esp, c->ebp);
    esp -= 4, wr32(esp, c->esi);
    esp -= 4, wr32(esp, c->edi);
    esp -= 4, wr32(esp, rd32(currentThread()->teb));
    const uint32_t target = rd32(kSavedEsp);
    wr32(kSavedEsp, esp);
    tf.handoffEsp = target;

    void* to = nullptr;
    if (tf.onMain) {
        // A coroutine resumes on its fiber only where that fiber suspended; any other
        // return address is a new coroutine image (its entry point, e.g. 0x9D8690).
        const uint32_t co = rd32(kCurrent), resumeAt = rd32(target + 20);
        ThreadFibers::Co& f = tf.coroutines[co];
        if (!f.fiber || f.suspendedAt != resumeAt) {
            if (f.fiber) DeleteFiber(f.fiber);
            f.fiber = CreateFiberEx(0x10000, kFiberStack, FIBER_FLAG_FLOAT_SWITCH, coroutineMain, nullptr);
            if (!f.fiber) die("CreateFiberEx failed (%lu)", GetLastError());
            HLOG(1, "coroutine 0x%08X started at 0x%08X", co, resumeAt);
        }
        to = f.fiber;
        tf.current = co;
        tf.onMain = false;
    } else {
        tf.coroutines[tf.current].suspendedAt = rd32(c->esp);  // our return address
        static uint32_t yields = 0;
        if (g_logLevel >= 2 && (++yields % 2000) == 1) {  // where the coroutine is, now and then
            std::string chain;
            for (uint32_t a = c->esp, n = 0; a < c->esp + 0x1000 && n < 12; a += 4)
                if (const uint32_t v = rd32(a); v >= 0x401000 && v < 0x1200000) { chain += " " + hexs(v); ++n; }
            HLOG(2, "coroutine 0x%08X yield #%u, stack:%s", tf.current, yields, chain.c_str());
        }
        to = tf.main;
        tf.onMain = true;
    }
    void* const landings = recomp_landing_chain_get();
    SwitchToFiber(to);
    // resumed: someone switched back to this context
    recomp_landing_chain_set(landings);
    resumeHere(c, false);
}
