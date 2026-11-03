#include <cstdio>

// Model of CEngineCamera::ConsoleRunDebugScript (retail 0x00B2FC10): start the debug script at element 0 and stamp the time.
static double g_time = 0.0;
static int g_timeCalls = 0;
static double GFGetTime() { ++g_timeCalls; return g_time; }

struct CGenericVarVector { int dummy; };

class CEngineCamera { public:
    long   CurrentDebugScriptElement;
    bool   DebugScriptRunning;
    double DebugScriptElementStartTime;
    static void ConsoleRunDebugScript(CEngineCamera& camera, const CGenericVarVector& args)
    {
        (void)args;
        camera.DebugScriptRunning = true;
        camera.CurrentDebugScriptElement = 0;
        camera.DebugScriptElementStartTime = GFGetTime();
    }
};

int main()
{
    CEngineCamera cam;
    cam.CurrentDebugScriptElement = 7;
    cam.DebugScriptRunning = false;
    cam.DebugScriptElementStartTime = -1.0;
    CGenericVarVector args;
    args.dummy = 0;
    g_time = 42.25;
    CEngineCamera::ConsoleRunDebugScript(cam, args);
    if (!cam.DebugScriptRunning || cam.CurrentDebugScriptElement != 0 || g_timeCalls != 1) { std::printf("00b2fc10_TEST FAIL\n"); return 1; }
    double d = cam.DebugScriptElementStartTime - 42.25;
    if (d < -1e-9 || d > 1e-9) { std::printf("00b2fc10_TEST FAIL\n"); return 1; }
    std::printf("00b2fc10_TEST PASS\n");
    return 0;
}