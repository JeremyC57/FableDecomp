// behaviour copy of CMouseDX::CMouseDX(CInputManagerDX*, bool): field defaults, event buffers cleared, ShowCursor(!exclusive),
// engine cursor enabled, then SetExclusiveMode(exclusive)
#include <cstdio>
static int g_showCursorArg = -1, g_cursorEnabled = -1, g_exclusiveArg = -1, g_order = 0, g_clearOrder = 0, g_showOrder = 0;
static void ShowCursorStub(int show) { g_showCursorArg = show; g_showOrder = ++g_order; }
class CMouse { public:
    long NoStoredEvents; long NoEventScansRunning; void* Manager; bool BaseExclusive;
    CMouse(void* manager, bool exclusive) : NoStoredEvents(5), NoEventScansRunning(3), Manager(manager), BaseExclusive(exclusive) {}
    void ClearStoredEvents() { NoStoredEvents = 0; g_clearOrder = ++g_order; }
};
struct CMouseDX : CMouse {
    void* DMouse; void* InputManagerDX; float MouseStep; bool ExclusiveMode, InputEnabled, ForceRecentre, DrawingCursor;
    long ExclusiveModeEventsCount, NoExclusiveModeEvents; bool LMBOn, MMBOn, RMBOn, Filtering; void* CursorObject; void* CursorCount;
    CMouseDX(void* manager, bool exclusiveMode) : CMouse(manager, exclusiveMode), InputManagerDX(manager), CursorObject(0), CursorCount(0)
    {
        DMouse = 0; MouseStep = 1.0f; InputEnabled = true;
        NoEventScansRunning = 0;
        ClearStoredEvents();
        ForceRecentre = false; DrawingCursor = true; ExclusiveModeEventsCount = 0; NoExclusiveModeEvents = 0;
        LMBOn = false; MMBOn = false; RMBOn = false; Filtering = false;
        ShowCursorStub(!exclusiveMode);
        g_cursorEnabled = 1;
        SetExclusiveMode(exclusiveMode);
    }
    bool SetExclusiveMode(bool e) { g_exclusiveArg = e; ExclusiveMode = e; return true; }
};
int main()
{
    int mgr = 0;
    CMouseDX m(&mgr, true);
    if (m.InputManagerDX != &mgr || m.Manager != &mgr || m.DMouse != 0 || m.MouseStep != 1.0f || !m.InputEnabled || m.NoStoredEvents != 0
        || m.NoEventScansRunning != 0 || m.ForceRecentre || !m.DrawingCursor || m.ExclusiveModeEventsCount != 0 || m.LMBOn || m.Filtering
        || g_showCursorArg != 0 || g_cursorEnabled != 1 || g_exclusiveArg != 1 || !m.ExclusiveMode || g_clearOrder > g_showOrder || m.CursorCount)
    { std::printf("00ab5d00_TEST FAIL\n"); return 1; }
    CMouseDX n(&mgr, false);
    if (g_showCursorArg != 1 || g_exclusiveArg != 0) { std::printf("00ab5d00_TEST FAIL\n"); return 1; }
    std::printf("00ab5d00_TEST PASS\n");
    return 0;
}