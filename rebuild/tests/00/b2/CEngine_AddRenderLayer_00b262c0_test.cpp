#include <cstdio>
#include <cstring>

// Model of CEngine::AddRenderLayer (retail 0x00B262C0): report progress, push the layer, return it by reference.
class CCharString { public:
    const char* text;
    CCharString(const char* t, long length = -1) : text(t) { (void)length; }
};
static const char* g_lastText = 0;
static float g_lastProgress = 0.0f;
static int g_progressCalls = 0;
static void DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    ++g_progressCalls; g_lastText = text.text; g_lastProgress = progress; (void)a; (void)b;
}

class CEngineLayerRenderer { public: int id; };

static int g_overflow = 0;
struct LayerVector {
    CEngineLayerRenderer** start;
    CEngineLayerRenderer** finish;
    CEngineLayerRenderer** eos;
    void push_back(CEngineLayerRenderer* const& x)
    {
        if (finish != eos) { *finish = x; ++finish; }
        else ++g_overflow;
    }
};

class CEngine { public:
    LayerVector RenderLayers;
    CEngineLayerRenderer& AddRenderLayer(CEngineLayerRenderer* layer)
    {
        DisplayProgress("Engine: Add Render Layer", -1.0f, false, false);
        RenderLayers.push_back(layer);
        return *layer;
    }
};

int main()
{
    CEngineLayerRenderer* storage[1];
    CEngineLayerRenderer l1, l2;
    CEngine engine;
    engine.RenderLayers.start = storage; engine.RenderLayers.finish = storage; engine.RenderLayers.eos = storage + 1;
    CEngineLayerRenderer& r1 = engine.AddRenderLayer(&l1);
    if (&r1 != &l1 || storage[0] != &l1 || engine.RenderLayers.finish != storage + 1) { std::printf("00b262c0_TEST FAIL\n"); return 1; }
    if (g_progressCalls != 1 || std::strcmp(g_lastText, "Engine: Add Render Layer") != 0 || g_lastProgress != -1.0f) { std::printf("00b262c0_TEST FAIL\n"); return 1; }
    CEngineLayerRenderer& r2 = engine.AddRenderLayer(&l2);
    if (&r2 != &l2 || g_overflow != 1 || g_progressCalls != 2) { std::printf("00b262c0_TEST FAIL\n"); return 1; }
    std::printf("00b262c0_TEST PASS\n");
    return 0;
}