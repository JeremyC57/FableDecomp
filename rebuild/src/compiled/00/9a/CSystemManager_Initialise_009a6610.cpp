// CSystemManager::Initialise(const CSystemManagerInit& init) -- bring the system layer up: copy the init settings, then (once) create the fibre, COM and critical section, the window, scratch buffer, display, input, drive, sound, font, graphic-bank and mesh-bank managers; always create the profile manager, maths tables, system colours and application name
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A6610, 998 bytes, `ret 4`. Real C++ reconstruction of a row previously held only as an oracle.
// Callees: CWideString::operator= 0x0099B7D0 / dtor 0x0099B510, CInitBaseClass::Initialise 0x0099A350 (manifest label CXMVPlaybackInfo ctor
//   is propagated), ConvertThreadToFiber [0x0143FD68], CoInitialize [0x01440644], InitializeCriticalSection [0x0143FEE4], GetCurrentProcess
//   [0x0143FD6C] (result unused), InitGUIDFromName 0x009A52F0, InitWindow 0x009A64B0, Uninitialise 0x009A60D0, CScratchBuffer::Initialise
//   0x009F4910 (global 0x013CB498), CDisplayManagerInit copy 0x009A6A00, operator new 0x00BFEA1A, the manager ctors (display 0x009C0880,
//   input 0x00A60050, sound 0x00A3EF00, font 0x009E2E30, graphic bank 0x009F8690, mesh bank 0x00A0A110, profile 0x009F9160), the
//   counted-pointer Reset COMDATs 0x009A6FD0/7180/73D0/72D0/7340/71F0/7260/7510, CDisplayManager::Init 0x009C0E50 /
//   AddDeviceResetCallback 0x009C0190, CDriveManager::Initialise 0x009E4AD0 (global 0x013CAA28, vtable 0x0129AFF4), default error
//   func 0x009A5260, Math_InitializeCosineLookup 0x00A0DB60, GFInitVectorMath 0x00A5B850, InitSystemColours 0x009A5490,
//   InitApplicationName 0x009A5340, init count 0x013CA608.
// Layout: retail CSystemManager = Ego_r.pdb + 8 from WindowInitialised on (one extra counted pointer at +0x84, as the landed
//   Uninitialise 0x009A60D0 / InitSystemColours 0x009A5490 of this chunk), plus IMEMessageFunc/MessageFilter at +0x178/+0x17c;
//   retail CSystemManagerInit = PDB up to +0x35, then 0x20 more bytes before InputManagerInit (+0x58) / DisplayManagerInit (+0x5c),
//   IsErrorFailureFunc +0x9c, Identifier +0xa0, ScratchBufferSize +0xb4, TempPath +0xbc, +0xc0/+0xc4 the two retail-only callbacks.
//   The manager class sizes are the operator new sizes; manager names past the PDB are descriptive.
// Shape: the base Initialise() == true guard wraps the once-only setup; the profile manager onward always runs.
class CWideString { public:
    void* PStringData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                      // 0x0099B510
};
struct CStringDataC { long RefCount; long Length; };
class CCharString { public:
    CStringDataC* PStringData;
    bool IsEmpty() const { return !PStringData || PStringData->Length == 0; }
};
class CInputManagerInit { public: unsigned char _body[4]; };
class CDisplayManagerInit { public:                       // retail 0x3c bytes
    long Width;                                    // +0x00
    long Height;                                   // +0x04
    unsigned char _pad08[0x14 - 0x08];
    void* WindowHandle;                            // +0x14
    unsigned char _pad18[0x30 - 0x18];
    bool SkipConfigDetection;                      // +0x30
    unsigned char _pad31[0x38 - 0x31];
    CWideString ShaderResourcePath;                // +0x38
    CDisplayManagerInit(const CDisplayManagerInit& other);   // 0x009A6A00
};
typedef bool (__cdecl* TErrorFunc)(long code);
class CSystemManagerInit { public:                        // retail layout
    void* InstanceHandle;                          // +0x00
    char* CommandLine;                             // +0x04
    int WindowsShow;                               // +0x08
    CWideString AppName;                           // +0x0c
    bool ExclusiveMode;                            // +0x10
    bool PreferPrimaryDisplayDevice;               // +0x11
    unsigned long Flags;                           // +0x14
    bool PreventHardwareAcceleration;              // +0x18
    bool AttachToExternalWindow;                   // +0x19
    void* ApplicationWindowHandle;                 // +0x1c
    bool PreventExclusiveModeChanges;              // +0x20
    bool UseAccurateTimer;                         // +0x21
    void* AppReinitFunc;                           // +0x24
    void* AppReinitContext;                        // +0x28
    CCharString FontBankHandle;                    // +0x2c
    CCharString StreamingFontBankHandle;           // +0x30
    bool WaitWhileInactive;                        // +0x34
    bool SkipConfigDetection;                      // +0x35
    unsigned char _pad36[0x38 - 0x36];
    unsigned char SoundManagerInit[0x58 - 0x38];   // +0x38
    CInputManagerInit InputManagerInit;            // +0x58
    CDisplayManagerInit DisplayManagerInit;        // +0x5c
    unsigned char _pad98[0x9c - 0x98];
    TErrorFunc IsErrorFailureFunc;                 // +0x9c
    char Identifier[0x14];                         // +0xa0
    long ScratchBufferSize;                        // +0xb4
    CWideString LogfileName;                       // +0xb8
    CWideString TempPath;                          // +0xbc
    void* IMEMessageFunc;                          // +0xc0
    void* MessageFilter;                           // +0xc4
};
struct SCountBlock { long RefCount; void (__fastcall* Destroy)(void* object); void* Object; };
class CDeviceResetCallback { public: virtual void OnDeviceReset(); };
class CSystemManager;
class CDisplayManager { public:                       // 0x2c8 bytes
    unsigned char _body[0x2c8];
    CDisplayManager(CSystemManager* owner);        // 0x009C0880
    void AddDeviceResetCallback(CDeviceResetCallback* callback);   // 0x009C0190
    bool Init(const CDisplayManagerInit& init, bool exclusive);    // 0x009C0E50
};
class CInputManagerDX { public: unsigned char _body[0x84]; CInputManagerDX(const CInputManagerInit& init, bool exclusive); };   // 0x00A60050
struct CSoundManager { unsigned char _body[0x80]; CSoundManager(const void* init, bool exclusive); };                  // 0x00A3EF00
class CFontManager { public: unsigned char _body[0x3c]; CFontManager(const CCharString& bank, const CCharString& streamingBank); };   // 0x009E2E30
class CGraphicBankManager { public: unsigned char _body[8]; CGraphicBankManager(); };   // 0x009F8690
class CMeshBankManager { public: void* Base0; CDeviceResetCallback ResetCallback; long Pad8; CMeshBankManager(); };   // 0x00A0A110
class CProfileManager { public: unsigned char _body[0x78]; CProfileManager(); };   // 0x009F9160
class CDriveManager { public:                             // 8 bytes, vtable 0x0129AFF4
    virtual void v00();
    long Pad;
    CDriveManager() { }
    long Initialise(CSystemManager* owner);        // 0x009E4AD0
};
class CInputManager;
template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* Info;
};
struct CPDisplay : CCountedPointer<CDisplayManager> { void Reset(CDisplayManager* p); };     // 0x009A6FD0
struct CPInput : CCountedPointer<CInputManagerDX> { void Reset(CInputManagerDX* p); };       // 0x009A7180
struct CPDrive : CCountedPointer<CDriveManager> { void Reset(CDriveManager* p); };           // 0x009A73D0
struct CPSound : CCountedPointer<CSoundManager> { void Reset(CSoundManager* p); };           // 0x009A72D0
struct CPFont : CCountedPointer<CFontManager> { void Reset(CFontManager* p); };              // 0x009A7340
struct CPGraphic : CCountedPointer<CGraphicBankManager> { void Reset(CGraphicBankManager* p); };   // 0x009A71F0
struct CPMesh : CCountedPointer<CMeshBankManager> { void Reset(CMeshBankManager* p); };      // 0x009A7260
struct CPProfile : CCountedPointer<CProfileManager> { void Reset(CProfileManager* p); };     // 0x009A7510
class CInitBaseClass { public:
    virtual void v00();
    bool Valid;                                    // +0x04
    bool Initialise();                             // 0x0099A350
};
class CScratchBuffer : public CInitBaseClass { public: void Initialise(long size); };   // 0x009F4910
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void* __stdcall ConvertThreadToFiber(void* parameter);              // [0x0143FD68]
extern "C" __declspec(dllimport) long __stdcall CoInitialize(void* reserved);                        // [0x01440644]
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION* cs); // [0x0143FEE4]
extern "C" __declspec(dllimport) void* __stdcall GetCurrentProcess();                                // [0x0143FD6C]
extern CPDrive g_DriveManager;                     // 0x013CAA28
extern CScratchBuffer g_ScratchBuffer;             // 0x013CB498
extern unsigned char g_SystemInitialiseCount;      // 0x013CA608
bool __cdecl DefaultErrorFunc(long code);          // 0x009A5260
void __cdecl Math_InitializeCosineLookup();        // 0x00A0DB60
void __cdecl GFInitVectorMath();                   // 0x00A5B850

