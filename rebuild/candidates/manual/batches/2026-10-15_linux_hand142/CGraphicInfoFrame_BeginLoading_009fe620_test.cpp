#include <cstdio>
// Host model of CGraphicInfoFrame::BeginLoading: flat-animated graphics load through frame 0; the frame data is created
// on demand and receives the request.
struct Info;
struct Data { int loads; unsigned long a, b; Data() : loads(0), a(0), b(0) {} void BeginLoading(unsigned long x, unsigned long y) { ++loads; a = x; b = y; } };
struct Frame {
    unsigned short FrameNum; Data* data; Info* info;
    void BeginLoading(unsigned long a, unsigned long b);
};
struct Info { bool flat; Frame* frames; };
void Frame::BeginLoading(unsigned long a, unsigned long b)
{
    Frame* f = this;
    while (f->FrameNum > 0 && f->info->flat) f = f->info->frames;
    if (!f->data) f->data = new Data;
    f->data->BeginLoading(a, b);
}
int main()
{
    int fail = 0;
    Info info; Frame frames[3];
    info.frames = frames; info.flat = true;
    for (int i = 0; i < 3; ++i) { frames[i].FrameNum = (unsigned short)i; frames[i].data = 0; frames[i].info = &info; }
    frames[2].BeginLoading(1, 2);
    if (!frames[0].data || frames[2].data || frames[0].data->loads != 1 || frames[0].data->b != 2) fail = 1;
    info.flat = false;
    frames[1].BeginLoading(3, 4);
    if (!frames[1].data || frames[1].data->a != 3 || frames[0].data->loads != 1) fail = 1;
    frames[0].BeginLoading(5, 6);
    if (frames[0].data->loads != 2) fail = 1;
    delete frames[0].data; delete frames[1].data;
    if (fail) { std::printf("009fe620_TEST FAIL\n"); return 1; }
    std::printf("009fe620_TEST PASS\n");
    return 0;
}
