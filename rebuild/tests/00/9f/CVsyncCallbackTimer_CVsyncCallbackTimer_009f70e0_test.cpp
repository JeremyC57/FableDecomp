// Behaviour test for 0x009F70E0 CVsyncCallbackTimer::CVsyncCallbackTimer (empty query lists, zeroed shared stamps, frame counters).
#include <cstdio>
#include <cstdlib>

struct Node { Node* next; Node* prev; char query[16]; };
struct Stamp { double ts; double prev; long frames; long readers; };

static Stamp g_cpu[2];
static double g_gpuTs, g_gpuPrev;
static long g_gpuFrames, g_current;

static Node* NewHead(unsigned int n)
{
    Node* h = n != 0 ? (Node*)std::malloc(n * sizeof(Node)) : 0;
    h->next = h; h->prev = h;
    return h;
}

struct Timer {
    void* device; Node* freeQueries; Node* pending; long pendingFrame; long finishedFrame;
    Timer()
    {
        freeQueries = NewHead(1);
        pending = NewHead(1);
        g_gpuFrames = 0; g_current = 0;
        g_cpu[0].ts = 0.0; g_cpu[0].prev = 0.0; g_cpu[1].ts = 0.0; g_cpu[1].prev = 0.0;
        g_gpuTs = 0.0; g_gpuPrev = 0.0;
        g_cpu[0].frames = 0; g_cpu[0].readers = 0; g_cpu[1].frames = 0; g_cpu[1].readers = 0;
        pendingFrame = 1; finishedFrame = 0;
    }
};

static int Zero(double d) { return d < 1e-9 && d > -1e-9; }

int main()
{
    int ok = 1;
    g_gpuFrames = 7; g_current = 1; g_cpu[1].ts = 3.5; g_cpu[0].readers = 2; g_gpuPrev = 9.0;
    Timer t;
    if (t.freeQueries->next != t.freeQueries || t.freeQueries->prev != t.freeQueries) ok = 0;
    if (t.pending->next != t.pending || t.pending == t.freeQueries) ok = 0;
    if (g_gpuFrames != 0 || g_current != 0 || !Zero(g_cpu[1].ts) || g_cpu[0].readers != 0 || !Zero(g_gpuPrev)) ok = 0;
    if (t.pendingFrame != 1 || t.finishedFrame != 0) ok = 0;
    std::free(t.freeQueries); std::free(t.pending);
    if (!ok) { std::printf("009f70e0_TEST FAIL\n"); return 1; }
    std::printf("009f70e0_TEST PASS\n");
    return 0;
}