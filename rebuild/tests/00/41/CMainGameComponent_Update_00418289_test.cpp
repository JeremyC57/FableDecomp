// Behaviour test for 0x00418289 CMainGameComponent::Update (controller-disconnect pause, update timing, saved / client event packages)
#include <stdio.h>

static int g_log[64]; static int g_n;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }

class CGameEvent { public: long Type; unsigned char Data[8]; int Size;
    CGameEvent(long type) : Type(type), Size(0) { }
    void AddUBYTE(unsigned char v) { Data[Size++] = v; } };
class CWorld { public: bool Paused; bool IsPaused() const { return Paused; } void UpdateInputRelatedInterpolation() { Log(10); }
    static long GetFrame() { return 777; } };
struct CNetworkClient { bool Local; bool HasSet;
    bool IsLocal() const { return Local; }
    bool GetGameEventPackageSet(int*) { Log(11); return HasSet; } };
class CInputManager { public: bool Loading, Saving;
    bool IsLoadingEvents() const { return Loading; } bool IsSavingEvents() const { return Saving; } };
struct CManager { bool HasError; bool HasErrorMessage() { return HasError; } void ClearErrorMessage() { HasError = false; Log(12); } };
static CManager g_ui; static CManager* GetInstance() { return &g_ui; }
static void ShowErrorMessage(int type) { Log(13); g_ui.HasError = true; (void)type; }
class CSubtitleRenderer { public: bool Interactive; bool IsInInteractiveState() const { return Interactive; } };
static CSubtitleRenderer g_subtitles;
static double g_time;
static double GFGetTime() { g_time += 0.25; return g_time; }

static CInputManager g_input; static CInputManager* PInputManager = &g_input;
static long g_FrameSleepMilliseconds; static bool g_ControllerDisconnectPaused; static long g_ForceRenderFrames;
static unsigned long g_RenderFrame;
static void Sleep(unsigned long) { Log(14); }

struct Game {
    CWorld World; CWorld* PWorld; CNetworkClient Client; int EventPackageSet;
    long CurrentServerFrame; bool LoadingEventPackages, SavingEventPackages, WorldUpdateTurn, AllowRender;
    unsigned long NoRenderFramesSinceLastGameUpdate; float TimePassedSinceLastUpdate, LastUpdateTime;
    bool Disconnected, TimeForUpdate, FromSave; long Difference; int LastEventByte; int Events;
    bool IsControllerDisconnected() const { return Disconnected; }
    void AddGameEvent(const CGameEvent& e) { ++Events; LastEventByte = e.Data[0]; Log(20 + e.Type); }
    bool IsTimeForServerUpdate(long) const { return TimeForUpdate; }
    bool GetEventPackageSetFromSave(int*) { return FromSave; }
    void AddEventPackageSetToSave(const int*) { Log(15); }
    void UpdateFromEventPackageSet(const int*) { Log(16); }
    void SignalFrameUpdate() { Log(17); }
    void GetInputs() { Log(18); }
    long GetFrameDifferenceFromCurrent(long) const { return Difference; }
    void Update()
    {
        if (g_FrameSleepMilliseconds > 0) Sleep(g_FrameSleepMilliseconds);
        CSubtitleRenderer& subtitle_renderer = g_subtitles;
        if (IsControllerDisconnected() && subtitle_renderer.IsInInteractiveState()) {
            if (!GetInstance()->HasErrorMessage()) {
                if (!PWorld->IsPaused()) { CGameEvent event(9); event.AddUBYTE(true); AddGameEvent(event); g_ControllerDisconnectPaused = true; }
                else g_ControllerDisconnectPaused = false;
                ShowErrorMessage(0);
            }
        } else if (GetInstance()->HasErrorMessage()) {
            GetInstance()->ClearErrorMessage();
            if (g_ControllerDisconnectPaused) { CGameEvent event(9); event.AddUBYTE(false); AddGameEvent(event); g_ControllerDisconnectPaused = false; }
        }
        TimePassedSinceLastUpdate = (float)(GFGetTime() - LastUpdateTime);
        LastUpdateTime = (float)GFGetTime();
        WorldUpdateTurn = false;
        if (LoadingEventPackages) {
            if (IsTimeForServerUpdate(CurrentServerFrame)) {
                if (GetEventPackageSetFromSave(&EventPackageSet) == true) {
                    UpdateFromEventPackageSet(&EventPackageSet); NoRenderFramesSinceLastGameUpdate = 0; WorldUpdateTurn = true;
                }
                SignalFrameUpdate();
            }
        } else {
            if (Client.IsLocal() && CurrentServerFrame > 1 && !PInputManager->IsLoadingEvents() && !PInputManager->IsSavingEvents()) {
                long difference = GetFrameDifferenceFromCurrent(CurrentServerFrame);
                if (g_ForceRenderFrames == 0 && difference > 15) { CurrentServerFrame += difference; AllowRender = false; }
            }
            if (Client.GetGameEventPackageSet(&EventPackageSet)) {
                PWorld->UpdateInputRelatedInterpolation(); GetInputs(); SignalFrameUpdate();
                if (SavingEventPackages) AddEventPackageSetToSave(&EventPackageSet);
                UpdateFromEventPackageSet(&EventPackageSet); NoRenderFramesSinceLastGameUpdate = 0; WorldUpdateTurn = true;
            }
        }
        g_RenderFrame = CWorld::GetFrame();
    }
};