class CSystemManager : public CInitBaseClass { public:
    bool WindowsQuit;                                   // +0x08
    bool ApplicationActive;                             // +0x09
    bool ApplicationHasInputFocus;                      // +0x0a
    unsigned char _pad0b[0x30 - 0x0b];
    bool PreferPrimaryDevice;                           // +0x30
    _RTL_CRITICAL_SECTION CriticalSection;              // +0x34
    void* MainFibre;                                    // +0x4c
    CCountedPointer<void> LibDebugManager;              // +0x50
    CPInput PInputManager;                              // +0x58
    CPDisplay PDisplayManager;                          // +0x60
    void* DebugManager;                                 // +0x68
    CPGraphic PGraphicBankManager;                      // +0x6c
    CPMesh PMeshBankManager;                            // +0x74
    CPSound PSoundManager;                              // +0x7c
    CPFont PFontManager;                                // +0x84
    bool WindowInitialised;                             // +0x8c
    bool DDrawInitialised;                              // +0x8d
    bool ExclusiveMode;                                 // +0x8e
    void* WinInstanceHandle;                            // +0x90
    void* ApplicationWinHandle;                         // +0x94
    int WinShow;                                        // +0x98
    char* WinCommandLine;                               // +0x9c
    CWideString WinAppName;                             // +0xa0
    unsigned char _padA4[0xd8 - 0xa4];
    unsigned long InitFlags;                            // +0xd8
    bool EverBeenActive;                                // +0xdc
    unsigned char _padDD[0xe4 - 0xdd];
    long MouseZMove;                                    // +0xe4
    bool WindowValid;                                   // +0xe8
    bool RestoreExclusive;                              // +0xe9
    bool Restore;                                       // +0xea
    bool WaitWhileInactive;                             // +0xeb
    bool AttachedToExternalWindow;                      // +0xec
    bool PreventExclusiveModeChanges;                   // +0xed
    bool UsingAccurateTimer;                            // +0xee
    void* AppReinitFunc;                                // +0xf0
    void* AppReinitContext;                             // +0xf4
    CPProfile ProfileManager;                           // +0xf8
    unsigned char SystemColours[0x164 - 0x100];         // +0x100
    CWideString TempPath;                               // +0x164
    unsigned char _pad168[0x16c - 0x168];
    bool ForceMusicPlay;                                // +0x16c
    float ForceMusicPlayOffset;                         // +0x170
    TErrorFunc ErrorFunc;                               // +0x174
    void* IMEMessageFunc;                               // +0x178
    void* MessageFilter;                                // +0x17c
    void InitGUIDFromName(const char* name);            // 0x009A52F0
    bool InitWindow(long width, long height);           // 0x009A64B0
    void Uninitialise();                                // 0x009A60D0
    void InitSystemColours();                           // 0x009A5490
    void InitApplicationName();                         // 0x009A5340
    long Initialise(const CSystemManagerInit& init);
};

