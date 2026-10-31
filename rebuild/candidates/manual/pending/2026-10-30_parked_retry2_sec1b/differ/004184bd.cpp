// Grade: DIFFER(238v1285) (sec1b 2026-10-31) -- partial reconstruction of the first 0xC0 bytes (g_pMainGameComponent store, StartProgress, progress-update toggles around InitialiseText, CConsole singleton Init with ConsoleFont +0x16144, the two console globals, FrameTime (+0x68 double) = 1.0f / refresh rate + 0.0001f, the CGameDefinitionManager create-if-absent, the first two progress/profile steps with the literals read from the binary) matching retail's instruction shape; left: zero register ebx (ours edi) and the 8-byte frame, and the remaining ~17 start-up steps and the checkpoint/checksum tail (about 1050 bytes) are not written; several callee names are descriptive (name_review)
// CMainGameComponent::Init() -- the game's start-up sequence (partial: progress start, text, console, frame time, definition manager, the first two steps)
// Leaf: ?Init@CMainGameComponent@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004184BD, 1285 bytes, frame-pointer size TU.
// Names: NProgressDisplay::StartProgress 0x009E9EF0, DisplayProgress 0x009E9F40, CTBaseSingleton<CConsole>::Get 0x00414C90,
//   CGameDefinitionManager::CGameDefinitionManager 0x0044C6C2, Init_RegisterGameComponents 0x004EE23F, NProfileTimer::StartProfile
//   0x009D8240 (labels); descriptive (name_review): EnableProgressUpdates 0x009E9F90, CConsole::Init 0x009ED190,
//   CGameDefinitionManager::Exists 0x0044C6B6 / Register 0x0044C71F, g_ConsoleLine0/1 0x013B7D6C/0x013B7D70.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
namespace NProgressDisplay {
void __fastcall StartProgress(bool a, bool b, long c, long d, long e, long f);   // 0x009E9EF0
void __fastcall EnableProgressUpdates(bool enable);                              // 0x009E9F90
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
namespace NProfileTimer {
void __fastcall StartProfile(const CCharString& name, long a);                   // 0x009D8240
}
struct CConsole {
    void Init();                                       // 0x009ED190
};
template <class T> struct CTBaseSingleton {
    static T* __stdcall Get(long width, long height, void* font);   // 0x00414C90
};
struct CGameDefinitionManager {
    unsigned char _data[0xe0];
    CGameDefinitionManager();                          // 0x0044C6C2
    static bool __fastcall Exists();                   // 0x0044C6B6
    void Register();                                   // 0x0044C71F
};
void* __cdecl operator new(unsigned int bytes);        // 0x00BFEA1A
void __fastcall Init_RegisterGameComponents();         // 0x004EE23F
struct CDisplayEngine { unsigned char _pad[0x1c8]; long RefreshRate; };   // +0x1c8
extern CDisplayEngine* g_pDisplay;                     // 0x013B8390
extern long g_ConsoleLine0;                            // 0x013B7D6C
extern long g_ConsoleLine1;                            // 0x013B7D70
class CMainGameComponent;
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0
#pragma pack(push, 4)
class CMainGameComponent {
public:
    virtual void Init();
    unsigned char _pad04[0x68 - 0x04];
    double        FrameTime;                           // +0x68
    unsigned char _pad70[0x16144 - 0x70];
    void*         ConsoleFont;                         // +0x16144
    void InitialiseText();                             // 0x00416832
};
#pragma pack(pop)
void CMainGameComponent::Init()
{
    g_pMainGameComponent = this;
    NProgressDisplay::StartProgress(true, false, 0, 0, 0, 0);
    NProgressDisplay::EnableProgressUpdates(false);
    InitialiseText();
    NProgressDisplay::EnableProgressUpdates(true);
    CTBaseSingleton<CConsole>::Get(0x60, 0x29, ConsoleFont)->Init();
    g_ConsoleLine1 = 0;
    g_ConsoleLine0 = 0;
    FrameTime = 1.0f / (float)g_pDisplay->RefreshRate + 0.0001f;
    if (!CGameDefinitionManager::Exists())
        (new CGameDefinitionManager)->Register();
    NProgressDisplay::DisplayProgress("Init Thing Components", -1.0f, false, false);
    Init_RegisterGameComponents();
    NProfileTimer::StartProfile("Init Definition Manager", 0);
}
