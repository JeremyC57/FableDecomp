// Grade: DIFFER(410v418) -- two dead stores: retail keeps the GetFont temporary's `Data = 0 ; Info = 0` (mov [esp+8],ebx ; mov [esp+0Ch],ebx) after its inline release; ours drops them (a named local keeps them but then passes its own address to operator= and swaps eax/ecx in the release)
// CProgressDisplay::StartProgress(bool fullScreen, bool forRelaunch, bool forFrontEnd, bool blackScreen, bool forFileInstaller, bool renderFlag) -- resets the progress state, records the mode flags, loads the textures and the ENG_ARIAL_16 font, and draws the first "Initialising progress display" frame unless debug display is on
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00499AA0, 418 bytes, `ret 18h`; vtable slot (Ego_r.pdb ?StartProgress@CProgressDisplay@@UAEX_N00000@Z).
// Callees: GFGetTime 0x009E1BC0 (+3.0 double 0x012368F0), CCharString::operator=(const char*) 0x0099EFE0 ("" 0x0122D70E),
//   g_pMainGameComponent (0x013B86A0) virtual slot 9, InitialiseTextures (own vtable slot 4), WordWrap_SetOption 0x00A0FDB0
//   (`__fastcall`(0, 1)), GFGetSystemManager 0x009A4EC0 -> +0x84 font bank -> GetFont 0x009E2C80 (hidden result),
//   CCountedPointer::operator= 0x00419463 on the static PFont 0x013B8998, RenderProgress 0x00499880, CCharString 0x0099EBF0/0x0099EAE0.
// Layout: Ego_r.pdb CProgressDisplay with retail 12-byte vectors (-0x14 from TextTags on), packed to 4 (the vfptr is not
//   padded to the doubles' alignment); member-name globals as in the landed DisplayProgress 0x00499C50.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const char* text);               // 0x0099EFE0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00419463
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CFontBank;
struct CFontBankBank {
    CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
struct CSystemManager {
    unsigned char  _pad00[0x84];
    CFontBankBank* FontBank;                                // +0x84
};
CSystemManager* __fastcall GFGetSystemManager();           // 0x009A4EC0
double __fastcall GFGetTime();                             // 0x009E1BC0
void __fastcall WordWrap_SetOption(unsigned int option, unsigned int value);   // 0x00A0FDB0

struct CMainGameComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsDebugDisplayForced();                    // slot 9 (+0x24)
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
extern bool   g_bProgressDisplayEnabled;                    // 0x01375445
extern bool   g_bProgressDisplaySuppressed;                 // 0x013B89D3
extern bool   g_bAutoSaveProgressShown;                     // 0x013B89C1
extern double g_AutoSaveProgressEndTime;                    // 0x013B89C8
extern bool   g_bProgressDebugDisplay;                      // 0x013B85F0
extern bool   g_bCompileFrontendDefinitions;                // 0x013B8648

#pragma pack(push, 4)
struct CProgressDisplay {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void InitialiseTextures(bool forRelaunch, bool forFrontEnd);   // slot 4 (+0x10)
    unsigned char _pad04[0x28 - 0x04];
    long          LoadingScreenIndex;                      // +0x28
    unsigned char _pad2c[0x44 - 0x2c];
    long          TextTagIndex;                            // +0x44
    unsigned char _pad48[4];                               // +0x48 Text
    CCharString   TextTag;                                 // +0x4c
    CCharString   LastTextDisplayed;                       // +0x50
    long          GuildSealFrameIndex;                     // +0x54
    long          StageCount;                              // +0x58
    unsigned char _pad5c[4];
    double        LastUpdateTime;                          // +0x60
    double        LastScreenUpdate;                        // +0x68
    double        LastProgressUpdate;                      // +0x70
    bool          DebugDisplay;                            // +0x78
    bool          Initialised;                             // +0x79
    bool          FullScreen;                              // +0x7a
    bool          BlackScreen;                             // +0x7b
    bool          DisplayText;                             // +0x7c
    bool          DisplayResetProgressTipText;             // +0x7d
    bool          ForRelaunch;                             // +0x7e
    bool          ForFrontEnd;                             // +0x7f
    bool          ForFileInstaller;                        // +0x80
    bool          FileInstallerFinished;                   // +0x81
    static CCountedPointer<CFontBank> PFont;               // 0x013B8998
    void RenderProgress(const CCharString& textTag, float progress, bool a, bool b);   // 0x00499880
    void StartProgress(bool fullScreen, bool forRelaunch, bool forFrontEnd, bool blackScreen, bool forFileInstaller, bool renderFlag);
};
#pragma pack(pop)

void CProgressDisplay::StartProgress(bool fullScreen, bool forRelaunch, bool forFrontEnd, bool blackScreen, bool forFileInstaller, bool renderFlag)
{
    if (!g_bProgressDisplayEnabled)
        return;
    if (!g_bProgressDisplaySuppressed) {
        if (Initialised)
            return;
        if (g_bAutoSaveProgressShown)
            g_AutoSaveProgressEndTime = GFGetTime() + 3.0;
    }
    LastUpdateTime = 0.0;
    LoadingScreenIndex = 0;
    LastScreenUpdate = 0.0;
    TextTagIndex = -1;
    LastProgressUpdate = 0.0;
    TextTag = "";
    LastTextDisplayed = "";
    GuildSealFrameIndex = 0;
    StageCount = 0;
    DebugDisplay = g_bProgressDebugDisplay;
    if ((g_pMainGameComponent && g_pMainGameComponent->IsDebugDisplayForced()) || g_bCompileFrontendDefinitions)
        DebugDisplay = true;
    Initialised = true;
    FullScreen = fullScreen;
    BlackScreen = blackScreen;
    DisplayText = true;
    DisplayResetProgressTipText = false;
    ForRelaunch = forRelaunch;
    ForFrontEnd = forFrontEnd;
    ForFileInstaller = forFileInstaller;
    FileInstallerFinished = false;
    InitialiseTextures(forRelaunch, forFrontEnd);
    WordWrap_SetOption(0, 1);
    {
        CCharString fontName("ENG_ARIAL_16");
        CFontBankBank* bank = GFGetSystemManager()->FontBank;
        PFont = bank->GetFont(fontName);
    }
    if (!DebugDisplay) {
        CCharString text("Initialising progress display");
        RenderProgress(text, -1.0f, false, renderFlag);
    }
}
