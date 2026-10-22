// Behaviour test for CEnginePrimitiveManagerSpriteTrails::RenderSubPrimitive @ 0x00BCB800 -- texture bind + trail render when the frame resolves
#include <cstdio>
struct D3DTexture { int Id; };
struct Texture { D3DTexture* D3D; };
struct FrameData { long ReferenceCount; Texture* PTexture; };
struct Frame {
    FrameData* Data;
    Frame() : Data(0) { }
    ~Frame() { if (Data) --Data->ReferenceCount; }
};
struct Core {
    D3DTexture* Bound[4];
    long ActiveTextureCount;
    int Captures;
    void SetTexture(long stage, D3DTexture* t) { Bound[stage] = t; }
};
struct Bank {
    FrameData* Frames[4];
    bool GetFrame(Frame* out, long bankIndex, long frameIndex)
    {
        if (bankIndex < 0 || bankIndex >= 4 || frameIndex != 0 || !Frames[bankIndex]) return false;
        out->Data = Frames[bankIndex];
        ++out->Data->ReferenceCount;
        return true;
    }
};
struct Trail { long BankIndex; long FrameIndex; };
static Core g_core;
static Bank g_bank;
static int g_rendered = 0;
struct Capture { Capture() { ++g_core.Captures; } ~Capture() { --g_core.Captures; } };
static void RenderSub(Trail* trail)
{
    Capture capture;
    Frame frame;
    if (g_bank.GetFrame(&frame, trail->BankIndex, trail->FrameIndex)) {
        Texture* texture = frame.Data->PTexture;
        g_core.SetTexture(0, texture ? texture->D3D : 0);
        if (g_core.ActiveTextureCount < 1)
            g_core.ActiveTextureCount = 1;
        ++g_rendered;
    }
}
int main()
{
    bool ok = true;
    D3DTexture d3d = { 7 };
    Texture tex = { &d3d };
    FrameData fd = { 1, &tex };
    FrameData empty = { 1, 0 };
    g_bank.Frames[0] = &fd; g_bank.Frames[1] = &empty; g_bank.Frames[2] = 0; g_bank.Frames[3] = 0;
    Trail a = { 0, 0 };
    RenderSub(&a);
    ok = ok && g_rendered == 1 && g_core.Bound[0] == &d3d && g_core.ActiveTextureCount == 1 && fd.ReferenceCount == 1 && g_core.Captures == 0;
    g_core.ActiveTextureCount = 3;
    Trail b = { 1, 0 };
    RenderSub(&b);
    ok = ok && g_rendered == 2 && g_core.Bound[0] == 0 && g_core.ActiveTextureCount == 3 && empty.ReferenceCount == 1;
    Trail c = { 2, 0 };
    g_core.Bound[0] = &d3d;
    RenderSub(&c);
    ok = ok && g_rendered == 2 && g_core.Bound[0] == &d3d && g_core.Captures == 0;
    if (ok) std::printf("00bcb800_TEST PASS\n"); else std::printf("00bcb800_TEST FAIL\n");
    return ok ? 0 : 1;
}
