// FABLE_PROFILE=<seconds>: a sampling profiler for the whole process (all threads). Every
// millisecond of CPU time SIGPROF records the interrupted program counter; after <seconds>
// the histogram goes to profile.txt as offsets into the executable (resolve them with
// tools: nm -n on the binary). Debugging only.
#include "xhost.hpp"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <map>
#include <signal.h>
#include <sys/time.h>
#include <thread>
#include <ucontext.h>

namespace xb {
namespace {
constexpr size_t kMax = 1 << 22;
uintptr_t* g_samples;
std::atomic<size_t> g_count{0};

void onProf(int, siginfo_t*, void* uc) {
    const auto* c = static_cast<ucontext_t*>(uc);
#if defined(__x86_64__)
    const uintptr_t pc = static_cast<uintptr_t>(c->uc_mcontext.gregs[REG_RIP]);
#elif defined(__aarch64__)
    const uintptr_t pc = static_cast<uintptr_t>(c->uc_mcontext.pc);
#else
    const uintptr_t pc = 0;
#endif
    const size_t i = g_count.fetch_add(1, std::memory_order_relaxed);
    if (i < kMax) g_samples[i] = pc;
}
}  // namespace

void profilerStart() {
    const char* e = std::getenv("FABLE_PROFILE");
    if (!e) return;
    const double secs = std::atof(e);
    g_samples = new uintptr_t[kMax];
    struct sigaction sa{};
    sa.sa_sigaction = onProf;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGPROF, &sa, nullptr);
    itimerval tv{{0, 1000}, {0, 1000}};
    setitimer(ITIMER_PROF, &tv, nullptr);
    std::thread([secs] {
        std::this_thread::sleep_for(std::chrono::duration<double>(secs));
        itimerval off{};
        setitimer(ITIMER_PROF, &off, nullptr);
        Dl_info self{};
        dladdr(reinterpret_cast<void*>(&profilerStart), &self);
        const uintptr_t base = reinterpret_cast<uintptr_t>(self.dli_fbase);
        std::map<uintptr_t, size_t> hist;  // offset (or absolute address outside the executable)
        const size_t n = std::min(g_count.load(), kMax);
        for (size_t i = 0; i < n; ++i) {
            Dl_info d{};
            const uintptr_t pc = g_samples[i];
            if (dladdr(reinterpret_cast<void*>(pc), &d) && d.dli_fbase == self.dli_fbase) ++hist[pc - base];
            else ++hist[pc | (uintptr_t{1} << 63)];  // shared libraries (driver, libc): marked
        }
        if (FILE* f = std::fopen("profile.txt", "w")) {
            std::fprintf(f, "# %zu samples\n", n);
            for (auto& [k, v] : hist) std::fprintf(f, "%llx %zu\n", static_cast<unsigned long long>(k), v);
            std::fclose(f);
        }
        std::printf("profile: %zu samples written to profile.txt\n", n);
    }).detach();
}

}  // namespace xb