static void Reset(Game& g)
{
    g_n = 0; g_time = 0; g.World.Paused = false; g.PWorld = &g.World; g.Client.Local = true; g.Client.HasSet = true;
    g.CurrentServerFrame = 5; g.LoadingEventPackages = g.SavingEventPackages = false; g.WorldUpdateTurn = true; g.AllowRender = true;
    g.NoRenderFramesSinceLastGameUpdate = 3; g.LastUpdateTime = 0.0f; g.Disconnected = false; g.TimeForUpdate = true; g.FromSave = true;
    g.Difference = 0; g.Events = 0; g.LastEventByte = -1; g_ui.HasError = false; g_subtitles.Interactive = true;
    g_input.Loading = g_input.Saving = false; g_FrameSleepMilliseconds = 0; g_ControllerDisconnectPaused = false; g_ForceRenderFrames = 0;
    g_RenderFrame = 0;
}

int main()
{
    int fails = 0;
    Game g;
    // disconnected, world running: pause event (byte 1), error shown
    Reset(g); g.Disconnected = true; g.Update();
    if (g.Events != 1 || g.LastEventByte != 1 || !g_ControllerDisconnectPaused || !g_ui.HasError) ++fails;
    // reconnected: error cleared and an unpause event (byte 0)
    g.Disconnected = false; g.Update();
    if (g.Events != 2 || g.LastEventByte != 0 || g_ControllerDisconnectPaused || g_ui.HasError) ++fails;
    // disconnected while already paused: no event, flag false, error shown
    Reset(g); g.Disconnected = true; g.World.Paused = true; g.Update();
    if (g.Events != 0 || g_ControllerDisconnectPaused || !g_ui.HasError) ++fails;
    // timing: 0.25 - 0 then 0.5
    if (g.TimePassedSinceLastUpdate != 0.25f || g.LastUpdateTime != 0.5f) ++fails;
    // client path: catch-up and the package set applied
    Reset(g); g.Difference = 20; g_FrameSleepMilliseconds = 3; g.SavingEventPackages = true; g.Update();
    if (g.CurrentServerFrame != 25 || g.AllowRender || !g.WorldUpdateTurn || g.NoRenderFramesSinceLastGameUpdate != 0 || g_RenderFrame != 777) ++fails;
    if (g_n != 7 || g_log[0] != 14 || g_log[1] != 11 || g_log[2] != 10 || g_log[3] != 18 || g_log[4] != 17 || g_log[5] != 15 || g_log[6] != 16) ++fails;
    // no catch-up while loading input events
    Reset(g); g.Difference = 20; g_input.Loading = true; g.Client.HasSet = false; g.Update();
    if (g.CurrentServerFrame != 5 || !g.AllowRender || g.WorldUpdateTurn) ++fails;
    // saved packages
    Reset(g); g.LoadingEventPackages = true; g.Update();
    if (!g.WorldUpdateTurn || g_n != 2 || g_log[0] != 16 || g_log[1] != 17) ++fails;
    Reset(g); g.LoadingEventPackages = true; g.TimeForUpdate = false; g.Update();
    if (g.WorldUpdateTurn || g_n != 0) ++fails;
    if (fails) { printf("00418289_TEST FAIL (%d)\n", fails); return 1; }
    printf("00418289_TEST PASS\n");
    return 0;
}