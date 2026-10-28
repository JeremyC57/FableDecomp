// Behaviour test for 0x00B25950 CEngine::RenderFrame(const CEngineInterpolationInfo&)
#include <cstdio>
#include <cmath>

static int g_starts = 0, g_ends = 0, g_layers = 0, g_freed = 0, g_restores = 0;
static unsigned long g_seenFrame = 0, g_seenFlags = 0;
static float g_seenInterpolation = 0;
struct Params { float FrameInterpolation; unsigned long FrameID, SceneFilterFlags; bool OffWorld; };
struct Component {
    unsigned long Filter;
    unsigned long PeekSceneFilterFlags() const { return Filter; }
    void FrameStart(const Params& p) { ++g_starts; g_seenFrame = p.FrameID; g_seenFlags = p.SceneFilterFlags; g_seenInterpolation = p.FrameInterpolation; }
    void FrameEnd(const Params&) { ++g_ends; }
};
struct Layer { void Render(const Params&) { ++g_layers; } };
struct StateManager {
    void* Stack[16]; long StackSize; unsigned long Mask; char Dummy;
    void Push() { Stack[StackSize] = &Dummy; StackSize++; Mask <<= 1; }
    void RestoreCaptureBlock() { ++g_restores; --StackSize; Mask >>= 1; }
};
static StateManager g_manager;
struct Engine {
    long CurrentRenderFrame; double NonInterpolatedWorldTime, InterpolatedWorldTime; unsigned long WorldFramesPerSecond;
    unsigned long SceneFilterFlags; bool ProcessThisFrame;
    Component Components[3]; Layer Layers[2];
    void RenderFrame(float wfInterpolate)                                 // the row's body
    {
        g_manager.Push();
        InterpolatedWorldTime = NonInterpolatedWorldTime + wfInterpolate / WorldFramesPerSecond;
        Params params;
        params.FrameInterpolation = wfInterpolate;
        params.FrameID = CurrentRenderFrame;
        params.SceneFilterFlags = SceneFilterFlags;
        params.OffWorld = ProcessThisFrame;
        for (int i = 0; i < 3; ++i) if (Components[i].PeekSceneFilterFlags() & SceneFilterFlags) Components[i].FrameStart(params);
        for (int i = 0; i < 2; ++i) Layers[i].Render(params);
        for (int i = 0; i < 3; ++i) if (Components[i].PeekSceneFilterFlags() & SceneFilterFlags) Components[i].FrameEnd(params);
        ++g_freed;                                                    // FrameMemoryPool.FreeAll()
        ++CurrentRenderFrame;
        g_manager.RestoreCaptureBlock();
    }
};

int main()
{
    int ok = 1;
    Engine e;
    e.CurrentRenderFrame = 41; e.NonInterpolatedWorldTime = 10.0; e.WorldFramesPerSecond = 10; e.SceneFilterFlags = 0x5;
    e.ProcessThisFrame = true;
    e.Components[0].Filter = 0x1; e.Components[1].Filter = 0x2; e.Components[2].Filter = 0x4;
    g_manager.StackSize = 0; g_manager.Mask = 1;
    e.RenderFrame(0.5f);
    ok &= std::fabs(e.InterpolatedWorldTime - 10.05) < 1e-6;
    ok &= g_starts == 2 && g_ends == 2 && g_layers == 2 && g_freed == 1 && g_restores == 1;
    ok &= g_seenFrame == 41 && g_seenFlags == 0x5 && g_seenInterpolation == 0.5f && e.CurrentRenderFrame == 42;
    ok &= g_manager.StackSize == 0 && g_manager.Mask == 1;
    if (ok) std::printf("00b25950_TEST PASS\n"); else std::printf("00b25950_TEST FAIL\n");
    return ok ? 0 : 1;
}