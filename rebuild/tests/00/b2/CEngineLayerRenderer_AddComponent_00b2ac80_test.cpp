#include <cstdio>

// Model of CEngineLayerRenderer::AddComponent (retail 0x00B2AC80): push a non-null component onto the layer's list.
class CEngineComponent { public: int id; };

static int g_overflow = 0;
struct ComponentVector {
    CEngineComponent** start;
    CEngineComponent** finish;
    CEngineComponent** eos;
    void insert_overflow(CEngineComponent** pos, CEngineComponent* const& x, unsigned int n, bool atEnd)
    {
        ++g_overflow; (void)pos; (void)x; (void)n; (void)atEnd;
    }
    void push_back(CEngineComponent* const& x)
    {
        if (finish != eos) { *finish = x; ++finish; }
        else insert_overflow(finish, x, 1, true);
    }
};

class CEngineLayerRenderer { public:
    long Layer;
    unsigned long Flags;
    unsigned long SceneFilterFlags;
    ComponentVector Components;
    void AddComponent(CEngineComponent* component)
    {
        if (component)
            Components.push_back(component);
    }
};

int main()
{
    CEngineComponent* storage[2];
    CEngineComponent a, b, c;
    CEngineLayerRenderer layer;
    layer.Components.start = storage; layer.Components.finish = storage; layer.Components.eos = storage + 2;
    layer.AddComponent(0);
    if (layer.Components.finish != storage) { std::printf("00b2ac80_TEST FAIL\n"); return 1; }
    layer.AddComponent(&a);
    layer.AddComponent(&b);
    if (layer.Components.finish != storage + 2 || storage[0] != &a || storage[1] != &b || g_overflow != 0) { std::printf("00b2ac80_TEST FAIL\n"); return 1; }
    layer.AddComponent(&c);
    if (g_overflow != 1 || layer.Components.finish != storage + 2) { std::printf("00b2ac80_TEST FAIL\n"); return 1; }
    std::printf("00b2ac80_TEST PASS\n");
    return 0;
}