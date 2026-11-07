#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CSYSTEMMANAGERINIT
#define FABLE_ENGINE_GFMAIN_CSYSTEMMANAGERINIT
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CSystemManager.h"
#include "engine/CInputManagerInit.h"
#include "engine/CDisplayManagerInit.h"
#include "fable_string.h"
class CSystemManagerInit{public:
 HINSTANCE__*InstanceHandle;char*CommandLine;int WindowsShow;CWideString AppName;
 bool ExclusiveMode,PreferPrimaryDisplayDevice;unsigned long Flags;bool PreventHardwareAcceleration,AttachToExternalWindow;HWND ApplicationWindowHandle;
 bool PreventExclusiveModeChanges,UseAccurateTimer;void(__fastcall*AppReinitFunc)(void*);void*AppReinitContext;
 CCharString FontBankHandle,StreamingFontBankHandle;bool WaitWhileInactive,SkipConfigDetection;
 unsigned long Body_38[8]; // Layout: native sound-initialization block at+38; absent from all three donor versions.
 CInputManagerInit InputManagerInit;CDisplayManagerInit DisplayManagerInit;
 bool UseTripleBuffer;bool(__fastcall*IsErrorFailureFunc)(ESystemCode);char Identifier[17];long ScratchBufferSize;
 CWideString LogfileName,TempPath;bool(__fastcall*IMEMessageFunc)(HWND,unsigned int,unsigned int,long,long&);bool(__fastcall*MessageFilter)(tagMSG*);
 CSystemManagerInit();
};
FABLE_STATIC_ASSERT(sizeof(CSystemManagerInit)==200);
FABLE_STATIC_ASSERT(offsetof(CSystemManagerInit,DisplayManagerInit)==0x5c);
#endif

#else

#endif
