// Behaviour test for CTCDoor::OnSerialise(CPersistContext&, bool) @ 0x00731170: "Open" (default false) always,
// "OldOpen" (default CurrentOpen) only for a full transfer, then Open(false) for an open door unless Mode is 1 or 3.
#include <cstdio>
#include <cstring>
struct CTCDoor {
    void* __vftable; long m4; unsigned char _pad_0x08[0x4]; long Jammed;
    bool CurrentOpen; bool OldOpen; bool Opening; bool Closing;
    long LastCreatureFoundWF; long UpdateID; unsigned char TriggerType[0x4]; void* PDef_Object;
};
static int g_transfers = 0;
static const char* g_names[4];
static bool g_defaults[4];
static int g_openCalls = 0;
static bool g_openArg = true;
struct CPersistContext {
    unsigned char _pad[0x18];
    long          Mode;
    bool          LoadedOpen;
    void Transfer(const char* name, bool& value, const bool& defaultValue)
    {
        g_names[g_transfers] = name; g_defaults[g_transfers] = defaultValue; ++g_transfers;
        if (std::strcmp(name, "Open") == 0) value = LoadedOpen;
    }
};
struct CTCDoor_Methods : CTCDoor {
    void Open(bool instant) { ++g_openCalls; g_openArg = instant; }
    void OnSerialise(CPersistContext& context, bool fullState);
};
void CTCDoor_Methods::OnSerialise(CPersistContext& context, bool fullState)
{
    context.Transfer("Open", CurrentOpen, false);
    if (fullState) {
        context.Transfer("OldOpen", OldOpen, CurrentOpen);
        if (context.Mode != 1 && context.Mode != 3 && CurrentOpen)
            Open(false);
    }
}
static bool Run(long mode, bool loadedOpen, bool full, int wantTransfers, int wantOpen)
{
    CTCDoor_Methods d; d.CurrentOpen = false; d.OldOpen = false;
    CPersistContext c; c.Mode = mode; c.LoadedOpen = loadedOpen;
    g_transfers = 0; g_openCalls = 0; g_openArg = true;
    d.OnSerialise(c, full);
    if (g_transfers != wantTransfers || g_openCalls != wantOpen) return false;
    if (std::strcmp(g_names[0], "Open") != 0 || g_defaults[0] != false) return false;
    if (wantTransfers == 2 && (std::strcmp(g_names[1], "OldOpen") != 0 || g_defaults[1] != loadedOpen)) return false;
    if (wantOpen && g_openArg != false) return false;
    return true;
}
int main()
{
    if (!Run(0, true, true, 2, 1) || !Run(2, true, true, 2, 1) || !Run(1, true, true, 2, 0) || !Run(3, true, true, 2, 0)
        || !Run(0, false, true, 2, 0) || !Run(0, true, false, 1, 0)) { std::printf("00731170_TEST FAIL\n"); return 1; }
    std::printf("00731170_TEST PASS\n");
    return 0;
}
