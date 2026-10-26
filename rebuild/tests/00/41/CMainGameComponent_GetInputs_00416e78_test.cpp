// Behaviour test for 00416e78: src/00416e78.cpp copied verbatim, the camera source, input manager, system manager and
// player interface stubbed so the checksum and the event dispatch are observable. CMainGameComponent::GetInputs()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CMainGameComponent::GetInputs() -- fold the camera's interpolated positions (t = 0 and t = 1) into an input checksum, record it with the world checksum and frame in the input manager and update the input manager; while the application is active (and more than one ... is reported), poll the player interface's queued game events, queueing them as game events -- or, while loading, only processing system events
// Leaf: GetInputs
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00416E78, length 393 bytes; size-optimised frame-pointer TU
//   (`#pragma optimize("s", on)` + `optimize("y", off)`), /GS cookie 0x0139C8A8 / __security_check_cookie @ 0x00BFE9F9.
// Callees: the camera source (+0x24 -> +0x34) virtual slot 1 (a whole 56-byte CCamera by hidden pointer, float t), __ftol2 @ 0x00BFEA70,
//   CMainGameComponent::PeekWorldChecksum @ 0x00416392, CInputManager::UpdateChecksum(long, CChecksum*) @ 0x009F4A90 and
//   the input manager's virtual slot 2 on the global @ 0x013B8388, GFGetSystemManager @ 0x009A4EC0 /
//   CSystemManager::IsApplicationActive @ 0x009A57B0, 0x0049D870 (a long query, manifest "GFGetPreMainMemoryUsed"),
//   the player interface (+0x20) member @ 0x004457F0 and virtual slot 1 (pop a game event), AddGameEvent @ 0x0041649C,
//   EventIsSystemEvent @ 0x00415FF2, ProcessEvent @ 0x00415FE0 (landed names).
// Layout: CMainGameComponent members as used here: PlayerInterface +0x20, the camera owner +0x24, input channel +0x48,
//   frame +0x1613C, loading flag +0x16170; CGameEvent (40 bytes) with the default constructor Player(-1), Valid(false) as
//   the landed CPlayer::UnInitActiveMode 0x00633BF0; CChecksum {world, frame, input, spare}.
// Shape: six `sum = (long)(sum + source->GetCamera(t).WorldPosition.c)` statements, each fetching the camera again; the
//   56-byte CCamera temporary (Ego_r.pdb layout) is retail's 0x38-byte frame block at [ebp-78h]. The player interface is
//   read through an inline GetPlayerInterface() accessor, which keeps retail's top-tested event loop.
#pragma optimize("s", on)
#pragma optimize("y", off)
class C3DVector { public: float X, Y, Z; };
struct CMemoryBuffer { };
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent() : Player(-1), Valid(false) { }
};
class CChecksum { public:
    unsigned long WorldChecksum;
    unsigned long Frame;
    long          InputChecksum;
    unsigned long Spare;
    CChecksum(unsigned long world, unsigned long frame, long input) : WorldChecksum(world), Frame(frame), InputChecksum(input), Spare(0) { }
};
class CRightHandedSet { public: C3DVector Up, Forward; };   // 24 bytes (Ego_r.pdb CCamera RHSet +0x0c..+0x24)
class CCamera { public:                                   // 56 bytes (Ego_r.pdb)
    C3DVector       WorldPosition;                 // +0x00
    CRightHandedSet RHSet;                         // +0x0c
    bool            HeightLocked;                  // +0x24
    bool            ViewVecZLocked;                // +0x25
    unsigned long   FOVFlags;                      // +0x28
    float           HorizontalFOV;                 // +0x2c
    float           VerticalFOV;                   // +0x30
    float           Zoom;                          // +0x34
};
struct CCameraSource {
    virtual void v00();
    virtual CCamera GetCamera(float t);            // slot 1 (CCamera by hidden pointer)
};
struct CCameraOwner {
    unsigned char  _pad[0x34];
    CCameraSource* Camera;                         // +0x34
};
class CInputManager { public:
    virtual void v00(); virtual void v01();
    virtual void Update();                         // slot 2
    void UpdateChecksum(long channel, CChecksum* checksum);   // 0x009F4A90
};
extern CInputManager* g_pInputManager;             // 0x013B8388
class CSystemManager { public:
    bool IsApplicationActive();                    // 0x009A57B0
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
long GFGetPreMainMemoryUsed();                     // 0x0049D870
class CGamePlayerInterface { public:
    virtual void v00();
    virtual bool PopGameEvent(CGameEvent* event);  // slot 1
    void BeginEventPoll();                         // 0x004457F0
};
class CMainGameComponent {
public:
    unsigned char         _pad000[0x20];
    CGamePlayerInterface* PlayerInterface;         // +0x20
    CGamePlayerInterface* GetPlayerInterface() const { return PlayerInterface; }
    CCameraOwner*         CameraOwner;             // +0x24
    unsigned char         _pad028[0x48 - 0x28];
    long                  InputChannel;            // +0x48
    unsigned char         _pad04C[0x1613c - 0x4c];
    unsigned long         Frame;                   // +0x1613c
    unsigned char         _pad16140[0x16170 - 0x16140];
    bool                  Loading;                 // +0x16170
    unsigned long PeekWorldChecksum();             // 0x00416392
    void AddGameEvent(const CGameEvent* event);    // 0x0041649C
    bool EventIsSystemEvent(const CGameEvent* event);   // 0x00415FF2
    void ProcessEvent(const CGameEvent* event);    // 0x00415FE0
    void GetInputs();
};

void CMainGameComponent::GetInputs()
{
    CGameEvent event;
    long inputChecksum = (long)CameraOwner->Camera->GetCamera(0.0f).WorldPosition.X;
    inputChecksum = (long)(inputChecksum + CameraOwner->Camera->GetCamera(0.0f).WorldPosition.Y);
    inputChecksum = (long)(inputChecksum + CameraOwner->Camera->GetCamera(0.0f).WorldPosition.Z);
    inputChecksum = (long)(inputChecksum + CameraOwner->Camera->GetCamera(1.0f).WorldPosition.X);
    inputChecksum = (long)(inputChecksum + CameraOwner->Camera->GetCamera(1.0f).WorldPosition.Y);
    inputChecksum = (long)(inputChecksum + CameraOwner->Camera->GetCamera(1.0f).WorldPosition.Z);
    CChecksum checksum(PeekWorldChecksum(), Frame, inputChecksum);
    g_pInputManager->UpdateChecksum(InputChannel, &checksum);
    g_pInputManager->Update();
    if (GFGetSystemManager()->IsApplicationActive() && GFGetPreMainMemoryUsed() > 1) {
        GetPlayerInterface()->BeginEventPoll();
        while (GetPlayerInterface()->PopGameEvent(&event)) {
            if (!Loading)
                AddGameEvent(&event);
            else if (EventIsSystemEvent(&event))
                ProcessEvent(&event);
        }
    }
}

// ---- stubs ----
#pragma optimize("", on)
struct CTestCamera : CCameraSource { };
void CCameraSource::v00() {}
static int g_camCalls = 0;
CCamera CCameraSource::GetCamera(float t)
{ ++g_camCalls; CCamera c; memset(&c, 0, sizeof(c)); c.WorldPosition.X = t ? 100.5f : 10.25f; c.WorldPosition.Y = t ? 200.0f : 20.0f; c.WorldPosition.Z = t ? 300.0f : 30.0f; return c; }
static long g_channel = -1; static CChecksum g_seen(0, 0, 0); static int g_updates = 0;
void CInputManager::v00() {} void CInputManager::v01() {}
void CInputManager::Update() { ++g_updates; }
void CInputManager::UpdateChecksum(long channel, CChecksum* checksum) { g_channel = channel; g_seen = *checksum; }
static CInputManager g_inputs; CInputManager* g_pInputManager = &g_inputs;
static CSystemManager g_sys; static bool g_active = true; static long g_preMain = 2;
CSystemManager* __fastcall GFGetSystemManager() { return &g_sys; }
bool CSystemManager::IsApplicationActive() { return g_active; }
long GFGetPreMainMemoryUsed() { return g_preMain; }
static int g_polls = 0, g_queued = 0, g_begin = 0;
void CGamePlayerInterface::v00() {}
bool CGamePlayerInterface::PopGameEvent(CGameEvent* event)
{ if (g_queued == 0) return false; --g_queued; event->Type = 100 + g_queued; ++g_polls; return true; }
void CGamePlayerInterface::BeginEventPoll() { ++g_begin; }
static int g_added = 0, g_processed = 0, g_systemAsked = 0;
unsigned long CMainGameComponent::PeekWorldChecksum() { return 0xABCD; }
void CMainGameComponent::AddGameEvent(const CGameEvent* event) { ++g_added; }
bool CMainGameComponent::EventIsSystemEvent(const CGameEvent* event) { ++g_systemAsked; return event->Type == 100; }
void CMainGameComponent::ProcessEvent(const CGameEvent* event) { ++g_processed; }
int main()
{
    static CTestCamera cam; static CCameraOwner owner; owner.Camera = &cam;
    static CGamePlayerInterface pi;
    static unsigned char raw[sizeof(CMainGameComponent)];
    CMainGameComponent* gc = (CMainGameComponent*)raw;
    gc->PlayerInterface = &pi; gc->CameraOwner = &owner; gc->InputChannel = 4; gc->Frame = 77; gc->Loading = false;
    g_queued = 3;
    gc->GetInputs();
    // checksum: (long)(... (long)10.25 + 20 + 30 + 100.5 + 200 + 300) with truncation after each step
    long expect = (long)10.25f; expect = (long)(expect + 20.0f); expect = (long)(expect + 30.0f);
    expect = (long)(expect + 100.5f); expect = (long)(expect + 200.0f); expect = (long)(expect + 300.0f);
    CHECK(g_camCalls == 6); CHECK(g_channel == 4); CHECK(g_seen.WorldChecksum == 0xABCD && g_seen.Frame == 77 && g_seen.InputChecksum == expect && g_seen.Spare == 0);
    CHECK(g_updates == 1); CHECK(g_begin == 1 && g_polls == 3 && g_added == 3 && g_processed == 0);
    // loading: only system events are processed
    gc->Loading = true; g_queued = 3; g_added = 0;
    gc->GetInputs();
    CHECK(g_added == 0 && g_systemAsked == 3 && g_processed == 1);
    // inactive application / pre-main count <= 1: no polling
    g_active = false; g_queued = 2; gc->GetInputs(); CHECK(g_begin == 2 && g_queued == 2);
    g_active = true; g_preMain = 1; gc->GetInputs(); CHECK(g_begin == 2);
    if (fails == 0) printf("00416e78_TEST PASS\n"); else printf("00416e78_TEST FAIL\n");
    return fails;
}