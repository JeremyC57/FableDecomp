// Behaviour test for 0x00A0A940: Init sets up the base and font, stores the device, resets flags/counters, reads caps and the back buffer pixel format.
#include <cstdio>
struct Caps { int filled; };
struct Device { int capsCalls; void GetDeviceCaps(Caps* c) { ++capsCalls; c->filled = 1; } };
struct Surface { int fmt; int GetPixelFormat() { return fmt; } };
struct Display { Surface BackBufferSurface; };
struct Core { int baseInits; Device* baseDevice; int fontInits; Display* DisplayManager; int ScreenPixelFormat; bool Rendering, TexturesEnabled, XBox, FixedFunction;
  Caps caps; int TextureStagesInUse, HighestMipmapToRender; Device* PD3DDevice; void* PSelectedIndexBuffer;
  void Init(Device* d) { ++baseInits; baseDevice = d; PD3DDevice = d; ++fontInits; TexturesEnabled = true; FixedFunction = true; HighestMipmapToRender = 0; PSelectedIndexBuffer = 0;
    PD3DDevice->GetDeviceCaps(&caps); TextureStagesInUse = 0; Rendering = false; ScreenPixelFormat = DisplayManager->BackBufferSurface.GetPixelFormat(); XBox = false; } };
int main()
{
    Device dev = {0}; Display disp = {{22}};
    Core c; c.baseInits = 0; c.fontInits = 0; c.DisplayManager = &disp; c.Rendering = true; c.XBox = true; c.TextureStagesInUse = 3; c.HighestMipmapToRender = 2; c.PSelectedIndexBuffer = &c; c.caps.filled = 0;
    c.Init(&dev);
    bool ok = c.baseInits == 1 && c.baseDevice == &dev && c.PD3DDevice == &dev && c.fontInits == 1 && c.TexturesEnabled && c.FixedFunction && !c.Rendering && !c.XBox
           && c.TextureStagesInUse == 0 && c.HighestMipmapToRender == 0 && c.PSelectedIndexBuffer == 0 && c.caps.filled == 1 && dev.capsCalls == 1 && c.ScreenPixelFormat == 22;
    if (!ok) { std::printf("00a0a940_TEST FAIL\n"); return 1; }
    std::printf("00a0a940_TEST PASS\n");
    return 0;
}