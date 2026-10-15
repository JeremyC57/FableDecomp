// CSystemManager::CSystemManager() -- CInitBaseClass base, own vtable, then the member constructors: seven empty counted pointers, the two wide strings, the profile-manager counted pointer and the application-name string
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A7440, length 118 bytes.
// Callees: CInitBaseClass::CInitBaseClass @ 0x0099A310, CWideString::CWideString() @ 0x0099AED0 (twice; catalog label says
// CCharString), CCharString::CCharString() @ 0x0099E4B0. Vtable 0x0129AFFC.
// Shape: every zero store is an inline CCountedPointer ctor (one shared `xor edi,edi` after the vtable store), the string
// members are out-of-line ctors in declaration order; nothing else is initialised.
// Model: PDB CSystemManager (0x178) with the retail layout: one extra counted pointer at +0x84 (retail is 8 bytes larger from
// there on: WinAppName +0xA0, ProfileManager +0xF8, TempPath +0x164, ApplicationName +0x168).
struct CInitBaseClass {
    CInitBaseClass();                           // 0x0099A310
    virtual ~CInitBaseClass();
    unsigned char _pad04[4];
};
struct CWideString {
    void* PString;
    CWideString();                              // 0x0099AED0
};
struct CCharString {
    void* PString;
    CCharString();                              // 0x0099E4B0
};
template <class T> struct CCountedPointer {
    T* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
struct CDebugManager; struct CInputManager; struct CDisplayManager; struct CGraphicBankManager;
struct CMeshBankManager; struct CFontManager; struct CProfileManager; struct CRetailManager;
struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };

struct CSystemManager : CInitBaseClass {        // vtable 0x0129AFFC
    bool WindowsQuit;                           // +0x08
    bool ApplicationActive;                     // +0x09
    bool ApplicationHasInputFocus;              // +0x0a
    GUID ApplicationGUID;                       // +0x0c
    GUID BestGUID;                              // +0x1c
    GUID* PBestGUID;                            // +0x2c
    bool PreferPrimaryDevice;                   // +0x30
    RTL_CRITICAL_SECTION CriticalSection;       // +0x34
    void* MainFibre;                            // +0x4c
    CCountedPointer<CDebugManager> LibDebugManager;         // +0x50
    CCountedPointer<CInputManager> PInputManager;           // +0x58
    CCountedPointer<CDisplayManager> PDisplayManager;       // +0x60
    CDebugManager* DebugManager;                            // +0x68
    CCountedPointer<CGraphicBankManager> PGraphicBankManager;   // +0x6c
    CCountedPointer<CMeshBankManager> PMeshBankManager;     // +0x74
    CCountedPointer<CFontManager> PFontManager;             // +0x7c
    CCountedPointer<CRetailManager> PRetailManager;         // +0x84 (retail only; not in the PDB)
    bool WindowInitialised;                     // +0x8c
    bool DDrawInitialised;                      // +0x8d
    bool ExclusiveMode;                         // +0x8e
    void* WinInstanceHandle;                    // +0x90
    void* ApplicationWinHandle;                 // +0x94
    int WinShow;                                // +0x98
    char* WinCommandLine;                       // +0x9c
    CWideString WinAppName;                     // +0xa0
    unsigned char WinClass[0x30];               // +0xa4 tagWNDCLASSEXW
    bool ResolutionSet;                         // +0xd4
    unsigned long InitFlags;                    // +0xd8
    unsigned char _flagsdc[0xf0 - 0xdc];        // +0xdc EverBeenActive .. UsingAccurateTimer
    void* AppReinitFunc;                        // +0xf0
    void* AppReinitContext;                     // +0xf4
    CCountedPointer<CProfileManager> ProfileManager;        // +0xf8
    unsigned char SystemColours[0x64];          // +0x100 CGuiColoursInfo
    CWideString TempPath;                       // +0x164
    CCharString ApplicationName;                // +0x168
    CSystemManager();
    virtual ~CSystemManager();
};

CSystemManager::CSystemManager()
{
}