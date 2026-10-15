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
    FrameData* a = new FrameData; a->refs = 2; a->initialised = true;
    Frame* f1 = new Frame; f1->data = a;
    Frame* f2 = new Frame; f2->data = a;
    delete f1;
    if (g_dataDeleted != 0 || a->refs != 1) fail = 1;
    delete f2;
    if (g_dataDeleted != 1 || g_uninitialised != 1) fail = 1;
    Frame* f3 = new Frame; delete f3;
    if (g_dataDeleted != 1) fail = 1;
    if (fail) { std::printf("00b2ed60_TEST FAIL\n"); return 1; }
    std::printf("00b2ed60_TEST PASS\n");
    return 0;
}