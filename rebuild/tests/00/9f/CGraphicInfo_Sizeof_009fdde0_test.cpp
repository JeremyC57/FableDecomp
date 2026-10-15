#include <cstdio>
// Host model of CGraphicInfo::Sizeof: 0x2c + per frame 0x10 + (loaded data: 0x44 + graphic frame (4 + data size) + texture size).
struct FrameData { unsigned long size; unsigned long Sizeof() const { return size; } };
struct GraphicFrame { FrameData* fd; unsigned long Sizeof() const { return fd ? fd->Sizeof() + 4 : 4; } };
struct Texture { bool set; unsigned long size; unsigned long Sizeof() const { return size; } };
struct Data { GraphicFrame frame; Texture tex; unsigned long Sizeof() const { unsigned long s = 0x44 + frame.Sizeof(); if (tex.set) s += tex.Sizeof(); return s; } };
struct Frame { Data* data; unsigned long Sizeof() const { unsigned long s = 0x10; if (data) s += data->Sizeof(); return s; } };
struct Info {
    unsigned short FrameCount; Frame* frames;
    unsigned long Sizeof() const { unsigned long s = 0x2c; for (unsigned long i = 0; i < FrameCount; ++i) s += frames[i].Sizeof(); return s; }
};
int main()
{
    int fail = 0;
    FrameData fd = { 100 };
    Data d1; d1.frame.fd = &fd; d1.tex.set = true; d1.tex.size = 1000;
    Data d2; d2.frame.fd = 0; d2.tex.set = false; d2.tex.size = 7;
    Frame frames[3]; frames[0].data = &d1; frames[1].data = 0; frames[2].data = &d2;
    Info info; info.FrameCount = 3; info.frames = frames;
    unsigned long expect = 0x2c + (0x10 + 0x44 + 104 + 1000) + 0x10 + (0x10 + 0x44 + 4);
    if (info.Sizeof() != expect) fail = 1;
    info.FrameCount = 0;
    if (info.Sizeof() != 0x2c) fail = 1;
    if (fail) { std::printf("009fdde0_TEST FAIL\n"); return 1; }
    std::printf("009fdde0_TEST PASS\n");
    return 0;
}