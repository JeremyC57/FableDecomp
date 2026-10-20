// CSystemManagerInit::CSystemManagerInit() -- default engine start-up parameters ("BBB GUID" identifier, bbb.log, C:\temp, 128K scratch buffer)
// Leaf: ??0CSystemManagerInit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403B10, 265 bytes (+7 int3 padding).
// Callees: CWideString::CWideString() 0x0099AED0, CCharString::CCharString() 0x0099E4B0,
//   CDisplayManagerInit::CDisplayManagerInit() 0x004016F0 (landed; manifest label CRegionDef is propagated),
//   strncpy 0x00BFE9C2, CWideString::operator=(const wchar_t*) 0x0099B800.
// Layout (retail, 0xc8 bytes; Ego_r.pdb's CSystemManagerInit is 0xa0 and lacks the +0x38..+0x57 block and the two
//   trailing words, so `this` is a local model): InstanceHandle +0x00, CommandLine +0x04, WindowsShow +0x08,
//   AppName +0x0c, ExclusiveMode +0x10, PreferPrimaryDisplayDevice +0x11, Flags +0x14, PreventHardwareAcceleration
//   +0x18, AttachToExternalWindow +0x19, ApplicationWindowHandle +0x1c, PreventExclusiveModeChanges +0x20,
//   UseAccurateTimer +0x21, AppReinitFunc +0x24, AppReinitContext +0x28, FontBankHandle +0x2c,
//   StreamingFontBankHandle +0x30, WaitWhileInactive +0x34, SkipConfigDetection +0x35, eight retail-only words
//   +0x38..+0x54 (two sizes, 64.0f/2.0f/1.0f, two zeroes, a 256K size: names descriptive), InputManagerInit +0x58
//   (three bools), DisplayManagerInit +0x5c (0x3c bytes), UseTripleBuffer +0x98, IsErrorFailureFunc +0x9c,
//   Identifier char[17] +0xa0, ScratchBufferSize +0xb4, LogfileName +0xb8, TempPath +0xbc, two retail-only words
//   +0xc0/+0xc4.
// Shape: the eight retail-only words are an embedded record whose inline constructor assigns them in retail's store
//   order (+0x3c before +0x38, +0x50 before +0x4c), and the input flags are an inline-constructed member, so both are
//   stored between the string constructors and the display-init call; every other scalar is a body assignment in retail store order.
//   `memset(Identifier, 0, 17)` is the intrinsic (four dword + one byte zero stores), then strncpy of 8 chars and an
//   explicit terminator at [8].
#include <string.h>

struct HINSTANCE__;
struct HWND__;

struct CWideString {
    void* m_pStringData;
    CWideString();                                           // 0x0099AED0
    const CWideString& operator=(const unsigned short* text);   // 0x0099B800
};

struct CCharString {
    void* m_pStringData;
    CCharString();                                           // 0x0099E4B0
};

struct CInputManagerInit {
    bool UseMouse;                                           // +0x00
    bool UseJoystick;                                        // +0x01
    bool UseKeyboard;                                        // +0x02
    CInputManagerInit() : UseMouse(false), UseJoystick(false), UseKeyboard(false) {}
};

struct CDisplayManagerInit {
    unsigned char _body[0x38];
    CWideString   ShaderResourcePath;                        // +0x38
    CDisplayManagerInit();                                   // 0x004016F0
};

// Retail-only 0x20-byte block between SkipConfigDetection and InputManagerInit (not in Ego_r.pdb; names descriptive).
// Its inline constructor assigns in the retail store order (+0x04 before +0x00, +0x18 before +0x14).
struct SStartupSizes {
    long  m_secondarySize;                                   // +0x00 (0x80)
    long  m_primarySize;                                     // +0x04 (0x40)
    float m_scaleA;                                          // +0x08 (64.0f)
    float m_scaleB;                                          // +0x0c (2.0f)
    float m_scaleC;                                          // +0x10 (1.0f)
    long  m_reservedA;                                       // +0x14
    long  m_reservedB;                                       // +0x18
    long  m_bufferSize;                                      // +0x1c (0x40000)
    SStartupSizes()
    {
        m_primarySize = 0x40;
        m_secondarySize = 0x80;
        m_scaleA = 64.0f;
        m_scaleB = 2.0f;
        m_scaleC = 1.0f;
        m_reservedB = 0;
        m_reservedA = 0;
        m_bufferSize = 0x40000;
    }
};

enum ESystemCode { SYSTEM_OK = 0 };

struct CSystemManagerInit {
    HINSTANCE__*          InstanceHandle;                    // +0x00
    char*                 CommandLine;                       // +0x04
    long                  WindowsShow;                       // +0x08
    CWideString           AppName;                           // +0x0c
    bool                  ExclusiveMode;                     // +0x10
    bool                  PreferPrimaryDisplayDevice;        // +0x11
    unsigned long         Flags;                             // +0x14
    bool                  PreventHardwareAcceleration;       // +0x18
    bool                  AttachToExternalWindow;            // +0x19
    HWND__*               ApplicationWindowHandle;           // +0x1c
    bool                  PreventExclusiveModeChanges;       // +0x20
    bool                  UseAccurateTimer;                  // +0x21
    void (__cdecl*        AppReinitFunc)(void*);             // +0x24
    void*                 AppReinitContext;                  // +0x28
    CCharString           FontBankHandle;                    // +0x2c
    CCharString           StreamingFontBankHandle;           // +0x30
    bool                  WaitWhileInactive;                 // +0x34
    bool                  SkipConfigDetection;               // +0x35
    SStartupSizes         StartupSizes;                      // +0x38 (retail-only block, 0x20 bytes)
    CInputManagerInit     InputManagerInit;                  // +0x58
    CDisplayManagerInit   DisplayManagerInit;                // +0x5c
    bool                  UseTripleBuffer;                   // +0x98
    bool (__cdecl*        IsErrorFailureFunc)(ESystemCode);  // +0x9c
    char                  Identifier[17];                    // +0xa0
    long                  ScratchBufferSize;                 // +0xb4
    CWideString           LogfileName;                       // +0xb8
    CWideString           TempPath;                          // +0xbc
    long                  m_extraA;                          // +0xc0
    long                  m_extraB;                          // +0xc4
    CSystemManagerInit();
};

CSystemManagerInit::CSystemManagerInit()
{
    InstanceHandle = 0;
    CommandLine = 0;
    ExclusiveMode = true;
    Flags = 0;
    IsErrorFailureFunc = 0;
    PreferPrimaryDisplayDevice = false;
    memset(Identifier, 0, 17);
    strncpy(Identifier, "BBB GUID", 8);
    Identifier[8] = 0;
    LogfileName = L"bbb.log";
    ScratchBufferSize = 0x20000;
    AttachToExternalWindow = false;
    PreventExclusiveModeChanges = false;
    ApplicationWindowHandle = 0;
    UseAccurateTimer = false;
    AppReinitFunc = 0;
    AppReinitContext = 0;
    TempPath = L"C:\\temp";
    m_extraA = 0;
    m_extraB = 0;
    WaitWhileInactive = true;
}
