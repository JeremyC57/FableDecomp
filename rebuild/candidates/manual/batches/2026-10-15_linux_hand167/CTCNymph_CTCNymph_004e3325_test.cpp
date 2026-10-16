#include <cstdio>
// Behaviour model of CTCNymph::CTCNymph (0x004E3325): null def and pointers, an empty spline with tension 0.5.
struct Spline { void* data; long size; long cap; float tension; bool connected; Spline() : data(0), size(0), cap(0), tension(0.5f), connected(false) { } };
struct Nymph { void* def; Spline spline; void* effect; void* oldEffect; void* target; Nymph() : def(0), effect(0), oldEffect(0), target(0) { } };
int main()
{
    Nymph n;
    if (n.def || n.spline.data || n.spline.size || n.spline.cap || n.spline.tension != 0.5f || n.spline.connected || n.effect || n.oldEffect || n.target) { std::printf("004e3325_TEST FAIL\n"); return 1; }
    std::printf("004e3325_TEST PASS\n");
    return 0;
}
