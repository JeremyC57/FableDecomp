#ifndef FABLE_ENGINE_CSYSTEMMANAGER_H
#define FABLE_ENGINE_CSYSTEMMANAGER_H
#include "engine/CInitBaseClass.h"
#include "engine/CCountedPointer.h"
struct _GUID {unsigned long Data1;unsigned short Data2,Data3;unsigned char Data4[8];};
struct _RTL_CRITICAL_SECTION_DEBUG;
struct _RTL_CRITICAL_SECTION {_RTL_CRITICAL_SECTION_DEBUG* DebugInfo;long LockCount,RecursionCount;void* OwningThread;void* LockSemaphore;unsigned long SpinCount;};
class CDebugManager;class CInputManager;class CDisplayManager;
class CGraphicBankManager;class CMeshBankManager;class CFontManager;class CProfileManager;
struct HINSTANCE__;struct tagMSG;
#include "fable_string.h"
enum ESystemCode {SYS_OK=1,SYS_ERROR_INIT_DISPLAY_MANAGER=2,SYS_ERROR_INIT_SOUND_MANAGER=3,SYS_ERROR_INIT_WINDOW=4,SYS_ERROR_INIT_DRIVE_MANAGER=5,SYS_ERROR_INIT_JOYSTICK1=6,SYS_ERROR_INIT_JOYSTICK2=7,SYS_ERROR_INIT_JOYSTICK3=8,SYS_ERROR_INIT_JOYSTICK4=9,SYS_ERROR_INIT_MOUSE=10,SYS_ERROR_INIT_KEYBOARD=11,SYS_ERROR_INIT_INPUT_MANAGER=12,SYS_ERROR_INIT_RENDER_MANAGER=13,SYS_ERROR_INIT_DEBUG_MANAGER=14,SYS_ERROR_INIT_SYSTEM_MANAGER=15,SYS_ERROR_INIT_DDRAW=16,SYS_ERROR_INIT_NETWORK_MANAGER=17,SYS_ERROR_INIT_DLL_MANAGER=18,SYS_ERROR_INIT_GENERIC=19};
// Layout: original complete CSystemManager, native size0x180; PDisplayManager+0x60. Composite tail storage is named by its original member.
class CSystemManager:public CInitBaseClass {
private:
 bool WindowsQuit,ApplicationActive,ApplicationHasInputFocus;
 _GUID ApplicationGUID,BestGUID;_GUID* PBestGUID;bool PreferPrimaryDevice;
 _RTL_CRITICAL_SECTION CriticalSection;void* MainFibre;
 CCountedPointer<CDebugManager> LibDebugManager;
 CCountedPointer<CInputManager> PInputManager;
 CCountedPointer<CDisplayManager> PDisplayManager;
 CDebugManager* DebugManager;
 CCountedPointer<CGraphicBankManager> PGraphicBankManager;
 CCountedPointer<CMeshBankManager> PMeshBankManager;
 CCountedPointer<CFontManager> PFontManager;
 unsigned char _pad_0084[8]; // Native ctor009a7440 additional unaccessed composite; original name not recovered.
 bool WindowInitialised,DDrawInitialised,ExclusiveMode;
 HINSTANCE__* WinInstanceHandle;HWND ApplicationWinHandle;int WinShow;char* WinCommandLine;
 CWideString WinAppName;
 unsigned char WinClass[48]; // tagWNDCLASSEXW WinClass at0xa4.
 bool ResolutionSet;unsigned long InitFlags;
 bool EverBeenActive,MouseButton0Pressed,MouseButton1Pressed,MouseButton2Pressed,MouseButton3Pressed,MouseButton4Pressed;
 long MouseZMove;
 bool WindowValid,RestoreExclusive,Restore,WaitWhileInactive,AttachedToExternalWindow,PreventExclusiveModeChanges,UsingAccurateTimer;
 void (__fastcall* AppReinitFunc)(void*);void* AppReinitContext;
 CCountedPointer<CProfileManager> ProfileManager;
 unsigned char SystemColours[100]; // CGuiColoursInfo SystemColours at0x100.
 CWideString TempPath;
 unsigned char ApplicationName[4]; // CCharString ApplicationName at0x168.
 bool ForceMusicPlay;float ForceMusicPlayOffset;
 bool (__fastcall* ErrorFunc)(ESystemCode);
 bool (__fastcall* IMEMessageFunc)(HWND,unsigned int,unsigned int,long,long&);
 bool (__fastcall* MessageFilter)(tagMSG*);
public:CDisplayManager* GetDisplayManager(){return PDisplayManager.Get();}
};
FABLE_STATIC_ASSERT(sizeof(CSystemManager)==0x180);
CSystemManager* __fastcall GFGetSystemManager();
#endif
