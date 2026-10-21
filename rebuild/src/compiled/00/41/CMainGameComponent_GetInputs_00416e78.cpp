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
struct C3DVector { float X, Y, Z; };
struct CMemoryBuffer { };
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent() : Player(-1), Valid(false) { }
};
struct CChecksum {
    unsigned long WorldChecksum;
    unsigned long Frame;
    long          InputChecksum;
    unsigned long Spare;
    CChecksum(unsigned long world, unsigned long frame, long input) : WorldChecksum(world), Frame(frame), InputChecksum(input), Spare(0) { }
};
struct CRightHandedSet { C3DVector Up, Forward; };   // 24 bytes (Ego_r.pdb CCamera RHSet +0x0c..+0x24)
struct CCamera {                                   // 56 bytes (Ego_r.pdb)
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
struct CInputManager {
    virtual void v00(); virtual void v01();
    virtual void Update();                         // slot 2
    void UpdateChecksum(long channel, CChecksum* checksum);   // 0x009F4A90
};
extern CInputManager* g_pInputManager;             // 0x013B8388
struct CSystemManager {
    bool IsApplicationActive();                    // 0x009A57B0
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
long GFGetPreMainMemoryUsed();                     // 0x0049D870
struct CGamePlayerInterface {
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