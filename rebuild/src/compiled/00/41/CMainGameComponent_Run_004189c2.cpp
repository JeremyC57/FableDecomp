// CMainGameComponent::Run(CGameComponent** next) -- run the main game: start the clocks and event-package I/O, set the presentation interval (or touch the current region), apply the user profile, then loop system update / world tick / frame-time bookkeeping until Quit; hand back the next component, shut down and remember this component
// Retail 0x004189C2, 438 bytes, `ret 4` after `leave`. PDB ?Run@CMainGameComponent@@UAE_NPAPAVCGameComponent@@@Z.
// Callees: the client's empty 0x004AE9C0, __fastcall GFGetTime 0x009E1BC0, GetCurrentGameTime 0x00416231, the two
//   event-package starters 0x00416268 / 0x0041627F (both labelled BeginInputLoading; the second is the saving one),
//   CDisplayManager::ChangePresentationInterval(bool, long, bool) 0x009BFF10 (manager 0x013B8390), CWorld slot 13
//   (+0x34, the world map), the world map's current-region index 0x004FB150 (label "DrawGetEnvironment" is wrong:
//   it returns +0x9c) and GetRegion 0x004FC180, CUserProfileManager::Get 0x0040D2A0 / Apply 0x0040BC80, the display
//   engine's +0x2c object slot 55 (+0xDC), the memory allocator (0x013CAA90) ConsolidateFreeAreas 0x009F2660 /
//   ClearPerformanceStats 0x009F26B0, the empty per-frame hook 0x0098E1B0, GFGetSystemManager 0x009A4EC0 / Update
//   0x009A6460 (2 = quit, 1 = run a frame), the system manager's +0xF8 object's empty 0x009F8C10,
//   UpdateForcedTickSpeed 0x00417747, CRoughFrameCounter::NewFrame 0x009F8BA0, UpdateRegular 0x004162B5, the frame
//   time recorder 0x00416202 (thiscall(double); label "operator_new" is wrong), __fastcall GFLogMemoryUse 0x00415E85,
//   0x0044C6B0 (a __fastcall singleton getter) and its empty 0x009AC9E0(1), Shutdown 0x004175E5.
// Globals: 0x013B85F6 / 0x013B85F5 (load / save event packages), 0x013B865C the package file name, 0x013B8628 the
//   presentation-interval flag, 0x013B7D58 the last component run.
// Layout: CGameComponent Quit +8 / Running +9 (PDB); CMainGameComponent (PDB names): PWorld +0x24, PDisplayEngine +0x28,
//   ForceUpdateTick +0x34, CurrentFrameStartGameTime +0x58, GameStartTime +0x60, Client +0x13AB8,
//   LastRenderFrameStartTime +0x161A8, RoughFPSCounter +0x161BC, NextComponentToRun +0x161C4 (retail is 8 bytes short
//   of the PDB after the client). CRegion +0x24 an intrusive counted object (count +4, slot 1 Delete).
// Shape: the frame-pointer size TU of the landed InitGraphics rows; the region pointer is an inline by-value getter
//   whose discarded result is copied and dropped (`inc ; dec ; jne` on the pointee through AddRef/Release members);
//   the frame start time is read into a local before the second clock call; Running is cleared before *next is stored.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CWideString;
struct CRefCounted {
    virtual void v00();
    virtual void Delete();                         // slot 1
    long RefCount;                                 // +4
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Delete(); }
};
struct CRefPointer {
    CRefCounted* m_p;
    __forceinline CRefPointer(const CRefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    __forceinline ~CRefPointer() { if (m_p) m_p->Release(); }
};
struct CRegion {
    unsigned char _pad00[0x24];
    CRefPointer   m_24;                            // +0x24
    CRefPointer GetPointer() const { return m_24; }
};
struct CWorldMap {
    long GetCurrentRegionIndex() const;            // 0x004FB150
    CRegion& GetRegion(long index);                // 0x004FC180
};
struct CWorld {
    virtual void w00(); virtual void w01(); virtual void w02(); virtual void w03(); virtual void w04(); virtual void w05(); virtual void w06(); virtual void w07();
    virtual void w08(); virtual void w09(); virtual void w10(); virtual void w11(); virtual void w12();
    virtual CWorldMap* GetWorldMap();                 // slot 13 (+0x34)
};
struct CDisplayEngineObject {
    virtual void e00(); virtual void e01(); virtual void e02(); virtual void e03(); virtual void e04(); virtual void e05(); virtual void e06(); virtual void e07();
    virtual void e08(); virtual void e09(); virtual void e10(); virtual void e11(); virtual void e12(); virtual void e13(); virtual void e14(); virtual void e15();
    virtual void e16(); virtual void e17(); virtual void e18(); virtual void e19(); virtual void e20(); virtual void e21(); virtual void e22(); virtual void e23();
    virtual void e24(); virtual void e25(); virtual void e26(); virtual void e27(); virtual void e28(); virtual void e29(); virtual void e30(); virtual void e31();
    virtual void e32(); virtual void e33(); virtual void e34(); virtual void e35(); virtual void e36(); virtual void e37(); virtual void e38(); virtual void e39();
    virtual void e40(); virtual void e41(); virtual void e42(); virtual void e43(); virtual void e44(); virtual void e45(); virtual void e46(); virtual void e47();
    virtual void e48(); virtual void e49(); virtual void e50(); virtual void e51(); virtual void e52(); virtual void e53(); virtual void e54();
    virtual void SetOption(long option, float value);  // slot 55 (+0xDC)
};
struct CDisplayEngine {
    unsigned char         _pad00[0x2c];
    CDisplayEngineObject* m_2c;                    // +0x2C
};
struct CDisplayManager {
    void ChangePresentationInterval(bool enable, long interval, bool apply);   // 0x009BFF10
};
struct CUserProfileManager {
    static CUserProfileManager& __fastcall Get();  // 0x0040D2A0
    void Apply();                                  // 0x0040BC80
};
struct CMemoryAllocator {
    void ConsolidateFreeAreas();                   // 0x009F2660
    void ClearPerformanceStats();                  // 0x009F26B0
};
struct CSystemObjectF8 { void Update(); };         // 0x009F8C10 (empty)
struct CSystemManager {
    long Update();                                 // 0x009A6460
    unsigned char    _pad00[0xf8];
    CSystemObjectF8* m_f8;                         // +0xF8
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
double __fastcall GFGetTime();                     // 0x009E1BC0
void __fastcall GFLogMemoryUse();                  // 0x00415E85
void __fastcall GFFrameHook();                     // 0x0098E1B0 (empty)
struct CSingleton44C6B0 {
    static CSingleton44C6B0& __fastcall Get();     // 0x0044C6B0
    void Tick(long value);                         // 0x009AC9E0 (empty)
};
struct CNetworkClient { void Start(); };           // 0x004AE9C0 (empty)
struct CRoughFrameCounter { long _a, _b; void NewFrame(); };   // 0x009F8BA0
struct CGameComponent;
extern CDisplayManager* g_pDisplayManager;         // 0x013B8390
extern CMemoryAllocator* g_pMemoryAllocator;       // 0x013CAA90
extern bool g_LoadEventPackages;                   // 0x013B85F6
extern bool g_SaveEventPackages;                   // 0x013B85F5
extern CWideString g_EventPackageFileName;         // 0x013B865C
extern bool g_PresentationInterval;                // 0x013B8628
extern CGameComponent* g_pLastComponent;           // 0x013B7D58

struct CGameComponent {
    virtual void v00();
    long            _04;
    bool            Quit;                          // +0x08
    bool            Running;                       // +0x09
};
struct CMainGameComponent : CGameComponent {
    unsigned char      _pad0c[0x18];
    CWorld*            PWorld;                     // +0x24
    CDisplayEngine*    PDisplayEngine;             // +0x28
    unsigned char      _pad2c[8];
    bool               ForceUpdateTick;            // +0x34
    unsigned char      _pad35[0x23];
    double             CurrentFrameStartGameTime;  // +0x58
    double             GameStartTime;              // +0x60
    unsigned char      _pad68[0x13ab8 - 0x68];
    CNetworkClient     Client;                     // +0x13AB8
    unsigned char      _pad13abc[0x161a8 - 0x13abc];
    double             LastRenderFrameStartTime;   // +0x161A8
    unsigned char      _pad161b0[0xc];
    CRoughFrameCounter RoughFPSCounter;            // +0x161BC
    CGameComponent*    NextComponentToRun;         // +0x161C4
    double GetCurrentGameTime();                   // 0x00416231
    void BeginInputLoading(const CWideString& name);   // 0x00416268
    void BeginOutputSaving(const CWideString& name);   // 0x0041627F
    void UpdateForcedTickSpeed();                  // 0x00417747
    void UpdateRegular();                          // 0x004162B5
    void RecordFrameTime(double duration);         // 0x00416202
    void Shutdown();                               // 0x004175E5
    bool Run(CGameComponent** next);
};

bool CMainGameComponent::Run(CGameComponent** next)
{
    Client.Start();
    GameStartTime = GFGetTime();
    Running = true;
    Quit = false;
    LastRenderFrameStartTime = GetCurrentGameTime();
    if (g_LoadEventPackages)
        BeginInputLoading(g_EventPackageFileName);
    else if (g_SaveEventPackages)
        BeginOutputSaving(g_EventPackageFileName);
    if (g_PresentationInterval)
        g_pDisplayManager->ChangePresentationInterval(g_PresentationInterval, 1, true);
    else {
        CWorld* world = PWorld;
        world->GetWorldMap()->GetRegion(world->GetWorldMap()->GetCurrentRegionIndex()).GetPointer();
    }
    CUserProfileManager::Get().Apply();
    PDisplayEngine->m_2c->SetOption(0xc, 20.0f);
    g_pMemoryAllocator->ConsolidateFreeAreas();
    g_pMemoryAllocator->ClearPerformanceStats();
    while (!Quit) {
        GFFrameHook();
        CurrentFrameStartGameTime = GetCurrentGameTime();
        long result = GFGetSystemManager()->Update();
        if (result == 2)
            Quit = true;
        else if (result == 1) {
            if (GFGetSystemManager()->m_f8)
                GFGetSystemManager()->m_f8->Update();
            if (ForceUpdateTick)
                UpdateForcedTickSpeed();
            else {
                RoughFPSCounter.NewFrame();
                UpdateRegular();
            }
        }
        double frameStart = CurrentFrameStartGameTime;
        RecordFrameTime(GetCurrentGameTime() - frameStart);
        GFLogMemoryUse();
        CSingleton44C6B0::Get().Tick(1);
    }
    Running = false;
    *next = NextComponentToRun;
    Shutdown();
    g_pLastComponent = this;
    return *next != 0;
}