// Behaviour test for 0x00B23B80 CEngine::SetWindow: the forwarder must call CEngineCamera::SetGameWindow on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class C2DBoxF { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEngineCamera { public:
    void SetGameWindow(const C2DBoxF* window)
    {
        ++gCalls; gThis = this;
        gPtr[0] = window;
    }
};
static CEngineCamera gComponent;
CEngineCamera* g_pEngineCamera = &gComponent;
class CEngine { public:
    virtual void SetWindow(const C2DBoxF* window);
};
void CEngine::SetWindow(const C2DBoxF* window)
{
    g_pEngineCamera->SetGameWindow(window);
}
int main()
{
    C2DBoxF o0;
    CEngine engine;
    engine.SetWindow(&o0);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0) {
        std::printf("00b23b80_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23b80_TEST PASS\n");
    return 0;
}
