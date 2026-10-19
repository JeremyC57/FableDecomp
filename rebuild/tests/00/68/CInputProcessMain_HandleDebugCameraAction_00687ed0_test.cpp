// Behaviour test for CInputProcessMain::HandleDebugCameraAction (0x00687ED0): the events and priority per player mode.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#ifndef _MSC_VER
#define __int64 long long
#endif
struct CProcessedInput {
    long Priority; int Events[8]; int NoEvents;
    void SetPriority(long priority) { Priority = priority; }
};
void __fastcall AddManualCameraEvent_0B(bool on, CProcessedInput& input) { input.Events[input.NoEvents++] = on ? 0xB1 : 0xB0; }
void __fastcall AddManualCameraEvent_0C(bool on, CProcessedInput& input) { input.Events[input.NoEvents++] = on ? 0xC1 : 0xC0; }

struct CPlayer {
    unsigned char _pad000[0x220];
    bool          UsingFreeCamera;                 // +0x220
    long Mode;
    long GetCurrentMode() { return Mode; }
};
struct CInputProcessMain {
    virtual void v00() {}
    void*    PPlayerInterface;                     // +0x04 (CAInputProcess)
    unsigned char InputProcessListPos[0xc];        // +0x08
    void*    World;                                // +0x14 (CInputProcessGameBase)
    CPlayer* MyPlayer;                             // +0x18
    void HandleDebugCameraAction(CProcessedInput& input);
};

void CInputProcessMain::HandleDebugCameraAction(CProcessedInput& input)
{
    switch (MyPlayer->GetCurrentMode()) {
    case 19:
        AddManualCameraEvent_0B(false, input);
        input.SetPriority(5);
        break;
    case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 23:
        break;
    case 1:
        if (MyPlayer->UsingFreeCamera) {
            AddManualCameraEvent_0C(false, input);
            input.SetPriority(5);
        } else {
            AddManualCameraEvent_0B(true, input);
            AddManualCameraEvent_0C(true, input);
            input.SetPriority(5);
        }
        break;
    default:
        AddManualCameraEvent_0B(true, input);
        AddManualCameraEvent_0C(true, input);
        input.SetPriority(5);
        break;
    }
}

static int Run(CInputProcessMain& p, CPlayer& player, long mode, bool freeCam, CProcessedInput& in)
{
    player.Mode = mode; player.UsingFreeCamera = freeCam; in.Priority = -1; in.NoEvents = 0;
    p.HandleDebugCameraAction(in);
    return in.NoEvents;
}
int main()
{
    static CPlayer player; CInputProcessMain p; p.MyPlayer = &player; CProcessedInput in;
    int bad = 0;
    if (Run(p, player, 19, false, in) != 1 || in.Events[0] != 0xB0 || in.Priority != 5) bad = 1;
    if (Run(p, player, 1, true, in) != 1 || in.Events[0] != 0xC0 || in.Priority != 5) bad = 2;
    if (Run(p, player, 1, false, in) != 2 || in.Events[0] != 0xB1 || in.Events[1] != 0xC1 || in.Priority != 5) bad = 3;
    if (Run(p, player, 7, true, in) != 0 || in.Priority != -1) bad = 4;
    if (Run(p, player, 23, false, in) != 0 || Run(p, player, 13, false, in) != 0) bad = 5;
    if (Run(p, player, 2, false, in) != 2 || Run(p, player, 0, false, in) != 2 || Run(p, player, 40, true, in) != 2 || in.Priority != 5) bad = 6;
    if (Run(p, player, 14, false, in) != 2 || Run(p, player, 3, false, in) != 2) bad = 7;
    if (bad) { std::printf("00687ed0_TEST FAIL\n"); return 1; }
    std::printf("00687ed0_TEST PASS\n");
    return 0;
}