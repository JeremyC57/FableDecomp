#include <cstdio>
// Host model of CGraphicDataBank::GetGraphicMax*: the largest FrameWidth of the owning info over the frames of a graphic.
struct Info;
struct Frame { Info* info; };
struct Info { unsigned short FrameWidth, FrameHeight, FrameCount; Frame* frames; };
struct Bank {
    Info* first; Info* last;
    unsigned long size() const { return (unsigned long)(last - first); }
    const Info* Get(unsigned long i) const { return i < size() ? &first[i] : 0; }
    unsigned long Max(unsigned long index) const
    {
        const Info* info = Get(index);
        unsigned long m = 0;
        for (unsigned long i = 0; i < info->FrameCount; ++i) { unsigned long v = info->frames[i].info->FrameWidth; if (m <= v) m = v; }
        return m;
    }
};
int main()
{
    int fail = 0;
    Info infos[3];
    Frame f0[2], f1[3];
    infos[0].FrameWidth = 10; infos[0].FrameHeight = 12; infos[0].FrameCount = 2; infos[0].frames = f0;
    infos[1].FrameWidth = 30; infos[1].FrameHeight = 5; infos[1].FrameCount = 3; infos[1].frames = f1;
    infos[2].FrameWidth = 50; infos[2].FrameHeight = 60; infos[2].FrameCount = 0; infos[2].frames = 0;
    f0[0].info = &infos[0]; f0[1].info = &infos[1];
    f1[0].info = &infos[1]; f1[1].info = &infos[0]; f1[2].info = &infos[2];
    Bank b; b.first = infos; b.last = infos + 3;
    unsigned long e0 = infos[0].FrameWidth > infos[1].FrameWidth ? infos[0].FrameWidth : infos[1].FrameWidth;
    unsigned long e1 = e0 > infos[2].FrameWidth ? e0 : infos[2].FrameWidth;
    if (b.Max(0) != e0 || b.Max(1) != e1 || b.Max(2) != 0) fail = 1;
    if (fail) { std::printf("00a014d0_TEST FAIL\n"); return 1; }
    std::printf("00a014d0_TEST PASS\n");
    return 0;
}
