#ifndef FABLE_ENGINE_WIN32_TYPES_H
#define FABLE_ENGINE_WIN32_TYPES_H
// Selected Platform SDK declarations (guiddef.h, windef.h, winnt.h, winbase.h) for retail models that hold or pass these types
// (the lane has no Platform SDK; <windows.h> hangs cl under Wine). Spellings and layouts are the SDK's own:
//   _GUID / GUID (guiddef.h, 16 bytes), _RTL_CRITICAL_SECTION / RTL_CRITICAL_SECTION (winnt.h, 0x18 bytes),
//   CRITICAL_SECTION = RTL_CRITICAL_SECTION (winbase.h), Enter/LeaveCriticalSection (kernel32 imports, winbase.h),
//   HWND (windef.h DECLARE_HANDLE; the same typedef as engine/Direct3D9Types.h, which may share the TU).
// Retail evidence: CVideoSys::CriticalSection +0x64 is passed to EnterCriticalSection (IAT 0x0143FE0C) and
// LeaveCriticalSection (IAT 0x0143FE10) by 0x006286F0 and to DeleteCriticalSection by CVideoSys::Close 0x00A3B380;
// Ego_r.pdb types it `_RTL_CRITICAL_SECTION` (CVideoSys, CSystemManager::CriticalSection +0x34).
// Shared by the FABLE_RETAIL_CSYSTEMMANAGER, FABLE_RETAIL_CVIDEOSYS and FABLE_RETAIL_CBASEVIDEORENDERER branches, which
// each defined their own copy before (one SDK type, one definition).
#include "rebuild_abi.h"

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

struct HWND__; typedef HWND__* HWND;

struct _RTL_CRITICAL_SECTION_DEBUG;
typedef struct _RTL_CRITICAL_SECTION {
    _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;
    long          LockCount;
    long          RecursionCount;
    void*         OwningThread;
    void*         LockSemaphore;
    unsigned long SpinCount;
} RTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;

extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10

FABLE_STATIC_ASSERT(sizeof(GUID) == 0x10);
FABLE_STATIC_ASSERT(sizeof(RTL_CRITICAL_SECTION) == 0x18);

#endif // FABLE_ENGINE_WIN32_TYPES_H
