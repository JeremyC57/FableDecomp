#include <cstdio>
struct Pool {
    int depth; int lastWait; bool allocOk; long setHandle; void* setRes;
    bool BeginCritical(bool wait) { lastWait = wait ? 1 : 0; if (depth < 0) return false; ++depth; return true; }
    void EndCritical() { --depth; }
    bool Allocate(long* h, unsigned int size, unsigned int flags) { if (!allocOk) return false; *h = (long)size + 1; return true; }
    void InternalSetResource(long h, void* r) { setHandle = h; setRes = r; }
    bool AllocateResource(long* handle, void* owner, unsigned int size, unsigned int flags)
    {
        if (!BeginCritical((flags & 1) != 0)) return false;
        if (Allocate(handle, size, flags)) { InternalSetResource(*handle, owner); EndCritical(); return true; }
        EndCritical();
        return false;
    }
};
int main()
{
    Pool p; p.depth = 0; p.lastWait = -1; p.allocOk = true; p.setHandle = 0; p.setRes = 0;
    long h = 0; int owner;
    bool fail = false;
    if (!p.AllocateResource(&h, &owner, 9, 3) || h != 10 || p.setHandle != 10 || p.setRes != &owner || p.depth != 0 || p.lastWait != 1) fail = true;
    p.allocOk = false;
    if (p.AllocateResource(&h, &owner, 9, 2) || p.depth != 0 || p.lastWait != 0) fail = true;
    p.depth = -5;
    if (p.AllocateResource(&h, &owner, 9, 2) || p.depth != -5) fail = true;
    if (fail) { std::printf("00a62e40_TEST FAIL\n"); return 1; }
    std::printf("00a62e40_TEST PASS\n");
    return 0;
}
