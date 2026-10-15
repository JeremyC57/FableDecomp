#include <cstdio>
// Host model of the shared CGraphicFrameData reference: frames share a counted data block; the last reference deletes it.
static int g_dataDeleted = 0, g_uninitialised = 0;
struct FrameData { unsigned long refs; bool initialised; };
static void DeleteData(FrameData* d) { if (d->initialised) ++g_uninitialised; ++g_dataDeleted; delete d; }
struct Frame {
    FrameData* data;
    Frame() : data(0) {}
    Frame& operator=(const Frame& o)
    {
        FrameData* d = data;
        if (d && --d->refs == 0) DeleteData(d);
        data = o.data;
        if (data) ++data->refs;
        return *this;
    }
    ~Frame() { FrameData* d = data; if (d) { if (--d->refs == 0) DeleteData(d); data = 0; } }
};
int main()
{
    int fail = 0;
    FrameData* a = new FrameData; a->refs = 1; a->initialised = true;
    FrameData* b = new FrameData; b->refs = 1; b->initialised = false;
    Frame fa; fa.data = a;
    Frame fb; fb.data = b;
    fa = fb;                                   // a released (deleted), b shared
    if (g_dataDeleted != 1 || g_uninitialised != 1 || fa.data != b || b->refs != 2) fail = 1;
    fb.data = 0; b->refs = 1;
    fa = fb;                                   // b released, null adopted
    if (g_dataDeleted != 2 || fa.data != 0) fail = 1;
    if (fail) { std::printf("0041c0e0_TEST FAIL\n"); return 1; }
    std::printf("0041c0e0_TEST PASS\n");
    return 0;
}
