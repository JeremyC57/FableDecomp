#include <cstdio>
// Behaviour test for 009bf2d0.

// CDisplayManager::FlipToGDISurface: in exclusive mode presents until the screen index wraps to 0; a lost device sets the flag.
struct CDevice { int presents; int lostAt; long Present() { ++presents; return presents == lostAt ? (long)0x88760868 : 0; } };
struct CTimer { int swaps; void OnPostSwapScreens() { ++swaps; } };
struct CDisplayManager {
    CDevice* D3DDevice; long NoBackBuffers; bool ExclusiveMode; long CurrentScreenIndex; bool DeviceLostFlag; long FrameCounter; CTimer* Timer;
    void FlipToGDISurface()
    {
        if (ExclusiveMode) {
            while (CurrentScreenIndex != 0) {
                ++FrameCounter;
                long result = D3DDevice->Present();
                if (result == (long)0x88760868) DeviceLostFlag = true;
                else if (result == 0) { Timer->OnPostSwapScreens(); CurrentScreenIndex = (CurrentScreenIndex + 1) % (NoBackBuffers + 1); }
            }
        }
    }
};
int main()
{
    CDevice dev; dev.presents = 0; dev.lostAt = 1; CTimer t; t.swaps = 0;
    CDisplayManager m; m.D3DDevice = &dev; m.NoBackBuffers = 2; m.ExclusiveMode = true; m.CurrentScreenIndex = 1;
    m.DeviceLostFlag = false; m.FrameCounter = 0; m.Timer = &t;
    m.FlipToGDISurface();
    int ok = m.CurrentScreenIndex == 0 && m.DeviceLostFlag && dev.presents == 3 && t.swaps == 2 && m.FrameCounter == 3;
    m.ExclusiveMode = false; m.CurrentScreenIndex = 2; m.FlipToGDISurface();
    ok = ok && m.CurrentScreenIndex == 2 && dev.presents == 3;
    if (!ok) { std::printf("009bf2d0_TEST FAIL\n"); return 1; }
    std::printf("009bf2d0_TEST PASS\n");
    return 0;
}
