#ifndef FABLE_ENGINE_CSYSTEMMANAGERINIT_H
#define FABLE_ENGINE_CSYSTEMMANAGERINIT_H
// New header (B24C4, row 0x00402510 GFMain): CSystemManagerInit, the native (retail) layout, 0xC8 bytes. Ego_r.pdb's
// CSystemManagerInit is 0xA0 bytes: same member names and order, but without the sound block at +0x38 (0x20 bytes) and the two
// trailing callbacks +0xC0 / +0xC4 (as the landed constructor 0x00403B10 and CSystemManager::Initialise 0x009A6610 model it; the
// two callback names are those rows' spellings, typed as CSystemManager's own IMEMessageFunc / MessageFilter members they feed).
// The constructor is out of line in retail (0x00403B10, landed). Declaration only.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/Win32Types.h"
#include "engine/CSystemManager.h"
#include "engine/CSoundManagerInit.h"
#include "engine/CInputManagerInit.h"
#include "engine/CDisplayManagerInit.h"
struct HINSTANCE__; struct tagMSG;
class CSystemManagerInit {
public:
    HINSTANCE__* InstanceHandle;                                // +0x00
    char* CommandLine;                                          // +0x04
    int WindowsShow;                                            // +0x08
    CWideString AppName;                                        // +0x0c
    bool ExclusiveMode;                                         // +0x10
    bool PreferPrimaryDisplayDevice;                            // +0x11
    unsigned long Flags;                                        // +0x14
    bool PreventHardwareAcceleration;                           // +0x18
    bool AttachToExternalWindow;                                // +0x19
    HWND ApplicationWindowHandle;                               // +0x1c
    bool PreventExclusiveModeChanges;                           // +0x20
    bool UseAccurateTimer;                                      // +0x21
    void (__fastcall* AppReinitFunc)(void*);                    // +0x24
    void* AppReinitContext;                                     // +0x28
    CCharString FontBankHandle;                                 // +0x2c
    CCharString StreamingFontBankHandle;                        // +0x30
    bool WaitWhileInactive;                                     // +0x34
    bool SkipConfigDetection;                                   // +0x35
    CSoundManagerInit SoundManagerInit;                         // +0x38 (retail only)
    CInputManagerInit InputManagerInit;                         // +0x58
    CDisplayManagerInit DisplayManagerInit;                     // +0x5c
    bool UseTripleBuffer;                                       // +0x98
    bool (__fastcall* IsErrorFailureFunc)(ESystemCode);         // +0x9c
    char Identifier[17];                                        // +0xa0
    long ScratchBufferSize;                                     // +0xb4
    CWideString LogfileName;                                    // +0xb8
    CWideString TempPath;                                       // +0xbc
    bool (__fastcall* IMEMessageFunc)(HWND, unsigned int, unsigned int, long, long&);   // +0xc0 (retail only)
    bool (__fastcall* MessageFilter)(tagMSG*);                  // +0xc4 (retail only)
    CSystemManagerInit();                                       // 0x00403B10
};
FABLE_STATIC_ASSERT(sizeof(CSystemManagerInit) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, SoundManagerInit) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, DisplayManagerInit) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit, IMEMessageFunc) == 0xc0);
#endif