long CSystemManager::Initialise(const CSystemManagerInit& init)
{
    WindowInitialised = false;
    DDrawInitialised = false;
    WindowValid = false;
    RestoreExclusive = false;
    Restore = false;
    EverBeenActive = false;
    WaitWhileInactive = init.WaitWhileInactive;
    MouseZMove = 0;
    ForceMusicPlay = false;
    ForceMusicPlayOffset = 0.0f;
    AttachedToExternalWindow = init.AttachToExternalWindow;
    PreventExclusiveModeChanges = init.PreventExclusiveModeChanges;
    ExclusiveMode = init.ExclusiveMode;
    TempPath = init.TempPath;
    IMEMessageFunc = init.IMEMessageFunc;
    MessageFilter = init.MessageFilter;
    if (CInitBaseClass::Initialise() == true) {
        MainFibre = ConvertThreadToFiber(this);
        CoInitialize(0);
        InitializeCriticalSection(&CriticalSection);
        PreferPrimaryDevice = init.PreferPrimaryDisplayDevice;
        WinInstanceHandle = init.InstanceHandle;
        WinCommandLine = init.CommandLine;
        WinShow = init.WindowsShow;
        WinAppName = init.AppName;
        InitGUIDFromName(init.Identifier);
        if (init.IsErrorFailureFunc == 0)
            ErrorFunc = DefaultErrorFunc;
        else
            ErrorFunc = init.IsErrorFailureFunc;
        ApplicationActive = false;
        InitFlags = init.Flags;
        AppReinitFunc = init.AppReinitFunc;
        AppReinitContext = init.AppReinitContext;
        UsingAccurateTimer = init.UseAccurateTimer;
        if (init.Flags & 4) {
            if (!AttachedToExternalWindow) {
                if (!InitWindow(init.DisplayManagerInit.Width, init.DisplayManagerInit.Height)) {
                    Uninitialise();
                    return 4;
                }
            } else {
                ApplicationWinHandle = init.ApplicationWindowHandle;
            }
            WindowInitialised = true;
        }
        GetCurrentProcess();
        if (init.ScratchBufferSize > 0)
            g_ScratchBuffer.Initialise(init.ScratchBufferSize);
        if (init.Flags & 4) {
            CDisplayManagerInit displayInit(init.DisplayManagerInit);
            displayInit.WindowHandle = ApplicationWinHandle;
            displayInit.SkipConfigDetection = init.SkipConfigDetection;
            PDisplayManager.Reset(new CDisplayManager(this));
            PDisplayManager.Data->Init(displayInit, ExclusiveMode);
        }
        if (init.Flags & 1)
            PInputManager.Reset(new CInputManagerDX(init.InputManagerInit, ExclusiveMode));
        g_DriveManager.Reset(new CDriveManager);
        long result = g_DriveManager.Data->Initialise(this);
        if (result != 1) {
            Uninitialise();
            return result;
        }
        if (init.Flags & 0x10)
            PSoundManager.Reset(new CSoundManager(init.SoundManagerInit, ExclusiveMode));
        if (!init.FontBankHandle.IsEmpty() || !init.StreamingFontBankHandle.IsEmpty())
            PFontManager.Reset(new CFontManager(init.FontBankHandle, init.StreamingFontBankHandle));
        PGraphicBankManager.Reset(new CGraphicBankManager);
        PMeshBankManager.Reset(new CMeshBankManager);
        if (PDisplayManager.Data)
            PDisplayManager.Data->AddDeviceResetCallback(PMeshBankManager.Data ? &PMeshBankManager.Data->ResetCallback : 0);
    }
    ProfileManager.Reset(new CProfileManager);
    Math_InitializeCosineLookup();
    GFInitVectorMath();
    InitSystemColours();
    ++g_SystemInitialiseCount;
    InitApplicationName();
    if (!ApplicationActive && EverBeenActive == true)
        Restore = true;
    ApplicationActive = true;
    EverBeenActive = true;
    return 1;
}