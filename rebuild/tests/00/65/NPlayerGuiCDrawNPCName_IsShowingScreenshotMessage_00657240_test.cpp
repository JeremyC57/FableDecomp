#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

static long g_frame; class CDisplayEngine { public: long GetFrame() { return g_frame; } }; static CDisplayEngine g_eng; static CDisplayEngine* g_pDisplayEngine = &g_eng;
struct CDrawNPCName { unsigned char p[0x28]; void* Name; long ScreenShotMessageCounter;
    bool IsShowingScreenshotMessage() { if (ScreenShotMessageCounter > 0) { long framesSince = g_pDisplayEngine->GetFrame() - ScreenShotMessageCounter; if (fabsf((float)framesSince) < 10.0f) return true; } return false; } };
int main() { CDrawNPCName n; n.ScreenShotMessageCounter = 0; g_frame = 5; if (n.IsShowingScreenshotMessage()) { std::printf("00657240_TEST FAIL\n"); return 1; }
    n.ScreenShotMessageCounter = 100; g_frame = 105; if (!n.IsShowingScreenshotMessage()) { std::printf("00657240_TEST FAIL\n"); return 1; }
    g_frame = 95; if (!n.IsShowingScreenshotMessage()) { std::printf("00657240_TEST FAIL\n"); return 1; }
    g_frame = 120; if (n.IsShowingScreenshotMessage()) { std::printf("00657240_TEST FAIL\n"); return 1; }
    std::printf("00657240_TEST PASS\n"); return 0; }